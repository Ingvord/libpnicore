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

    
    template<typename ATYPE>  class array_view;
    /*!
    \ingroup numeric_array_classes
    \brief Multiplication expression template

    */
    template<typename OP1T,typename OP2T> class mult
    {
        private:
            //! left operand
            typename op_trait<OP1T>::ref_type _op1;
            //! right operand
            typename op_trait<OP2T>::ref_type _op2;
        public:
            //--------------------public types---------------------------------
            //! value type of the multiplication
            typedef typename OP1T::value_type value_type;
            //! type of the operation class
            typedef mult<OP1T,OP2T> array_type;
            //! storage type
            typedef void storage_type;
            //! non-const iterator type - this is only to satsify the interaface
            typedef container_iterator<array_type,0> iterator; 
            //! const iterator type
            typedef container_iterator<array_type,1> const_iterator;
            //! view type
            typedef array_view<array_type> view_type;

            //===================constructors==================================
            /*! 
            \brief constructor

            Set up the operator class.
            \param o1 left operand
            \param o2 right operand
            */
            mult(const OP1T &o1,const OP2T &o2):
                _op1(o1),
                _op2(o2)
            {}

            //====================public methods===============================
            /*! 
            \brief get value at index i

            Return the value of _op1[i]*_op2[i];
            \param i index for which to reqest data
            \return result of operation
            */
            value_type operator[](size_t i) const
            {
                return this->_op1[i]*this->_op2[i];
            }

            //-----------------------------------------------------------------
            /*! 
            \brief return size of the operator

            This is the maximum size of the two operands.
            \return size of the operation
            */
            size_t size() const
            {
                return _op1.size()>_op2.size() ? _op1.size() : _op2.size();
            }

            //=======================inquery methods===========================
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
            //! get iterator on first element
            const_iterator begin() const
            {
                return const_iterator(this,0);
            }

            //-----------------------------------------------------------------
            //! get iterator on last+1 element
            const_iterator end() const
            {
                return const_iterator(this,this->size());
            }

    };



//end of namespace
}
}