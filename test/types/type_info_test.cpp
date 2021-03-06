//
// (c) Copyright 2012 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
//  Created on: Oct 01, 2012
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/test/unit_test.hpp>
#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif
#include <boost/test/floating_point_comparison.hpp>
#include <pni/core/types.hpp>
#include <boost/mpl/list.hpp>
#include <limits>
#include <climits>

using namespace pni::core;

typedef boost::mpl::list<uint8,uint16,uint32,uint64> unsigned_int_types;
typedef boost::mpl::list<int8,int16,int32,int64> signed_int_types;


BOOST_AUTO_TEST_SUITE(type_info_test)
   
    //========================================================================
    BOOST_AUTO_TEST_CASE_TEMPLATE(test_type_info_uint,T,unsigned_int_types)
    {
        typedef pni::core::type_info<T> info_type;
        typedef std::numeric_limits<T> limits_type; 

        BOOST_CHECK_EQUAL(info_type::min(),limits_type::min());
        BOOST_CHECK_EQUAL(info_type::max(),limits_type::max());
        BOOST_CHECK(info_type::is_integer);
        BOOST_CHECK(!info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);
    }
    
    //========================================================================
    BOOST_AUTO_TEST_CASE_TEMPLATE(test_type_info_int,T,signed_int_types)
    {
        typedef pni::core::type_info<int8> info_type;
        typedef std::numeric_limits<int8> limits_type; 

        BOOST_CHECK_EQUAL(info_type::min(),limits_type::min());
        BOOST_CHECK_EQUAL(info_type::max(),limits_type::max());
        BOOST_CHECK(info_type::is_integer);
        BOOST_CHECK(info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_int_sizes)
    {
        BOOST_CHECK(pni::core::type_info<uint8>::size==1);
        BOOST_CHECK(pni::core::type_info<int8>::size==1);
        BOOST_CHECK(pni::core::type_info<uint16>::size==2);
        BOOST_CHECK(pni::core::type_info<int16>::size==2);
        BOOST_CHECK(pni::core::type_info<uint32>::size==4);
        BOOST_CHECK(pni::core::type_info<int32>::size==4);
        BOOST_CHECK(pni::core::type_info<uint64>::size==8);
        BOOST_CHECK(pni::core::type_info<int64>::size==8);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_float32)
    {
        typedef pni::core::type_info<float32> info_type;
        typedef std::numeric_limits<float32> limits_type; 

        BOOST_CHECK_CLOSE(info_type::min(),-limits_type::max(),1.e-6);
        BOOST_CHECK_CLOSE(info_type::max(),+limits_type::max(),1.e-6);
        BOOST_CHECK(info_type::size==4);
        BOOST_CHECK(!info_type::is_integer);
        BOOST_CHECK(info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);
    }
    
    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_float64)
    {
        typedef pni::core::type_info<float64> info_type;
        typedef std::numeric_limits<float64> limits_type; 

        BOOST_CHECK_CLOSE(info_type::min(),-limits_type::max(),1.e-12);
        BOOST_CHECK_CLOSE(info_type::max(),+limits_type::max(),1.e-12);
        BOOST_CHECK(info_type::size==8);
        BOOST_CHECK(!info_type::is_integer);
        BOOST_CHECK(info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);
    }
    
    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_float128)
    {
        typedef pni::core::type_info<float128> info_type;
        typedef std::numeric_limits<float128> limits_type; 

        BOOST_CHECK_CLOSE(info_type::min(),-limits_type::max(),1.e-12);
        BOOST_CHECK_CLOSE(info_type::max(),+limits_type::max(),1.e-12);
#if defined(_MSC_VER) || defined(__arm__)
	BOOST_CHECK(info_type::size == 8);
#else
        BOOST_CHECK(info_type::size==16);
#endif
        BOOST_CHECK(!info_type::is_integer);
        BOOST_CHECK(info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);
    }
    
//========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_complex32)
    {
        typedef pni::core::type_info<complex32> info_type;
        typedef std::numeric_limits<float32> limits_type; 

        BOOST_CHECK_CLOSE(info_type::min(),-limits_type::max(),1.e-6);
        BOOST_CHECK_CLOSE(info_type::max(),+limits_type::max(),1.e-6);
        BOOST_CHECK(info_type::size==8);
        BOOST_CHECK(!info_type::is_integer);
        BOOST_CHECK(info_type::is_signed);
        BOOST_CHECK(info_type::is_complex);
    }
    
    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_complex64)
    {
        typedef pni::core::type_info<complex64> info_type;
        typedef std::numeric_limits<float64> limits_type; 

        BOOST_CHECK_CLOSE(info_type::min(),-limits_type::max(),1.e-12);
        BOOST_CHECK_CLOSE(info_type::max(),+limits_type::max(),1.e-12);
        BOOST_CHECK(info_type::size==16);
        BOOST_CHECK(!info_type::is_integer);
        BOOST_CHECK(info_type::is_signed);
        BOOST_CHECK(info_type::is_complex);
    }
    
    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_complex128)
    {
        typedef pni::core::type_info<complex128> info_type;
        typedef std::numeric_limits<float128> limits_type; 

        BOOST_CHECK_CLOSE(info_type::min(),-limits_type::max(),1.e-12);
        BOOST_CHECK_CLOSE(info_type::max(),+limits_type::max(),1.e-12);
#if defined(_MSC_VER) || defined(__arm__)
	BOOST_CHECK(info_type::size == 16);
#else
        BOOST_CHECK(info_type::size==32);
#endif
        BOOST_CHECK(!info_type::is_integer);
        BOOST_CHECK(info_type::is_signed);
        BOOST_CHECK(info_type::is_complex);
    }
    
    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_bool)
    {
        typedef pni::core::type_info<bool> info_type;

        BOOST_CHECK_EQUAL(info_type::min(),false);
        BOOST_CHECK_EQUAL(info_type::max(),true);
        BOOST_CHECK(info_type::size==1);
        BOOST_CHECK(info_type::is_integer);
        BOOST_CHECK(!info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_bool_t)
    {
        typedef pni::core::type_info<bool_t> info_type;

        BOOST_CHECK_EQUAL(info_type::min(),false);
        BOOST_CHECK_EQUAL(info_type::max(),true);
        BOOST_CHECK(info_type::size==1);
        BOOST_CHECK(info_type::is_integer);
        BOOST_CHECK(!info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);
    }

    //========================================================================
    BOOST_AUTO_TEST_CASE(test_type_info_binary)
    {
        typedef pni::core::type_info<binary> info_type;
            
        BOOST_CHECK(info_type::size==1);
        BOOST_CHECK(info_type::is_integer);
        BOOST_CHECK(!info_type::is_signed);
        BOOST_CHECK(!info_type::is_complex);

        BOOST_CHECK_EQUAL(info_type::min() ,0);
        BOOST_CHECK_EQUAL(info_type::max() ,255);
    }


BOOST_AUTO_TEST_SUITE_END()
