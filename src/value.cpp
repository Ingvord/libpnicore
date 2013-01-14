/*
 * (c) Copyright 2013 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
 * Created on: Jan 14, 2013
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#include "value.hpp"


namespace pni{
namespace core{

    //-------------------------------------------------------------------------
    void value::_throw_not_allocated_error(const ExceptionRecord &r)
    {
        throw MemoryNotAllocatedError(r, "Instance of value holds no data!");
    }

    //-------------------------------------------------------------------------
    value &value::operator=(const value &o)
    {
        if(this == &o) return *this;
        _ptr = std::unique_ptr<value_holder_interface>(
                o._ptr->clone());

        return *this;
    }
    
    //-------------------------------------------------------------------------
    value &value::operator=(value &&o)
    {
        if(this == &o) return *this;
        _ptr = std::move(o._ptr);
        return *this;
    }
    
    //-------------------------------------------------------------------------
    TypeID value::type_id() const
    {
        if(_ptr)
            return _ptr->type_id();
        else
            _throw_not_allocated_error(EXCEPTION_RECORD);

        return TypeID::NONE; //just to make the compiler happy
    }

    //-------------------------------------------------------------------------
    std::ostream &operator<<(std::ostream &stream,const value &v)
    {
        if(v._ptr)
            return v._ptr->write(stream);
        else 
            v._throw_not_allocated_error(EXCEPTION_RECORD);

        return stream;
    }
    
    //-------------------------------------------------------------------------
    std::istream &operator>>(std::istream &stream,value &v)
    {
        if(v._ptr)
            return v._ptr->read(stream);
        else
            v._throw_not_allocated_error(EXCEPTION_RECORD);

        return stream;
    }
//end of namespace
}
}
