//Unit test for the array object

#include<cppunit/TestFixture.h>
#include<cppunit/extensions/HelperMacros.h>

#include "ArrayShape.hpp"

using namespace pni::utils;

class ArrayTest : public CppUnit::TestFixture{
	CPPUNIT_TEST_SUITE(ArrayTest);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testSetAndGet);
	CPPUNIT_TEST(testComparison);
	CPPUNIT_TEST(testTypeInfo);
	CPPUNIT_TEST(testSlowArrayConstruction);
	CPPUNIT_TEST(testAssignment);
	CPPUNIT_TEST_SUITE_END();
private:
	ArrayShape s1,s2;
	UInt32 r1,r2;
public:
	void setUp();
	void tearDown();
	void testConstruction();
	void testAssignment();
	void testSetAndGet();
	void testComparison();
	void testTypeInfo();
	void testSlowArrayConstruction();


};