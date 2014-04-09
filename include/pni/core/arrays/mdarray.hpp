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
// ============================================================================
//
// Created on: Oct 28, 2013
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#pragma once

//#include<memory>
#include<iostream>
#include<sstream>
#include<stdexcept>
#include<utility>
#include<complex>
#include<cstdarg>
#include<cstdio>
#include<memory>


#include "../error/exception_utils.hpp"
#include "../types/types.hpp"
#include "../utilities.hpp"
#include "slice.hpp"
#include "array_view.hpp"
#include "array_factory.hpp"
#include "array_view_utils.hpp"
#include "index_map/index_maps.hpp"
#include "../algorithms.hpp"


namespace pni {
namespace core {
    
    template<typename S,typename MAP,typename IPA> class mdarray; 
    //-------------------------------------------------------------------------
    //!
    //! \ingroup mdim_array_classes
    //! \brief array identifier 
    //!
    //! Specialization of the is_array template for instances of mdarray.
    //!
    template<typename ...ARGS> 
    struct is_array<mdarray<ARGS...>>
    {
        //! type is an array type
        static const bool value = true;
    };

    //-------------------------------------------------------------------------
    //!
    //! \ingroup mdim_array_classes
    //! \brief get data pointer
    //! 
    //! Get the pointer to the data stored in an mdarray.
    //! 
    //! \tparam ARGS mdarray template arguments
    //! \param a reference to the array
    //! \return pointer to data
    template<typename ...ARGS> 
    const typename mdarray<ARGS...>::value_type* data(const mdarray<ARGS...> &a)
    {
        return a.storage().data();
    }
    
    //! 
    //! \ingroup mdim_array_classes
    //! \brief template for a multi-dimensional array class
    //!
    //! \tparam STORAGE storage object to use to keep the data
    //! \tparam IMAP the index map 
    //! \tparam IPA unary (inplace) arithmetics implementation
    //!
    template<
             typename STORAGE,
             typename IMAP=dynamic_cindex_map,
             typename IPA =inplace_arithmetics
            > 
    class mdarray
    {
        public:
            //================public types=====================================
            //! type of the buffer object
            typedef STORAGE storage_type;
            //! arrays element type
            typedef typename storage_type::value_type value_type;  
            //! index map type
            typedef IMAP map_type;
            //! type of the array
            typedef mdarray<storage_type,map_type,IPA> array_type;
            //! iterator type
            typedef typename storage_type::iterator iterator;
            //! const iterator type
            typedef typename storage_type::const_iterator const_iterator; 
            //! reverse iterator
            typedef typename storage_type::reverse_iterator reverse_iterator;
            //! const reverse iterator
            typedef typename storage_type::const_reverse_iterator
                const_reverse_iterator;
            //! inplace arithmetics type
            typedef IPA inplace_arithmetic;
            //! view type
            typedef array_view<array_type> view_type;
            //! const view type
            typedef array_view<const array_type> const_view_type;

            
            //==================public members=================================
            //! type ID of the element type
            static const type_id_t type_id; 
        private:
            //! Index map of the array 
            IMAP _imap;  
            //! instance of STORAGE
            STORAGE _data;  
        public:

            //=================constructors and destructor=====================
            //! default constructor
            mdarray():_imap(),_data() {}

            //-----------------------------------------------------------------
            //!
            //! \brief construct from map and storage
            //!
            //! Construct an array from an index map and a storage. 
            //! \param map the index map instance
            //! \param s array storage
            //!
            explicit mdarray(const map_type &map,const storage_type &s):
                _imap(map),
                _data(s)
            {}

            //-----------------------------------------------------------------
            //!
            //! \brief move construct from map and storage 
            //!  
            //! Move construct an array from rvalue refernces to an index 
            //! map and a storage. 
            //!
            //! \param map rvalue reference to the index map
            //! \param s rvalue reference to the storage
            //!
            explicit mdarray(map_type &&map,storage_type &&s):
                _imap(std::move(map)),
                _data(std::move(s))
            {}

            //-----------------------------------------------------------------
            //!
            //! \brief constrcut from a view
            //!
            //! This constructor creates a new array from an array view 
            //! instance.  The resulting array object has the same shape as 
            //! the view. 
            //! 
            //! \tparam ATYPE storage type of the view
            //! \param view reference to the view
            //!
            template<typename ATYPE>
            explicit mdarray(const array_view<ATYPE> &view):
                _imap(map_utils<map_type>::create(pni::core::shape<shape_t>(view))),
                _data(container_utils<storage_type>::create(view.size()))
            {
                std::copy(view.begin(),view.end(),_data.begin());
            }


            //-----------------------------------------------------------------
            //!
            //! \brief construction from an other array
            //!
            //! This constructor can be used for instance along with 
            //! expression templates in order to construct an array from an 
            //! expression. 
            //! 
            //! \tparam MDARGS template parameters of mdarray
            //! \param array reference to the source array
            //!
            template<typename ...MDARGS>
            explicit mdarray(const mdarray<MDARGS...> &array):
                _imap(map_utils<map_type>::create(pni::core::shape<shape_t>(array))),
                _data(container_utils<storage_type>::create(array.size()))
            {
                //copy data
                for(size_t i=0;i<array.size();++i) (*this)[i] = array[i];
                //std::copy(array.begin(),array.end(),this->begin());
            }

            //====================static methods to create arrays==============
            //!
            //! \brief generic construction function
            //! 
            //! This function can be used for easy array construction. It uses 
            //! the array_factory template in the background. 
            //!
            //! \tparam ARGS variadic argument types
            //! \param arguments variadic argument list
            //! \return array instance
            //!
            template<typename ...ARGS>
            static array_type create(ARGS... arguments)
            {
                typedef array_factory<array_type> factory_type;

                return factory_type::create(std::forward<ARGS>(arguments)...);
            }

            //-----------------------------------------------------------------
            //!
            //! \brief construction from initializer list
            //!
            //! This version of the static create function uses an initializer 
            //! list. 
            //!
            //! 
            template<
                     typename T1,
                     typename T2
                    >
            static array_type create(const std::initializer_list<T1> &l1,
                                     const std::initializer_list<T2> &l2)
            {
                typedef array_factory<array_type> factory_type;

                return factory_type::create(l1,l2);
            }



            //====================assignment operations========================
            //!
            //! \brief assignment from a different array type
            //!
            //! Assign the data from a different array type to this one. 
            //! 
            //! \throws size_mismatch_error if array sizes do not match
            //! \throws shape_mismatch_error if shapes do not match
            //! \tparam MDARGS template parameters of the source type
            //! \param array reference to the source array
            //! \return reference to the updated array
            //!
            template<typename ...MDARGS>
            array_type &operator=(const mdarray<MDARGS...> &array)
            {
                if((void*)this == (void*)&array) return *this;

                for(size_t i=0;i<array.size();++i) (*this)[i] = array[i];

                return *this;
            }

            //-----------------------------------------------------------------
            //!
            //! \brief assignment from an initializer list
            //! 
            //! Assigns the values given by an initializer list to an allocated
            //! array. If the number of elements in the list does not match
            //! the number of elements in the array an exception is thrown. 
            //!
            //! \throws size_mismatch_error if list size and array size do not 
            //! match
            //! \param l reference to an initializer list
            //! \return reference to the array
            //!
            array_type &operator=(const std::initializer_list<value_type> &l)
            {
                if(l.size() != this->size())
                    throw size_mismatch_error(EXCEPTION_RECORD,
                            "Size of array and initializer list do not match");

                std::copy(l.begin(),l.end(),this->begin());
                return *this;
            }
            

            //================public methods===================================
            //! 
            //! \brief get index map
            //!
            //! Returns a const reference to the index map of the array.
            //! \return reference to index map
            //!
            const map_type &map() const { return this->_imap; }

            //----------------------------------------------------------------
            //! 
            //! \brief shape to container
            //! 
            //! \deprecated  This method is deprecated and will be replaced in 
            //! one of the next versions of \c libpnicore. Use 
            //!  pni::core::shape() instead!
            //!
            template<typename CTYPE> CTYPE shape() const
            {
                DEPRECATED_FUNCTION("pni::core::shape()");
                CTYPE c(_imap.rank());
                std::copy(_imap.begin(),_imap.end(),c.begin());
                return c;
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief obtain buffer reference
            //!
            //! Return a const reference to the arrays buffer object.
            //! \return buffer reference
            //!
            const STORAGE &storage() const { return _data; }

            //-----------------------------------------------------------------
            //! 
            //! \brief get size of array
            //!
            //! Returns the total number of elements stored in the array.
            //! \return total number of elements
            //!
            size_t size() const 
            { 
                return _data.size(); 
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief get number of dimensions 
            //! 
            //! \deprecated This method is deprecated and will be removed in 
            //! one of the next versionso of \c libpnicore. Use
            //! pni::core::rank() instead.
            //!
            size_t rank() const 
            { 
                DEPRECATED_FUNCTION("pni::core::rank()");
                return _imap.rank(); 
            }

            //=============operators and methods to access array data==========
            //!
            //! \brief get referece to element i
            //!
            //! Returns a reference to the element at linear index i. No index
            //! checking is done! Thus use this operator with care.
            //!
            //! \param i linear index 
            //! \return reference to the element at linear index i
            //!
            value_type& operator[](size_t i) 
            { 
#ifdef DEBUG
                return at(i);
#else
                return _data[i]; 
#endif
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief get value at i
            //!
            //! Returns the value of the element at the linar array index i. 
            //! No index checking is done! Thus use this operator with care.
            //! 
            //! \param i linear index of the element
            //! \return value of the element at linear index i
            //!
            value_type operator[](size_t i) const 
            { 
#ifdef DEBUG
                return at(i);
#else
                return _data[i]; 
#endif
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief get value at i
            //!
            //! Return a reference to the value at linear index i. This method
            //! performs index checking. 
            //!
            //! \throws index_error if i exceeds array size
            //! \param i linear index of element
            //! \return reference to the value at i
            //!
            value_type &at(size_t i) 
            { 
                try
                {
                    return _data.at(i); 
                }
                catch(std::out_of_range &error)
                {
                    std::stringstream ss;
                    ss<<"Index "<<i<<" is out of range ("<<size()<<")!";
                    throw index_error(EXCEPTION_RECORD,ss.str());
                }
            
            } 

            //-----------------------------------------------------------------
            //! 
            //! \brief get value at i
            //!
            //! Return the value of element i. This method
            //! performs index checking. 
            //! \throws index_error if i exceeds array size
            //! \param i linear index of element
            //! \return value at i
            //! 
            value_type at(size_t i) const 
            { 
                try
                {
                    return _data.at(i); 
                }
                catch(std::out_of_range &error)
                {
                    std::stringstream ss;
                    ss<<"Index "<<i<<" is out of range ("<<size()<<")!";
                    throw index_error(EXCEPTION_RECORD,ss.str());
                }
            
            } 

            //-----------------------------------------------------------------
            //!
            //! \brief insert value at index i
            //!
            //! Insert value at index i. 
            //! 
            //! \throws index_error if i exceeds the size of the array
            //! \param i linear index of the element
            //! \param value the value to store at index i
            //!
            void insert(size_t i,const value_type &value)
            {
                at(i)=value;
            }

            //-----------------------------------------------------------------
            //!
            //! \brief multiindex access
            //!
            //! The () operator allows multindex access to the data stored in 
            //! the array. Like the [] operator it does not perform any checks 
            //! of index ranges and should thus be used with care. However, 
            //! due to the missign checks it is extremely fast. If index 
            //! checking is required have a look the corresponding at() 
            //! member function.
            //!
            //! \tparam ITYPES index types
            //! \param indexes list of index values
            //! \return reference to the value at the given index
            //!
            template<
                     typename... ITYPES,
                     typename = ENABLE_VALID_INDEX(ITYPES)
                    >
            VIEW_TYPE(array_type,ITYPES) operator()(ITYPES... indexes)
            {
                typedef view_provider<array_type,is_view_index<ITYPES...>::value>
                    provider_type;
                return provider_type::get_reference(*this,_imap,indexes...);
            }


            //-----------------------------------------------------------------
            //!
            //! \brief multiindex access
            //!
            //! The () operator allows multindex access to the data stored 
            //! in the array. Like the [] operator it does not perform any 
            //! checks of index ranges and should thus be used with care. 
            //! However, due to the missign checks it is extremely fast. If 
            //! index checking is required have a look the corresponding at() 
            //! member function.
            //!
            //! \tparam ITYPES index types
            //! \param indexes list of index values
            //! \return value at the given index
            //!
            template<
                     typename... ITYPES,
                     typename = ENABLE_VALID_INDEX(ITYPES)
                    >
            CONST_VIEW_TYPE(array_type,ITYPES) 
            operator()(ITYPES ...indexes) const
            {
                typedef view_provider<array_type,is_view_index<ITYPES...>::value>
                    provider_type;
                return provider_type::get_value(*this,_imap,indexes...);
            }

            //-----------------------------------------------------------------
            //!
            //! \brief return array view
            //!
            //! Return a view on the array determined by a set of slices 
            //! stored in a container type CTYPE. 
            //!
            //! \tparam CTYPE slice container type
            //! \param slices reference to the container
            //! \return array_view instance
            //!
            template<
                     typename CTYPE,
                     typename = ENABLE_VIEW_CONT(CTYPE)
                    >
            array_view<array_type> operator()(const CTYPE &slices)
            {
                typedef typename map_type::storage_type index_type;
                typedef array_view<array_type> view_type;

                return view_type(*this, array_selection::create(slices));
                
            }

            //-----------------------------------------------------------------
            //!
            //! \brief return element reference
            //!
            //! Returns the reference to a single elemnt of the array 
            //! determined by a multidimensional index of unsigned integers 
            //! stored in a container of type CTYPE. This method performs no 
            //! range checking. 
            //!
            //! \tparam CTYPE index container type
            //! \param index reference to index container
            //! \return reference to the element
            //!
            template<
                     typename CTYPE,
                     typename = ENABLE_ELEMENT_CONT(CTYPE)
                    >
            value_type &operator()(const CTYPE &index)
            {
               return _data[_imap.offset(index)]; 
            }
            
            //-----------------------------------------------------------------
            //!
            //! \brief return array view
            //!
            //! Return a view on the array determined by a set of slices 
            //! stored in a container type CTYPE. 
            //!
            //! \tparam CTYPE slice container type
            //! \param slices reference to the container
            //! \return array_view instance
            //!
            template<
                     typename CTYPE,
                     typename = ENABLE_VIEW_CONT(CTYPE)
                    >
            array_view<const array_type> operator()(const CTYPE &slices) const
            {
                typedef typename map_type::storage_type index_type;
                typedef array_view<const array_type> view_type;

                return view_type(*this,array_selection::create(slices));
                
            }

            //-----------------------------------------------------------------
            //!
            //! \brief return element value
            //!
            //! Returns the value of a single elemnt of the array determined 
            //! by a multidimensional index of unsigned integers stored in a
            //! container of type CTYPE. This method performs no range 
            //! checking. 
            //!
            //! \tparam CTYPE index container type
            //! \param index reference to index container
            //! \return value of the element
            //!
            template<
                     typename CTYPE,
                     typename = ENABLE_ELEMENT_CONT(CTYPE)
                    >
            value_type operator()(const CTYPE &index) const
            {
                return _data[_imap.offset(index)];
            }

            //-----------------------------------------------------------------
            //!
            //! \brief reference to first element
            //!
            //! Return a reference to the first element in the linear view of 
            //! the array.
            //! 
            //! \return reference to first element
            //!
            value_type &front() { return _data.front(); }

            //-----------------------------------------------------------------
            //!
            //! \brief value of first element
            //!
            //! Return the value of the first element in the linear view of 
            //! the array.
            //! 
            //! \return value of the first element
            //!
            value_type front() const { return _data.front(); }

            //-----------------------------------------------------------------
            //!
            //! \brief reference to last element
            //!
            //! Return a reference to the last element in the linear view of 
            //! the array.
            //! 
            //! \return reference to last element
            //!
            value_type &back() { return _data.back(); }

            //-----------------------------------------------------------------
            //!
            //! \brief value of last element
            //!
            //! Return the value of the last element in the linear view of 
            //! the array. 
            //! 
            //! \return value of last element
            //!
            value_type back() const { return _data.back(); }

            //-----------------------------------------------------------------
            //! 
            //! \brief iterator to first element
            //!
            //! Returns a non-const iterator to the first element in the array.
            //! \return iterator to first element
            //!
            iterator begin() { return _data.begin(); }

            //-----------------------------------------------------------------
            //! 
            //! \brief iterator to last element
            //!
            //! Returns a non-const iterator to the last element in the array. 
            //! 
            //! \return iterator to last element
            //!
            iterator end() { return _data.end(); }

            //-----------------------------------------------------------------
            //! 
            //! \brief const-iterator to first element
            //! 
            //! Returns a const-iterator to the first element in the array.
            //! \return iterator to first element
            //!
            const_iterator begin() const { return _data.begin(); }

            //-----------------------------------------------------------------
            //!
            //! \brief const-iterator to last element
            //!
            //! Returns a const-iterator to the last element in the array.
            //! 
            //! \return iterator to last element
            //!
            const_iterator end() const { return _data.end(); }

            //-----------------------------------------------------------------
            //! 
            //! \brief return reverse iterator to last element
            //! 
            reverse_iterator rbegin() { return _data.rbegin(); }

            //-----------------------------------------------------------------
            //!
            //! \brief return const reverse iterator to last element
            //! 
            const_reverse_iterator rbegin() const { return _data.rbegin(); }

            //-----------------------------------------------------------------
            //! 
            //! \brief return reverse iterator to 0-1 element
            //! 
            reverse_iterator rend() { return this->_data.rend(); }

            //-----------------------------------------------------------------
            //! 
            //! \brief return const reverse iterator to 0-1 element
            //! 
            const_reverse_iterator rend() const { return _data.rend(); }

            //==========implementation of unary arithmetic operators===========
            //!
            //! \brief unary addition of a scalar
            //!
            //! \code
            //! array_type a = ...;
            //! array_type::value_type s = ...;
            //!
            //! a += s;
            //! \endcode
            //! 
            //! \param s the scalar value to add
            //! \return array reference
            //!
            array_type &operator+=(value_type s) 
            { 
                IPA::add(*this,s); 
                return *this;
            }

            //-----------------------------------------------------------------
            //!
            //! \brief unary addition of an array 
            //!
            //! \code
            //! array_type1 a = ...;
            //! array_tyep2 b = ...;
            //!
            //! a += b;
            //! \endcode
            //!
            //! \tparam ATYPE type of the array to add
            //! \param v reference to the array to add 
            //! \return reference to the original array
            //!
            template<typename ATYPE> 
            array_type &operator+=(const ATYPE &v) 
            { 
                IPA::add(*this,v); 
                return *this;
            }

            //-----------------------------------------------------------------
            //!
            //! \brief unary subtraction of a scalar
            //!
            //! \code
            //! array_type a = ...;
            //! array_type::value_type s = ...;
            //!
            //! a -= s;
            //! \endcode
            //! 
            //! \param s the scalar value to subtract
            //! \return array reference
            //!
            array_type &operator-=(value_type s) 
            { 
                IPA::sub(*this,s); 
                return *this;
            }

            //-----------------------------------------------------------------
            //!
            //! \brief unary subtraction of an array
            //!
            //! \code
            //! array_type1 a = ...;
            //! array_tyep2 b = ...;
            //!
            //! a -= b;
            //! \endcode
            //!
            //! \tparam ATYPE type of the array to subtract
            //! \param v reference to the array to subtract 
            //! \return reference to the original array
            //!
            template<typename ATYPE> 
            array_type &operator-=(const ATYPE &v) 
            { 
                IPA::sub(*this,v); 
                return *this; 
            }

            //-----------------------------------------------------------------
            //!
            //! \brief unary multiplication of a scalar
            //!
            //! \code
            //! array_type a = ...;
            //! array_type::value_type s = ...;
            //! 
            //! a *= s;
            //! \endcode
            //! 
            //! \param s the scalar value to multiply with
            //! \return array reference
            //!
            array_type &operator*=(value_type s) 
            { 
                IPA::mult(*this,s); 
                return *this; 
            }

            //-----------------------------------------------------------------
            //!
            //! \brief unary multiplication of an array
            //!
            //! \code
            //! array_type1 a = ...;
            //! array_tyep2 b = ...;
            //!
            //! a *= b;
            //! \endcode
            //!
            //! \tparam ATYPE type of the array to multiply 
            //! \param v reference to the array to multiply 
            //! \return reference to the original array
            //!
            template<typename ATYPE>
            array_type &operator*=(const ATYPE &v) 
            { 
                IPA::mult(*this,v); 
                return *this;
            }

            //-----------------------------------------------------------------
            //!
            //! \brief unary division of a scalar
            //!
            //! \code
            //! array_type a = ...;
            //! array_type::value_type s = ...;
            //!
            //! a /= s;
            //! \endcode
            //! 
            //! \param s the scalar value to divide by
            //! \return array reference
            //!
            array_type &operator/=(value_type s) 
            { 
                IPA::div(*this,s); 
                return *this;
            }

            //-----------------------------------------------------------------
            //!
            //! \brief unary division of an array
            //!
            //! \code
            //! array_type1 a = ...;
            //! array_tyep2 b = ...;
            //! 
            //! a /= b;
            //! \endcode
            //! 
            //! \tparam ATYPE type of the array to divide by  
            //! \param v reference to the array to divide by 
            //! \return reference to the original array
            //!
            template<typename ATYPE>
            array_type &operator/=(const ATYPE &v) 
            { 
                IPA::div(*this,v); 
                return *this;
            }


    };

    //set data for static member attribute
    template<typename STORAGE,typename IMAP,typename IPA>
    const type_id_t mdarray<STORAGE,IMAP,IPA>::type_id = 
    type_id_map<typename STORAGE::value_type>::type_id;
    //=====================non-member operators================================

    //!
    //! \ingroup mdim_array_classes
    //! \brief output operator
    //! 
    //! Writes content of a DArray to an output stream. 
    //! \param o output stream
    //! \param a array to output
    //! \return output stream
    //!
    template<
             typename STORAGE,
             typename IMAP,
             typename IPA
            >
    std::ostream &operator<<(std::ostream &o,const mdarray<STORAGE,IMAP,IPA> &a)
    {
        for(auto iter = a.begin();iter!=a.end();++iter)
            o<<*iter<<" ";

        return o;
    }

    //-------------------------------------------------------------------------
    //!
    //! \ingroup mdim_array_classes
    //! \brief input stream operator
    //!
    //! Read data from an input stream. 
    //! \param is input stream
    //! \param a array where to store the data
    //! \return reference to input stream
    //!
    template<
             typename STORAGE,
             typename IMAP,
             typename IPA
            >
    std::istream &operator>>(std::istream &is,mdarray<STORAGE,IMAP,IPA> &a)
    {
        for(auto iter=a.begin();iter!=a.end();++iter)
            is>>*iter;

        return is;
    }
   
    //-------------------------------------------------------------------------
    //! 
    //! \ingroup mdim_array_classes
    //! \brief equality comparison operator
    //! 
    //! Returns true if thwo arrays are equal. This is the case when all
    //! element stored in the arrays are equal. 
    //! 
    //! \param b1 array on the lhs of the comparison
    //! \param b2 array on the rhs of the comparison
    //! 
    //! \return true if all elements are equal, false otherwise
    //!
    template<
             typename STORAGE,
             typename IMAP,
             typename IPA
            >
    bool operator==(const mdarray<STORAGE,IMAP,IPA> &b1, 
                    const mdarray<STORAGE,IMAP,IPA> &b2) 
    {
        return std::equal(b1.begin(),b1.end(),b2.begin());
    }

    //-------------------------------------------------------------------------
    //! 
    //! \ingroup mdim_array_classes
    //! \brief inequality comparison operator
    //! 
    //! Returns true if thwo arrays are not equal. 
    //! 
    //! \param b1 array on the lhs of the comparison
    //! \param b2 array on the rhs of the comparison
    //! 
    //! \return true if not equal, false otherwise
    //!
    template<
             typename STORAGE,
             typename IMAP,
             typename IPA
            >
    bool operator!=(const mdarray<STORAGE,IMAP,IPA> &b1, 
                    const mdarray<STORAGE,IMAP,IPA> &b2) 
    {
        return !(b1==b2);
    }

    
}
}
