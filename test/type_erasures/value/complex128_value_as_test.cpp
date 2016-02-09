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
//  Created on: Jan 9, 2015
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#ifdef GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/test/unit_test.hpp>
#ifdef GCC
#pragma GCC diagnostic pop
#endif
#include <boost/test/floating_point_comparison.hpp>
#include <pni/core/types.hpp>
#include <pni/core/type_erasures.hpp>

#include "fixture.hpp"

typedef complex128 value_type;
typedef pni::core::type_info<float128> sinfo_type;
typedef random_generator<value_type> generator_type;
typedef fixture<value_type> fixture_type;

BOOST_AUTO_TEST_SUITE(complex128_value_as_test)

	fixture_type create_fixture()
	{
		return fixture_type(float128(0.5)*sinfo_type::min(),
			float128(0.5)*sinfo_type::max());
	}

    BOOST_AUTO_TEST_CASE(test_as_uint8)
    {
        typedef uint8 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_uint16)
    {
        typedef uint16 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_uint32)
    {
        typedef uint32 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_uint64)
    {
        typedef uint64 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_int8)
    {
        typedef int8 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_int16)
    {
        typedef int16 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_int32)
    {
        typedef int32 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_int64)
    {
        typedef int64 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_float32)
    {
        typedef float32 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_float64)
    {
        typedef float64 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_float128)
    {
        typedef float128 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_complex32)
    {
        typedef complex32 target_type;
        typedef pni::core::type_info<float32> tinfo_type;
		fixture_type f(float128(float32(0.5)*tinfo_type::min()),
					  float128(float32(0.5)*tinfo_type::max()));
        value v(f.value_1);
        BOOST_CHECK_EQUAL(v.as<target_type>(),
                          convert<target_type>(f.value_1));
    
        v = sinfo_type::min();
        BOOST_CHECK_THROW(v.as<target_type>(),pni::core::range_error);
        v = sinfo_type::min();
        BOOST_CHECK_THROW(v.as<target_type>(),pni::core::range_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_complex64)
    {
        typedef complex64 target_type;
        typedef pni::core::type_info<float64> tinfo_type;
        fixture_type f(float128(float64(0.5)*tinfo_type::min()),
					   float128(float64(0.5)*tinfo_type::max()));
        value v(f.value_1);
        BOOST_CHECK_EQUAL(v.as<target_type>(),
                          convert<target_type>(f.value_1));
       
#ifndef MSVC
		//this does not work for the MSVC 2014 (VS2015) as float64 and 
		//float128 have the same size
        v = sinfo_type::min();
        BOOST_CHECK_THROW(v.as<target_type>(),range_error);
        v = sinfo_type::max();
        BOOST_CHECK_THROW(v.as<target_type>(),range_error);
#endif
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_complex128)
    {
        typedef complex128 target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_EQUAL(v.as<target_type>(),
                          convert<target_type>(f.value_1));
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_string)
    {
        typedef string target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_binary)
    {
        typedef binary target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_as_bool)
    {
        typedef bool_t target_type;
        fixture_type f=create_fixture();
        value v(f.value_1);
        BOOST_CHECK_THROW(v.as<target_type>(),type_error);
    }

BOOST_AUTO_TEST_SUITE_END()
