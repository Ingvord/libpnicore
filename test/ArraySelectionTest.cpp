

#include<cppunit/extensions/HelperMacros.h>

#include <list>

#include "ArraySelectionTest.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(ArraySelectionTest);

//-----------------------------------------------------------------------------
void ArraySelectionTest::setUp()
{

}

//-----------------------------------------------------------------------------
void ArraySelectionTest::tearDown()
{
}

//-----------------------------------------------------------------------------
void ArraySelectionTest::test_construction()
{

    typedef std::vector<size_t> itype;
    //testing default constructor
    ArraySelection sel1;
    CPPUNIT_ASSERT(sel1.rank() == 0);
    CPPUNIT_ASSERT(sel1.shape() == std::vector<size_t>());
    CPPUNIT_ASSERT(sel1.size() == 0);

    //testing standard constructor
    ArraySelection sel2(itype{1,100,100},itype{0,0,0},itype{1,1,1});
    CPPUNIT_ASSERT(sel2.rank() == 2);
    CPPUNIT_ASSERT(sel2.size() == 100*100);
    itype s{100,100};
    CPPUNIT_ASSERT(std::equal(s.begin(),s.end(),sel2.shape().begin()));

    //! copy constructor
    ArraySelection sel3(sel2);
    CPPUNIT_ASSERT(sel3.rank() == sel2.rank());
    CPPUNIT_ASSERT(sel3.size() == sel2.size());
    s = sel3.shape();
    CPPUNIT_ASSERT(std::equal(s.begin(),s.end(),sel2.shape().begin()));

    //! move construction
    ArraySelection sel4(std::move(sel3));
    CPPUNIT_ASSERT(sel4.rank() == sel2.rank());
    CPPUNIT_ASSERT(sel4.size() == sel2.size());
    s = sel4.shape();
    CPPUNIT_ASSERT(std::equal(s.begin(),s.end(),sel2.shape().begin()));

    CPPUNIT_ASSERT(sel3.size() == 0);
    CPPUNIT_ASSERT(sel3.rank() == 0);
    CPPUNIT_ASSERT(sel3.shape() == itype());

}

//-----------------------------------------------------------------------------
void ArraySelectionTest::test_assignment()
{
    ArraySelection sel(itype{100,1,200},itype{1,1,1},itype{1,1,2});
    CPPUNIT_ASSERT(sel.rank() == 2);
    itype s{99,99};
    for(auto v: sel.shape()) std::cout<<v<<" ";
    std::cout<<std::endl;
    CPPUNIT_ASSERT(std::equal(s.begin(),s.end(),sel.shape().begin()));
    std::cout<<sel.size()<<std::endl;
    CPPUNIT_ASSERT(sel.size() == 99*99);

    ArraySelection s1;

    s1 = sel;
    CPPUNIT_ASSERT(s1.rank() == sel.rank());
    CPPUNIT_ASSERT(s1.size() == sel.size());
    CPPUNIT_ASSERT(std::equal(s1.shape().begin(),s1.shape().end(),
                              sel.shape().begin()));

    ArraySelection s2;
    s2 = std::move(s1);
    CPPUNIT_ASSERT(s2.rank() == sel.rank());
    CPPUNIT_ASSERT(s2.size() == sel.size());
    CPPUNIT_ASSERT(std::equal(s2.shape().begin(),s2.shape().end(),
                              sel.shape().begin()));

    CPPUNIT_ASSERT(s1.size() == 0);
    CPPUNIT_ASSERT(s1.rank() == 0);
    CPPUNIT_ASSERT(s1.shape() == itype());

}

//-----------------------------------------------------------------------------
void ArraySelectionTest::test_index()
{
    ArraySelection sel(itype{10,20},itype{1,2},itype{3,2});
    itype s{3,9};
    CPPUNIT_ASSERT(std::equal(s.begin(),s.end(),sel.shape().begin()));
    CPPUNIT_ASSERT(sel.rank()==2);
    CPPUNIT_ASSERT(sel.size() == 3*9);
        
    itype i(sel.rank());
    sel.index(itype{1,3},i);
    itype r{4,8};
    CPPUNIT_ASSERT(std::equal(r.begin(),r.end(),i.begin()));

    ArraySelection sel2(itype{10,1,20},itype{1,1,2},itype{3,1,2});
    i = itype(3);
    sel2.index(itype{1,3},i);
    r = itype{4,1,8};
    CPPUNIT_ASSERT(std::equal(r.begin(),r.end(),i.begin()));


}
