/*
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 * This file is part of libpnicore.
 *
 * libpnicore is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libpnicore is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpnicore.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 *
 * Created on: Jun 1, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#include "array_selection.hpp"

namespace pni{
namespace core{
    //=====================implementation of constructors and destructor========
    array_selection::array_selection(const array_selection &s):
        _oshape(s._oshape),
        _offset(s._offset),
        _stride(s._stride),
        _rank(s._rank),
        _shape(s._shape),
        _size(s._size)
    {}

    //--------------------------------------------------------------------------
    array_selection::array_selection(array_selection &&s):
        _oshape(std::move(s._oshape)),
        _offset(std::move(s._offset)),
        _stride(std::move(s._stride)),
        _rank(std::move(s._rank)),
        _shape(std::move(s._shape)),
        _size(std::move(s._size))
    {
        s._offset = std::vector<size_t>();
        s._oshape = std::vector<size_t>();
        s._stride = std::vector<size_t>();
        s._rank = 0;
        s._shape = std::vector<size_t>();
        s._size = 0;
    }
    //===========implementation of assignment operators=========================
    array_selection &array_selection::operator=(const array_selection &s)
    {
        if(this == &s) return *this;
        _oshape = s._oshape;
        _offset = s._offset;
        _stride = s._stride;
        _rank   = s._rank;
        _shape  = s._shape;
        _size   = s._size;
        return *this;
    }

    //--------------------------------------------------------------------------
    array_selection &array_selection::operator=(array_selection &&s)
    {
        if(this == &s) return *this;
        _oshape = std::move(s._oshape);
        _offset = std::move(s._offset);
        _stride = std::move(s._stride);
        _rank   = std::move(s._rank);
        s._rank = 0;
        _shape  = std::move(s._shape);
        _size   = std::move(s._size);
        s._size = 0;
        return *this;
    }

    //======================implementation of general purpose methods===========

    array_selection array_selection::create(const std::vector<slice> &s)
    {
        std::vector<size_t> shape(s.size());
        std::vector<size_t> offset(s.size());
        std::vector<size_t> stride(s.size());

        size_t index = 0;
#ifdef NOFOREACH
        for(auto iter = s.begin();iter!=s.end();++iter)
        {
            auto slice = *iter;
#else
        for(auto slice: s)
        {
#endif
            offset[index] = slice.first();
            stride[index] = slice.stride();
            shape[index] = pni::core::size(slice);
            index++;
        }

        return array_selection(shape,offset,stride);
    }

    //--------------------------------------------------------------------------
    std::ostream &operator<<(std::ostream &o,const array_selection &s)
    {
        o<<"original data:"<<std::endl;
        for(size_t i=0;i<s._oshape.size();i++)
        {
            o<<i<<":\t"<<s._oshape[i]<<"\t"<<s._offset[i]<<"\t"<<s._stride[i];
            o<<std::endl;
        }
        o<<"effective shape: ( ";
#ifdef NOFOREACH
        for(auto iter = s._shape.begin();iter!=s._shape.end();++iter)
        {
            auto v = *iter;
#else
        for(auto v: s._shape)
        {
#endif
            o<<v<<" ";
        }
        o<<")"<<std::endl;

        return o;
    }
//end of namespace
}
}