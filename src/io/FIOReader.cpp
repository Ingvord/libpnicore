/*
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 * This file is part of libpniutils.
 *
 * libpniutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libpniutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpniutils.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 * 
 * FIOReader class implementation
 *
 * Created on: Apr 30, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 */

#include <sstream>
#include <string>
#include "FIOReader.hpp"

namespace pni{
namespace io{
    //======================private member functions===========================
    void FIOReader::_parse_file(std::ifstream &stream)
    {
        String::value_type buffer; 
        while(!stream.eof())
        {
            //read a character
            stream>>buffer;
            if(buffer == '%')
            {
                //each section in the file starts with a % sign - now we need to
                //figure out which section we are
                stream>>buffer;
                switch(buffer){
                    case 'p':
                        //parameters section
                        _parse_parameters(stream); break;
                    case 'd':
                        //data section
                        _parse_data(stream); 
                        return; 
                        break;
                }

            }
        }

        //reset EOF bit
        stream.clear();
    }

    //-------------------------------------------------------------------------
    void FIOReader::_parse_parameters(std::ifstream &stream)
    {
        String::value_type buffer;
        String param_name;
        //clear the parameter map
        _param_map.clear();

        //read the parameter section
        while(!stream.eof())
        {
            //read a single character form the file
            stream>>buffer;
            
            //reached end of section
            if(buffer == '%') break;

            //handle comment lines
            if(buffer == '!')
            {
                while(stream.get() != '\n');
                continue;
            }

            //the first = character indicates the end of the parameter name
            if(buffer == '=')
            {
                //finished with this parameter
                _param_map.insert({param_name,stream.tellg()});
               
                //reset the paremter name
                param_name.clear();

                //move stream to end of line before continuing with 
                //all other parameters
                while(stream.get() != '\n');
                continue;
            }

            if(buffer != ' ') param_name.push_back(buffer);

        }

        //reset the stream for one position
        stream.seekg(-1,std::ios::cur);
        
    }

    //-------------------------------------------------------------------------
    void FIOReader::_parse_data(std::ifstream &stream)
    {
        boost::regex comment("^!"); //regular expression for comment lines
        boost::regex col("^ Col.*"); //column description match
        //boost::regex dcol("[+-]?\\d*.?\\d*[e]?[+-]?\\d*");
        boost::regex is_dcol("^\\s+[+-]?\\d+\\.?\\d*e?[+-]?\\d*.*");
        boost::smatch match;

        String linebuffer;
        std::streampos data_offset_tmp = 0;
        size_t nr = 0; //number of records

        while(!stream.eof())
        {
            //save the position of the stream pointer in case we will need it
            //later
            data_offset_tmp = stream.tellg();

            //read a line
            std::getline(stream,linebuffer);                
            
            //check if the line matches a column definition line
            if(boost::regex_match(linebuffer,match,col))
            {
                _append_column(_read_column_info(linebuffer));
                continue;
            }

            //if the column belongs to a data line we save the stream pointer
            //and break the loop - we have everything we wanted 
            if(boost::regex_match(linebuffer,match,is_dcol))
            {
                //if the _dataoffset has not been written yet 
                if(!_data_offset) _data_offset = data_offset_tmp;
                _read_data_line(linebuffer);
                nr++;
            }
        }
       
        //set the number of records in the file
        _nrecords(nr);
        //must be called here to clear EOF error bit
        //must be called before next call to seekg
        stream.clear();
    }

    //-------------------------------------------------------------------------
    void FIOReader::_get_parameter_data(std::ifstream &stream,String &value)
        const
    {
        char buffer;
        value.clear();

        while(!stream.eof())
        {
            stream.get(buffer);
            if(buffer == '\n') break;
            value.push_back(buffer);
        }
    }

    //=================implementation of static private methods================
    TypeID FIOReader::_typestr2id(const String &tstr)
    {
        if(tstr == "FLOAT") 
            return TypeID::FLOAT32;
        else if(tstr == "DOUBLE")
            return TypeID::FLOAT64;
        else
            return TypeID::NONE;
    }

    //-------------------------------------------------------------------------
    ColumnInfo FIOReader::_read_column_info(const String &line)
    {
        String cname;
        String ctype;
        size_t cindex;
        std::stringstream ss(line);
        ss>>cname>>cindex>>cname>>ctype;
        
        return ColumnInfo(cname,_typestr2id(ctype),Shape());
    }

    //-------------------------------------------------------------------------
    std::vector<String> FIOReader::_read_data_line(const String &line)
    {
        boost::regex dcol("[+-]?\\d+\\.?\\d*e?[+-]?\\d*");
        std::vector<String> record;

        boost::match_results<String::const_iterator> imatch;
        String::const_iterator start = line.begin();
        String::const_iterator end   = line.end();
        while(boost::regex_search(start,end,imatch,dcol,boost::match_default))
        {
            record.push_back(imatch.str());
            start = imatch[0].second;
        }

        return record;
    }

    //=======================constructors and destructor======================= 
    //default constructor implementation
    FIOReader::FIOReader():SpreadsheetReader() {}

    //--------------------------------------------------------------------------
    //move constructor implementation
    FIOReader::FIOReader(FIOReader &&r):
        SpreadsheetReader(std::move(r)),
        _param_map(std::move(r._param_map)),
        _data_offset(std::move(r._data_offset))
    {}

    //-------------------------------------------------------------------------
    //standard constructor implementation
    FIOReader::FIOReader(const String &n):
        SpreadsheetReader(n),
        _param_map(),
        _data_offset(0)
    {
        _parse_file(_get_stream()); 
    }

    //-------------------------------------------------------------------------
    //! destructor
    FIOReader::~FIOReader()
    {}

    //=================assignment operators====================================
    //move assignment implementation
    FIOReader &FIOReader::operator=(FIOReader &&r)
    {
        if(this == &r) return *this;
        SpreadsheetReader::operator=(std::move(r));
        _param_map = std::move(r._param_map);
        _data_offset = std::move(r._data_offset);

        return *this;
    }

    //=============public memeber methods======================================
    //implementation of nparameters
    size_t FIOReader::nparameters() const
    {
        return _param_map.size();
    }
    
    //-------------------------------------------------------------------------
    //implementation of parameter names
    std::vector<String> FIOReader::parameter_names() const
    {
        std::vector<String> pnames;
        for(auto value: _param_map) pnames.push_back(value.first);
        return pnames;
    }



//end of namespace
}
}