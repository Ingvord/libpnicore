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
 *  Created on: Jan 9, 2013
 *      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#pragma once
#include<cppunit/TestFixture.h>
#include<cppunit/extensions/HelperMacros.h>
#include<boost/current_function.hpp>

#include <pni/core/data_value.hpp>
#include <functional>

using namespace pni::core;


template<typename T> class data_value_test : public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(data_value_test<T>);
        CPPUNIT_TEST(test_construction);
        CPPUNIT_TEST(test_copy_and_move);
        CPPUNIT_TEST(test_assignment);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        void test_construction();
        void test_copy_and_move();
        void test_assignment();
};

//-----------------------------------------------------------------------------
template<typename T> void data_value_test<T>::setUp() { }

//-----------------------------------------------------------------------------
template<typename T> void data_value_test<T>::tearDown() { }

//-----------------------------------------------------------------------------
template<typename T> void data_value_test<T>::test_construction()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    data_value v1 = T(1);
    data_value v2 = T(2);

    CPPUNIT_ASSERT(v1.as<T>()==T(1));
    CPPUNIT_ASSERT(v2.as<T>()==T(2));

}

//-----------------------------------------------------------------------------
template<typename T> void data_value_test<T>::test_copy_and_move()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    data_value v1 = T(1);
    
    data_value v2(v1);
    CPPUNIT_ASSERT(v1.as<T>() == T(1));
    CPPUNIT_ASSERT(v2.as<T>() == T(1));

    data_value v3(std::move(v2));
    CPPUNIT_ASSERT(v3.as<T>() == v1.as<T>());
    CPPUNIT_ASSERT_THROW(v2.as<T>(),MemoryNotAllocatedError);

}

//-----------------------------------------------------------------------------
template<typename T> void data_value_test<T>::test_assignment()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    data_value v1;
    CPPUNIT_ASSERT_THROW(v1.as<T>(),MemoryNotAllocatedError);

    v1 = T(1);
    CPPUNIT_ASSERT(v1.as<T>() == T(1));

    data_value v2;
    v2 = v1;
    CPPUNIT_ASSERT(v1.as<T>() == v2.as<T>());

    data_value v3;
    v3 = std::move(v2);
    CPPUNIT_ASSERT(v3.as<T>() == v1.as<T>());
    CPPUNIT_ASSERT_THROW(v2.as<T>(),MemoryNotAllocatedError);
}


