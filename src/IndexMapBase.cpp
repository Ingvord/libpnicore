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
 * Definition of a dynamic array template.
 *
 * Created on: Jul 19, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#include "IndexMapBase.hpp"

namespace pni{
namespace utils{
    IndexMapBase::IndexMapBase():_shape() {}

    IndexMapBase::IndexMapBase(const IndexMapBase &m):
        _shape(m._shape)
    {}

    IndexMapBase::IndexMapBase(IndexMapBase &&m):
        _shape(std::move(m._shape))
    {}
    //-------------------------------------------------------------------------
    IndexMapBase &IndexMapBase::operator=(const IndexMapBase &m)
    {
        if(this == &m) return *this;

        _shape = m._shape;
        return *this;
    }

    //-------------------------------------------------------------------------
    IndexMapBase &IndexMapBase::operator=(IndexMapBase &&m)
    {
        if(this == &m) return *this;
        _shape = std::move(m._shape);
        return *this;
    }

    //--------------------------------------------------------------------------
    size_t IndexMapBase::size() const
    {
        //return 0 if the map is not initialized
        if(_shape.size() == 0) return 0;

        size_t s = 1;
        for(auto v: _shape) s*=v;
        return s;
    }

    //--------------------------------------------------------------------------
    size_t IndexMapBase::rank() const
    {
        if(_shape.size() == 0) return 0;
        return _shape.size();
    }

//end of namespace
}
}