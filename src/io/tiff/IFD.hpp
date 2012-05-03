/*
 * Declaration of class TIFFIFD
 *
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
 * Declaration of class TIFFIFD.
 *
 * Created on: Jun 16, 2011
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 */

#ifndef TIFFIFD_HPP_
#define TIFFIFD_HPP_

#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

#include "../../Types.hpp"
#include "../../Array.hpp"

#include "IFDEntry.hpp"

namespace pni{
namespace io{
namespace tiff{

//IDF Entry type codes

    /*! \ingroup io_classes
    \brief IFD - Image File Directory class

    This class describes an Image File Directory (IFD) block in a TIFF file.
    Each IFD is associated with a single image. An IFD can be considered as a
    container for IFD entries. Each of this entries has a particular type and
    one or several values of this type. The entries can be obtained using the []
    operator in two different fashions: once with in integer argument
    representing the index of the entry in the IFD or with a string giving the
    name of the entry (if the entry is according to the standard). 
    The TIFF specification defines a group of standard entries with defines 
    names. 

    The IFD class behaves like a C++ container providing iterators to run over
    all entries stored in the IFD.  
    */
    class IFD {
        protected:
            std::vector<IFDEntry> _entries; //!< list of IFD entries
        public:
            //===============public data types=================================
            //some data types that can be useful for IFDs
            typedef std::vector<IFDEntry>::iterator       iterator;       //!< iterator over IDF entries in IDFList
            typedef std::vector<IFDEntry>::const_iterator const_iterator; //!< const. iterator over IDF entries in IDF List

            //==================constructors and destructor====================
            //! default constructor
            IFD();
            //! copy constructor
            IFD(const IFD &o);

            /*! \brief standard constructor

            This constructor assumes that the stream points to the first element
            of the IFD which must hold the number of entries. Once construction
            is finished the stream will point to the offset of the next IFD. 
            The calling programm can immediately read the offset and proceed
            with the next IFD.
            \param size number of elements in the entry
            */
            explicit IFD(size_t size);
            //! move constructor
            IFD(IFD &&o);
            //! destructor
            ~IFD();

            //===================assignment operator===========================
            //! copy assignment operator
            IFD &operator = (const IFD &o);

            //! move assignment operator
            IFD &operator = (IFD &&o);

            //====================public member methods========================
            /*! \brief get number of IFD entries

            Returns the number of entries stored in the IFD. 
            \return number of IFDs
            */
            size_t size() const { return _entries.size(); }

            /*! \brief operator to obtain an entry by index

            This woks for all entries stored in the IFD also for those
            not defined by the TIFF specification. If the index requested
            exceeds the number of entries stored in the IFD an exception
            will be raised.

            \param i unsigned integer with the index of the entry
            \return smart pointer to an IFDAbstractEntry object
            */
            IFDEntry operator[](size_t i) const;

            /*! operator to obtain an entry by its name

            This operator works only for entries which are defined in the
            TIFF specification. If the requested entry is not available
            an exception will be raised.

            \param n a string with the name of the entry
            \return smart pointer to an IFDAbstractEntry
            */
            IFDEntry operator[](const String &n) const;

            iterator begin() { return _entries.begin(); }
            iterator end()   { return _entries.end(); }
            const_iterator begin() const { return _entries.begin(); }
            const_iterator end() const { return _entries.end(); }

            //! overloaded ostream operator for standard output

            //! Thiss operator can be used to dump the content of an IFD to standard out.
            friend std::ostream &operator<<(std::ostream &o,const IFD &idf);
    };


//end of namespace
}
}
}


#endif /* TIFFIDF_HPP_ */