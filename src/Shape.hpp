/*
 * Declaration of class ArrayShape
 *
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
 * along with lipniutils.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 *
 * The ArrayShape object describes the shape of an array object. It provides
 * methods for computing the linear offset from a given index vector and
 * methods for computing such an index starting from a linear offset.
 *
 * Created on: Apr 19, 2011
 *     Author: Eugen Wintersberger
 *
 */

#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include <string>
#include <boost/shared_ptr.hpp>

#include "Exceptions.hpp"
#include "Index.hpp"
#include "Types.hpp"
#include "Buffer.hpp"


namespace pni{
namespace utils{

//! Shape object

//! ArrayShape objects describe the shape of an array. In particular
//! they are important to access array data stored in a linear,
//! contiguous stream of data. In all cases we assume C-ordering.
//! This means that the last index in the array varies fastest.
//! Each array is stored as a linear sequence of data values. To
//! mimic the behavior of a multidimensional array we need to map
//! each tuple of indices to a linear memory location. Consider an
//! array of \f$M\f$ dimension and \f$N_i\f$ elements along each dimension.
//! Furthermore we assume an multidimensional index for an array element
//! \f$(i_0,i_1,....,i_{(M-1)})\f$. From this the linear memory location can be
//! obtained with
//!\f[
//! 	o = \sum_{j=0}^{M-1} i_jS_j
//!\f]
//! where S_j is the stride of each dimension which is determined by
//!\f[
//! S_j = \prod_{k=j}^{M-2} N_{k+1}
//! with S_{M-1} = 1
//!\f]
//! After an ArrayShape object has been instantiated all its parameters
//! can be adjusted dynamically to whatever values are required.
//! The class takes care about all kinds of adoptions.
//! This might not be true for array classes using this ArrayShape type.

class Shape{
    private:
        Buffer<size_t> _dimstrides;  //!< the strides for the offset calculation
        Buffer<size_t> _shape;       //!< the number of values along each dimension
        size_t _size;         //!< the total number of elements in the array
        
        //! compute dimension strides

        //! compute the stride for each dimension - used internally to recompute
        //! the stride for each dimension if the shape is changed.
        void _compute_dimstrides();

        //! compute total number of elements

        //! used internally to recompute the number of elements in the array
        //! once the shape is changed in a way so that the size is changed.
        void _compute_size();
    public:
        typedef boost::shared_ptr<Shape> sptr;  //!< smart pointer to an ArrayShape object
        //! default constructor
        Shape();
        //! copy constructor

        //! Initialize an object of type ArrayShape with the content of an
        //! other ArrayShape object.
        //! \throws MemoryAllocationError if memory allocation fails
        Shape(const Shape &s);
        //! move constructor
        Shape(Shape &&s);
        //! constructor

        //! This constructor sets the rank of the shape object allowing it
        //! to allocate memory during creation.
        //! \throws MemoryAllocationError if memory allocation fails
        //! \param r rank of the shape (number of dimensions)
        Shape(const size_t &r);
        //! destructor
        virtual ~Shape();
        
        //! set the rank of the shape object

        //! If the new rank is not equal the all dimension information is
        //! lost and new memory is allocated. If the operation fails typically
        //! due to a problem in memory allocation the original object state
        //! remains untouched.
        //! \throws MemoryAllocationError in cases of memory allocation problems
        //! \param r number of dimensions (rank)
        virtual void rank(const size_t &r);
        //! return the array rank

        //! Returns the number of dimensions in the shape object.
        //! \return array rank
        virtual size_t rank() const;

        //! set a single dimension of the shape object

        //! Use this method to set a single dimension in an array to a new value.
        //! If the index i is larger than the rank of the shape an exception
        //! will be raised.
        //! \throws IndexError if i exceeds the rank of the shape
        //! \param i index of the dimension to set
        //! \param d new number of elements along dimension i
        virtual void dim(const size_t &i,const size_t &d);
        //! get a single dimension of the shape

        //! return the number of elements along dimension i. Raises an exception
        //! if i is larger than the rank of the shape.
        //! \throws IndexError if i exceeds the rank of the shape
        //! \param i index of the dimension
        //! \return the number of elements along dimension i
        virtual size_t dim(const size_t &i) const;
        //! total number of elements

        //! Returns the total number of elements that can be described by the
        //! shape.
        //! \return total number of elements
        virtual size_t size() const {return _size;}
        
        //! compute element offset

        //! This method computes the offset for the element in an array
        //! characterized by the indices in index. The offset is the address of
        //! the element in a linear buffer.
        //! \throws IndexError if one of the indices in the Index objects exceeds its dimension
        //! \param Index index object
        //! \return offset for an index
        virtual size_t offset(const Index &i) const;
        //! creates index from offset

        //! Creates an index object that belongs to a particular linear offset.
        //! The Index object that is passed to the method must be allocated (rank != 0)
        //! and have the same rank as the shape object - otherwise an
        //! exception will be thrown.
        //! \throws ShapeMissmatchError if Index and ArrayShape object have different rank
        //! \throws MemoryAccessError if Index of ArrayShape object have rank 0
        //! \param offset offset for which to compute the index
        //! \param i index object where to store the result
        virtual void index(const size_t &offset,Index &i) const;

        //the assignment operator must be a member function
        //and cannot be declared as a friend function
        //! assignment operator

        //! copy assignment operator
        Shape &operator=(const Shape &);
        //! move assignment operator
        Shape &operator=(Shape &&o);
        
        //! equality operator for array shapes

        //! It returns true if the rank and dimension of two arrays are equal, false otherwise
        //! \return boolean value representing shape equality
        friend bool operator==(const Shape &,const Shape &);
        //! inequality operator for array shapes

        //! Returns true if either the rank or one of the dimensions in two shape objects
        //! differ.
        //! \return boolean value
        friend bool operator!=(const Shape &,const Shape &);
        //! [] operator for read access

        //! This operator allows reading access to the array dimensions. You cannot
        //! use this operator to set array dimensions. The reason for this is, that
        //! once a single dimension has been changed we would have to recalculate
        //! strides and recompute the size. However, this cannot be done easily
        //! by operator overloading.
        //! \sa void setDimension(const unsigned int &i,const unsigned int &d)
        const size_t operator[](size_t i) const;

        //! operator for console output
        friend std::ostream &operator<<(std::ostream &o,const Shape &s);
};


}
}

#endif