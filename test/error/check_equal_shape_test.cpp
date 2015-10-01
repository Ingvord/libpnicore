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
// ============================================================================
//
//  Created on: Apr 8, 2014
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#include <boost/test/unit_test.hpp>
#include <boost/current_function.hpp>
#include <vector>
#include <list>
#include <pni/core/error/exception_utils.hpp>
#include "types.hpp"

using namespace pni::core;

struct check_equal_shape_test_fixture
{
    vector_type                std_vector;
    dynamic_array<float64>     darray_1;
    dynamic_array<uint16>      darray_2;
    fixed_dim_array<uint32,2>  farray_1;
    static_array<size_t,4,4,5> sarray_1;

    check_equal_shape_test_fixture():
        darray_1(dynamic_array<float64>::create(shape_t{2,3})),
        darray_2(dynamic_array<uint16>::create(shape_t{3,2})),
        farray_1(fixed_dim_array<uint32,2>::create(shape_t{2,3}))
    {}
};


BOOST_FIXTURE_TEST_SUITE(check_equal_shape_test,check_equal_shape_test_fixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_no_throw)
{
    BOOST_CHECK(check_equal_shape(darray_1,farray_1));
    BOOST_CHECK(!check_equal_shape(darray_1,darray_2));
    BOOST_CHECK(!check_equal_shape(darray_1,sarray_1));

}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_throw)
{
    BOOST_CHECK_NO_THROW(check_equal_shape(darray_1,farray_1,EXCEPTION_RECORD));
    BOOST_CHECK_THROW(check_equal_shape(darray_1,darray_2,EXCEPTION_RECORD),
                         shape_mismatch_error);
    BOOST_CHECK_THROW(check_equal_shape(darray_1,sarray_1,EXCEPTION_RECORD),
                         size_mismatch_error);

}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_invalid_input)
{
    dynamic_array<float64> d1,d2;

    BOOST_CHECK(check_equal_shape(d1,d2));
    BOOST_CHECK_THROW(check_equal_shape(d1,darray_1,EXCEPTION_RECORD),
                      size_mismatch_error);
}

BOOST_AUTO_TEST_SUITE_END()

