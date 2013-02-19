/*
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
 * Created on: Jul 25,2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */
#pragma once

#include "op_traits.hpp"

#include "../container_iterator.hpp"

namespace pni{
namespace core{

    template<typename ATYPE> class array_view;

    /*!
    \ingroup numeric_array_types
    \brief Addition expression template
    
    \tparam OP1T type of left operand
    \tparam OP2T type of right operand
    */
    template<typename OP1T,typename OP2T> class add
    {
        private:
            //! reference to the left operand
            typename op_trait<OP1T>::ref_type _op1;
            //! reference to the right operand
            typename op_trait<OP2T>::ref_type _op2;
        public:
            //--------------------public types---------------------------------
            //! result type of the operation
            typedef typename OP1T::value_type value_type;
            //! type of the expression template
            typedef add<OP1T,OP2T> array_type;
            //! storage type
            typedef void storage_type;
            //! non-const iterator type - just for interface
            typedef container_iterator<array_type,0> iterator;
            //! const iterator type
            typedef container_iterator<array_type,1> const_iterator;
            //! view type
            typedef array_view<array_type> view_type;

            //===================constructors==================================
            /*!
            \brief constructor
            \param o1 left operand
            \param o2 right operand
            */
            add(const OP1T &o1,const OP2T &o2):
                _op1(o1),
                _op2(o2)
            {}

            //====================public methods===============================
            /*!
            \brief get result at i

            Return the result of a[i]+b[i]. 
            \param i index at which to perform the operation
            \return result of the operation
            */
            value_type operator[](size_t i) const
            {
                return this->_op1[i]+this->_op2[i];
            }

            //-----------------------------------------------------------------
            /*! 
            \brief get size

            Return the maximum of a.size() and b.size().
            \return number of elements of result
            */
            size_t size() const
            {
                return _op1.size()>_op2.size() ? _op1.size() : _op2.size();
            }

            /*! 
            \brief get the shape

            Return the shape of the expression template. This both operands are
            instances of NumArray we can return the shape of one of the arrys. 
            If both are scalars we return the shape of the scalar. If one is
            scalar and one is an array the array type wins.
            \return shape of the operation
            */
            template<typename CTYPE> CTYPE shape() const
            {

                //_op1  is a scalar
                if(_op1.rank()==0)
                {
                    //if _op2 is an array 
                    if(_op2.rank() != 0) return _op2.template shape<CTYPE>();
                }
                //in all other cases we do not care what type _op2 is 
                //we just return the shape of _op1
                return  _op1.template shape<CTYPE>();
            }
            //=====================iterators===================================
            //! get const iterator to first element
            const_iterator begin() const
            {
                return const_iterator(this,0);
            }

            //-----------------------------------------------------------------
            //! get const iterator to last+1 element
            const_iterator end() const
            {
                return const_iterator(this,this->size());
            }

    };


//end of namespace
}
}