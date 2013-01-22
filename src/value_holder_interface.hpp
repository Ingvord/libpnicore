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
 * Created on: Jan 11, 2013
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */
#pragma once

#include <iostream>
#include "Types.hpp"

namespace pni{
namespace core{

    /*!
    \brief abstract interface for data holder

    Internal class defining the abstract interface for a data value holder. 
    */
    class value_holder_interface
    {
        public:
            //-----------------------------------------------------------------
            //! get type id
            virtual TypeID type_id() const = 0;

            //-----------------------------------------------------------------
            /*!
            \brief clone 

            Clone the actual holder object. 
            \return pointer to new holder instance 
            */
            virtual value_holder_interface *clone() const = 0;

            //-----------------------------------------------------------------
            //! write content to output stream
            virtual std::ostream &write(std::ostream &) const = 0;

            //-----------------------------------------------------------------
            //! read content from input stream
            virtual std::istream &read(std::istream &) = 0;

            //-----------------------------------------------------------------
            //! is reference
            virtual bool holds_reference() const = 0;

    };

//end of namespace
}
}