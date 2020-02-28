//
// (c) Copyright 2013 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
//  Created on: Sep 30, 2013
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#pragma once

#include <boost/current_function.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <pni/core/types.hpp>
#include <pni/core/type_erasures.hpp>


using namespace pni::core;


/*!
\brief testing type_id for containers

*/
template<typename CTYPE,type_id_t ID>
class type_id_container_test : public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(type_id_container_test);
        CPPUNIT_TEST(test_type_id);
        CPPUNIT_TEST_SUITE_END();
    public:
        void setUp() {}
        void tearDown() {}
        
        void test_type_id();


};


//-----------------------------------------------------------------------------
template<typename CTYPE,type_id_t ID> 
void type_id_container_test<CTYPE,ID>::test_type_id()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    CTYPE instance;
    CPPUNIT_ASSERT(type_id(instance)==ID);

    //check references
    CTYPE &reference = instance;
    CPPUNIT_ASSERT(type_id(reference)==ID);
    const CTYPE &const_reference = instance;
    CPPUNIT_ASSERT(type_id(const_reference)==ID);

    //check pointers
    CTYPE *pointer = &instance;
    CPPUNIT_ASSERT(type_id(pointer)==ID);
    const CTYPE *const_pointer = &instance;
    CPPUNIT_ASSERT(type_id(const_pointer) == ID);

    
}
