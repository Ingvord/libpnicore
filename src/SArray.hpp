/*
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 * This file is part of libpniutils.
 *
 * libpniutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libpniutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpniutils.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 *
 * Static array template.
 *
 * Created on: 24.05.2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#ifndef __SARRAY_HPP__
#define __SARRAY_HPP__

#include "Types.hpp"
#include "SBuffer.hpp"
#include "StaticCIndexMap.hpp"
#include "Exceptions.hpp"
#include "Slice.hpp"
#include "ArrayView.hpp"
#include "ArrayViewSelector.hpp"

namespace pni{
namespace utils{

    /*! 
    \ingroup multidim_array_classes
    \brief static array 

    Static arrays are multidimensional arrays which can be used to represent
    arrays whose size is fixed at compile time. Thus, the memory requirement for
    such an array should be sufficiently small (basically not more than the
    buffer required to store the data). This is quite usefull in the case where
    many arrays (think about matrices) must be stored. In such a case the memory
    overhead due to administrative data structures can become critical. 

    Lets have a look on how we use such an array 
    \code

    //create a static array of shape 4x5x10 of type double
    SArray<double,4,5,10> array;

    //data access is as usuall with the () operator
    double v = array(1,4,8);


    //global assignment works too
    array = 10;

    
    \endcode

    \tparam T data type to be stored in the array
    \tparam DIMS list of template parameters each representing the number of
    elements along a particular dimension.
    */
    template<typename T,size_t ...DIMS> class SArray
    {
        private:
            //! static buffer holding the data
            SBuffer<T,SizeType<DIMS...>::size > _data;    
            //! static shape describing the arrays dimensionality
            static const StaticCIndexMap<DIMS...> _shape; 


            //===================private methods===============================
            /*!
            \brief get view

            Return a view of the array. 
            \tparam ITYPES index types
            \param view dummy variable 
            \param indices the indices for the view
            \return array view
            */
            template<typename ...ITYPES> 
                ArrayView<SArray<T,DIMS...> > 
                 _get_data(ArrayView<SArray<T,DIMS...> > &view,
                               ITYPES ...indices)
            {

                ArraySelection s =
                    ArraySelection::create(std::vector<Slice>{Slice(indices)...});

                return ArrayView<SArray<T,DIMS...> >(*this,s);
            }

            //-----------------------------------------------------------------
            /*!
            \brief get data reference

            Return a reference to a single data value of the array.
            \param v dummy argument
            \param indices multidimensional index of the value
            \return reference to that value
            */
            template<typename ...ITYPES> T& _get_data(T v,ITYPES ...indices)
            {
                 return this->_data[this->_shape.offset(indices...)];   
            }

            //-----------------------------------------------------------------
            /*!
            \brief get data value

            Return the value of a single array element determined by the index 
            indices.
            \tparam ITYPES index types
            \param v dummy argument to determine the return type
            \param indices indices determining the element
            \return value of the element
            */
            template<typename ...ITYPES> T _get_data(T v,ITYPES ...indices) const
            {
                return this->_data[this->_shape.offset(indices...)];
            }

            //-----------------------------------------------------------------
            /*!
            \brief get array selection

            Template member function returning an array selection. 
            \tparam CTYPE primary container template
            \tparam OTS optional arguments to the array template
            \param view array template 
            \param c container with indices
            \return array view object`
            */
            template<template<typename ...> class CTYPE,typename ...OTS>
            ArrayView<SArray<T,DIMS...> >
            _get_data(ArrayView<SArray<T,DIMS...> > &view,const CTYPE<OTS...> &c)
            {
                ArraySelection s = ArraySelection::create(c);

                return ArrayView<SArray<T,DIMS...> >(*this,s);

            }

            //----------------------------------------------------------------
            /*! 
            \brief get data element

            Return a reference to a particular data element of the array. 
            \tparam CTYPE primary container template
            \tparam OTS optional template arguments
            \param v dummy argument to determine the return type
            \param c container with indices
            \return reference to the element
            */
            template<template<typename ...> class CTYPE,typename ...OTS>
            T &_get_data(T v,const CTYPE<OTS...> &c)
            {
                 return this->_data[this->_shape.offset(c)];   
            }

            //---------------------------------------------------------------
            /*!
            \brief get data value

            Reutrn the value of the data element indexed by the index container 
            c. 
            \tparam CTYPE container template
            \tparam OTS optional template arguments
            \param v dummy argument to determine the return type
            \param c container with indices
            \return value of the data element
            */
            template<template<typename ...> class CTYPE,typename ...OTS>
            T _get_data(T v,const CTYPE<OTS...> &c) const
            {
                return this->_data[this->_shape.offset(c)];
            }

        public:
            //================public types=====================================
            //! data type of the elements stored in the array
            typedef T value_type; 
            //! type of the array
            typedef SArray<T,DIMS...> array_type;
            //! type of the arrya view
            typedef ArrayView<array_type> view_type;
            //! storage type
            typedef SBuffer<T,SizeType<DIMS...>::size > storage_type;
            //! shared pointer to this type
            typedef std::shared_ptr<array_type> shared_ptr;
            //! unique pointer to this type
            typedef std::unique_ptr<array_type> unique_ptr;
            //! iterator
            typedef typename storage_type::iterator iterator; 
            //! const iterator
            typedef typename storage_type::const_iterator const_iterator; 
            //! type of the index map
            typedef StaticCIndexMap<DIMS...> map_type;
            //===============public members====================================
            //! ID of the datatype stored in the array
            static const TypeID type_id = TypeIDMap<value_type>::type_id;
            

            //============================constructor and destructor===========
            //! default constructor
            SArray() {} 

            //-----------------------------------------------------------------
            /*!
            \brief construct from an ArrayView object

            \throws SizeMissmatchError if sizes do not match
            \throws ShapeMissmatchError if shapes do not match
            \tparam ATYPE array type of the view
            \param view reference to the view object
            */
            template<typename ATYPE> SArray(const ArrayView<ATYPE> &view)
            {
                check_equal_size(view,*this,EXCEPTION_RECORD);
                check_equal_shape(view,*this,EXCEPTION_RECORD);

                std::copy(view.begin(),view.end(),this->begin()); 
            }

            //-----------------------------------------------------------------
            //! copy constructor
            explicit SArray(const array_type &a):
                _data(a._data)
            {}

            //-----------------------------------------------------------------
            //! construct from a container type
            template<template<typename ...> class CTYPE,typename ...OTS>
                SArray(const CTYPE<OTS...> &c)
            {
                //can check here only for size
                check_equal_size(c,*this,EXCEPTION_RECORD);

                std::copy(c.begin(),c.end(),this->begin());
            }

            //-----------------------------------------------------------------
            //! construct from a different array type
            template<typename AT> SArray(const AT &a)
            {
                //can check here for size and shape
                check_equal_size(a,*this,EXCEPTION_RECORD);
                check_equal_shape(a,*this,EXCEPTION_RECORD);

                std::copy(a.begin(),a.end(),this->begin());
            }

            //-----------------------------------------------------------------
            //! construction from an initializer list
            SArray(const std::initializer_list<T> &il)
            {
                check_equal_size(il,*this,EXCEPTION_RECORD);

                size_t index = 0;
#ifdef NOFOREACH
                for(auto iter = il.begin(); iter!=il.end();++iter)
                {
                    auto v = *iter;
#else
                for(auto v: il)
                {
#endif 
                    (*this)[index++] = v;
                }
            }

            //-----------------------------------------------------------------
            //! destructor
            ~SArray() {}


            //==============assignment operators===============================
            //!copy assignment operator for a StaticArray
            array_type &operator=(const array_type &a)
            {
                if(this == &a) return *this;
                this->_data = a._data;
                return *this;
            }
                

            //============operators and methods for data access================
            /*! 
            \brief access data element 

            Method returning a reference to an array element identified by a 
            multidimensional array index which is passed to the method as an
            argument list. As the method returns a reference to the element read
            write access is provided to the element.
            \code
            StaticArray<Float64,3,4> array;
            Float64 value = array(1,3);
            array(1,3) = 3.4;
            \endcode
            This method template throws a compile time error if the number of
            indices does not match the rank of the array.
            \tparam ITYPES index types
            \param indices list of array indices
            \return reference to the array element
            */
            template<typename ...ITYPES>
                typename ArrayViewSelector<array_type,ITYPES...>::reftype
                operator()(ITYPES ...indices)
            {

                static_assert((sizeof...(indices))==(sizeof...(DIMS)),
                        "Number of indices does not match array rank!");

                typedef ArrayViewSelector<array_type,ITYPES...> selector;
                typedef typename selector::viewtype viewtype;
                typedef typename selector::reftype  viewref;

                viewtype r = viewtype();

                return _get_data(r,indices...);
                //return this->_data[this->_shape.offset(indices...)];
            }

            //-----------------------------------------------------------------
            /*!
            \brief return data value

            Method returning the value of an array element (read only data
            access). The element is identified by its multidimensional index
            passed to the method as an argument list. 
            \code
            StaticArray<Float64,3,4> array;
            Float64 value = array(1,3);
            \endcode
            As the method returns the value of the element it cannot be used for
            write access.
            This template method throws a compile time error if the number of
            indices does not match the rank of the array.
            \tparam ITYPES index types
            \param indices element index
            \return value of the array element
            */
            template<typename ...ITYPES> 
                typename ArrayViewSelector<array_type, ITYPES...>::viewtype 
                operator()(ITYPES ...indices) const
            {
                static_assert((sizeof...(indices))==(sizeof...(DIMS)),
                        "Number of indices does not match array rank!");

                typedef typename ArrayViewSelector<array_type,ITYPES...>::viewtype
                    result_type;

                return _get_data(result_type(),indices...);
                //return result;
            }

            //-----------------------------------------------------------------
            /*! 
            \brief get element reference

            Returns a reference to an element determined by its multidimensional
            index. The indices are stored in a container of type CTYPE. Every
            container providing the [] operators for data access and a forward
            iterator are allowed. 
            \throws ShapeMissmatchError if container size and array rank do not
            match
            \tparam CTYPE container type
            \param c container with indices
            \return reference to the element
            */
            template<template<typename ...> class CTYPE,typename ...OTS>
            typename ArrayViewSelector<array_type,typename CTYPE<OTS...>::value_type>::reftype
            operator()(const CTYPE<OTS...> &c)
            {

                typedef ArrayViewSelector<array_type,typename CTYPE<OTS...>::value_type> selector;
                typedef typename selector::viewtype viewtype;
                typedef typename selector::reftype  viewref;

                viewtype r = viewtype();
                return _get_data(r,c);
            }

            //-----------------------------------------------------------------
            /*! 
            \brief get element value

            Return the value of an array element determined by its
            multidimensional index. The index is provided as a container of type
            CTYPE. Every container type implementing the [] operator and an STL
            compliant forward iterator is allowed. 
            \throws ShapeMissmatchError if container size and array rank do not
            match
            \tparam CTYPE container type
            \param c container with indices
            \return value of the array element
            */
            template<template<typename ...> class CTYPE,typename ...OTS>
            typename ArrayViewSelector<array_type,typename CTYPE<OTS...>::value_type>::viewtype
            operator()(const CTYPE<OTS...> &c) const
            {
                typedef ArrayViewSelector<array_type,typename CTYPE<OTS...>::value_type> sel;
                typename sel::viewtype result;

                return _get_data(result,c);
                return result;
            }


            //-----------------------------------------------------------------
            /*!
            \brief reference to element
    
            Return a reference to an array element determined by its linear
            index i. This operator does not perform any index checking. Thus, if
            the index exceeds the size of the array the program most probably
            will produce a segmentation violation.
            \param i linear index of the element
            \return reference to element
            */
            value_type &operator[](size_t i) { return this->_data[i]; }

            //-----------------------------------------------------------------
            /*! 
            \brief element value

            Return the value of an array element determined by its linear index
            i. No index checking is performed.
            \param i linear index of the element
            \return element value
            */
            value_type operator[](size_t i) const { return this->_data[i]; }

            //-----------------------------------------------------------------
            /*! 
            \brief get element value

            Get the value of the element with linear index i. An exception is
            thrown if the index exceeds the size of the array.
            \throws IndexError if i exceeds array size
            \param i linear index of the value
            \return value of array element
            */
            value_type at(size_t i) const
            {
                check_index(i,this->size(),EXCEPTION_RECORD);

                return (*this)[i];
            }

            //-----------------------------------------------------------------
            /*! 
            \brief get element reference

            Return the reference to the element at linear index i. If i exceeds
            the size of the array an exception will be thrown.
            \throws IndexError if exceeds the size of the array
            \param i linear index of the element
            \return reference to the requested element
            */
            value_type &at(size_t i) 
            {
                check_index(i,this->size(),EXCEPTION_RECORD);

                return (*this)[i];
            }

            //-----------------------------------------------------------------
            /*! 
            \brief insert value

            Insert a data value at linear position i. Method throws an exception
            if i exceeds the size of the array.
            \throws IndexError if i exceeds the arrays size
            \param i index where to insert the value
            \param v value to insert
            */
            void insert(size_t i,const value_type &v)
            {
                check_index(i,this->size(),EXCEPTION_RECORD);

                (*this)[i] = v;
            }

            //-----------------------------------------------------------------
            //! get iterator to first element
            iterator begin() { return this->_data.begin(); } 
            
            //-----------------------------------------------------------------
            //! get iterator to last element
            iterator end()   { return this->_data.end(); }

            //-----------------------------------------------------------------
            //! get const iterator to first element
            const_iterator begin() const { return this->_data.begin(); }

            //-----------------------------------------------------------------
            //! get const iterator to last element
            const_iterator end() const   { return this->_data.end(); }

            //=======================inquery methods===========================
            //! get number of elements
            size_t size() const { return this->_shape.size(); }

            //-----------------------------------------------------------------
            //! get number of dimensions
            size_t rank() const { return this->_shape.rank(); }
           
            //-----------------------------------------------------------------
            /*!
            \brief get array shape

            Returns the number of elements a long each dimension in a container. 
            The container must implement the [] operator for data access as well
            as a simple forward iterator.
            \code
            StaticArray<UInt16,100,100> array;
            auto shape = array.shape<std::vector<size_t> >();
            \endcode
            \tparam CTYPE container type
            \return instance of CTYPE with shape information
            */
            template<typename CTYPE> CTYPE shape() const
            {
                return this->_shape.shape<CTYPE>();
            }

            /*!
            \brief get index map

            Returns a const reference to the index map fo the array type.
            \return index map
            */
            const map_type &map() const { return this->_shape; }

            //-----------------------------------------------------------------
            /*!
            \brief get reference to storage

            Return a constant reference to the storage of the array. 
            \return reference to static buffer
            */
            const storage_type &storage() const
            {
                return _data;
            }

    };

    template<typename T,size_t ...DIMS> const
        StaticCIndexMap<DIMS...> SArray<T,DIMS...>::_shape = StaticCIndexMap<DIMS...>();
//end of namespace
}
}

#endif
