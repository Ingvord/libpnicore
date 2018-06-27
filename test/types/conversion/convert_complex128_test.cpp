//
// (c) Copyright 2014 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
//  Created on: Dec 29, 2014
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
#include <pni/core/types/convert.hpp>

using namespace pni::core;

typedef complex128 source_type;

BOOST_AUTO_TEST_SUITE(convert_complex128_test)

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_uint8)
    {
        //typedef uint8 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(1)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_uint16)
    {
        //typedef uint16 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(19)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_uint32)
    {
        //typedef uint32 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(19)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_uint64)
    {
        //typedef uint64 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(19)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_int8)
    {
        //typedef int8 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(19)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_int16)
    {
        //typedef int16 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(19)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_int32)
    {
        //typedef int32 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(-20)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_int64)
    {
        //typedef int64 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROW(convert<target_type>(source_type(19)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_float32)
    {
        //typedef float32 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROWS(convert<target_type>(source_type{19,-4.5}),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_float64)
    {
        //typedef float64 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROWS(convert<target_type>(source_type(19,-4.5)),type_error);

    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_float128)
    {
        //typedef float128 target_type;
        //does not compile
        //CPPUNIT_ASSERT_THROWS(convert<target_type>(source_type(19)),type_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_complex32)
    {
        typedef complex32 target_type;
        typedef float32   base_type;
       
        auto value = convert<target_type>(source_type(19,-4.5));
        BOOST_CHECK_CLOSE(base_type(19),value.real(),1.e-6);
        BOOST_CHECK_CLOSE(base_type(-4.5),value.imag(),1.e-6);

        BOOST_CHECK_THROW(convert<target_type>(source_type(1.e+100,-2e+100)),
                          range_error);
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_complex64)
    {
        typedef complex64 target_type;
        typedef float64   base_type;
        
        auto value = convert<target_type>(source_type(19,-4.5));
        BOOST_CHECK_CLOSE(base_type(19),value.real(),1.e-16);
        BOOST_CHECK_CLOSE(base_type(-4.5),value.imag(),1.e-16);

#if defined __GNUG__ && !defined __arm__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
        BOOST_CHECK_THROW(convert<target_type>(source_type(1.e+400,-2e+400)),
                          range_error);
#pragma GCC diagnostic pop
#endif
    }

    //=========================================================================
    BOOST_AUTO_TEST_CASE(test_to_complex128)
    {
        typedef complex128 target_type;
        typedef float128   base_type;
        
        auto value = convert<target_type>(source_type(19,-4.5));
        BOOST_CHECK_CLOSE(base_type(19),value.real(),1.e-16);
        BOOST_CHECK_CLOSE(base_type(-4.5),value.imag(),1.e-16);
    }

BOOST_AUTO_TEST_SUITE_END()
