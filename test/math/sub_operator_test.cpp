/*
 * (c) Copyright 2012 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
 *  Created on: Jul 25, 2012
 *      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */
#include<cppunit/extensions/HelperMacros.h>

#include "sub_operator_test.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<uint8> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<int8> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<uint16> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<int16> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<uint32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<int32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<uint64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<int64> >);

CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<float32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<float64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<float128> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<complex32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<complex64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<dynamic_array<complex128> >);

template<typename T> using farray = fixed_dim_array<T,3>;
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<uint8> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<int8> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<uint16> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<int16> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<uint32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<int32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<uint64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<int64> >);

CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<float32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<float64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<float128> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<complex32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<complex64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<farray<complex128> >);

template<typename T> using sarray = static_array<T,2,3,4>;
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<uint8> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<int8> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<uint16> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<int16> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<uint32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<int32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<uint64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<int64> >);

CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<float32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<float64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<float128> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<complex32> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<complex64> >);
CPPUNIT_TEST_SUITE_REGISTRATION(sub_operator_test<sarray<complex128> >);

