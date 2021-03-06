//
// (c) Copyright 2015 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
//
// This file is part of libpnicore.
//
// libpnicore is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// libpnicore is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libpnicore.  If not, see <http://www.gnu.org/licenses/>.
//
// ===========================================================================
//
//  Created on: Oct 16, 2015
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#pragma once 

#include <algorithm>
#include <pni/core/types.hpp>
#include "../../data_generator.hpp"

template<typename AT> struct md_array_trait
{
    typedef AT mdarray_type; 
    typedef typename mdarray_type::value_type value_type;
    typedef random_generator<value_type> generator_type; 
};

template<typename AT> struct fixture
{
    typename md_array_trait<AT>::generator_type generator; 
    pni::core::shape_t shape; 
    AT mdarray_1;
    AT mdarray_2; 

    fixture():
        generator(),
        shape(pni::core::shape_t{3,2}),
        mdarray_1(AT::create(shape)),
        mdarray_2(AT::create(shape))
    {
        std::generate(mdarray_1.begin(),mdarray_1.end(),generator); 
        std::generate(mdarray_2.begin(),mdarray_2.end(),generator);
    }
    
};
