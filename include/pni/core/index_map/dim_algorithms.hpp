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
// Created on: Feb 27, 2014
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#pragma once

#include <iterator>
#include <algorithm>
#include "../container_utils.hpp"

namespace pni{
namespace core{

    //=========================================================================
    //
    // Algorithms used on dimension layout objects 
    // 
    //=========================================================================

    //! 
    //! \ingroup dim_layout_classes 
    //! \brief total elements 
    //!
    //! Returns the total number of elements that the layout spans. 
    //!
    template<typename DIMLT> size_t span(const DIMLT &layout)
    {

    }

    //-------------------------------------------------------------------------
    //!
    //! \ingroup index_mapping_classes
    //! \brief number of elements
    //!
    //! Returns the total number of elements that can be addressed with this
    //! dimension layout.
    //! This can simply be done by multiplying all the count values provided by
    //! the layout instance.
    /*!
     \f[  size = \prod_{i=1}^{r}c_i \f]
    */ 
    //! \tparam DIMLT dimension layout type
    //! \param layout const reference to the layout instance
    //! \return number of reachable elements
    //! 
    template<typename DIMLT> size_t size(const DIMLT &layout)
    {
        return std::accumulate(layout.counts().begin(),
                               layout.counts().end(),
                               size_t(1),
                               std::multiplies<size_t>()); 
    }
   
    //-------------------------------------------------------------------------
    //!
    //! \ingroup dim_layout_classes
    //! \brief number of dimensions
    //! 
    //! Get the number of dimensions of a memory layout. 
    //!
    //! \tparam DIMLT dimension layout type
    //! \param layout const reference to the layout instance
    //! \return number of dimensions
    //! 
    template<typename DIMLT> size_t rank(const DIMLT &layout) noexcept
    {
        return layout.counts().size();
    }

    //-------------------------------------------------------------------------
    //!
    //! \ingroup dim_layout_classes
    //! \brief check if compact
    //!
    //! Returns true if the memory layout is compact. This means that all stride
    //! values are 1.
    //! 
    //!
    template<typename DIMLT> bool is_compact(const DIMLT &layout)
    {
        typedef typename DIMLT::value_type value_type;

        return std::all_of(layout.stride().begin(),
                           layout.stride().end(),
                           [](const value_type &x){ return x==value_type(1);});
    }

    //-------------------------------------------------------------------------
    //!
    //! \ingroup dim_layout_classes
    //! \brief get effective rank 
    //!
    //! Returns the number of dimensions for which the count value is not one.
    //! Dimensions with a count value of one have typically no effect. 
    //!
    //! \tparam DIMLT dimension layout type
    //! \param layout const reference to the layout instance
    //! \return number of dimensions with count value != 1
    //!
    template<typename DIMLT> bool effective_rank(const DIMLT &layout)
    {
        typedef typename DIMLT::value_type value_type;
        return std::count_if(layout.counts().begin(),
                             layout.counts().end(),
                             [](const value_type &x)
                             { return x!=value_type(1);});
    }

    //-------------------------------------------------------------------------
    //!
    //! \ingroup dim_layout_classes
    //! \brief expand index 
    //! 
    //! In some cases an it is usefull to use a compressed index tuple where the
    //! number of indices is equal to the effective rank (makes code more
    //! readable). However, for alter calculations the full index is required. 
    //! This function expands an index to this full index set. 
    //! 
    //! \throws shape_mismatch_error if effective rank does not match
    //! 
    //! 
    template<typename DIMLT,
             typename EFFIT,
             typename FULLIT>
    void expand_index(const DIMLT &layout,const EFFIT &eindex,FULLIT &findex)
    {
        //check if the effective rank matches the rank of the original index
        if(effective_rank(layout) != eindex.size())
            throw shape_mismatch_error(EXCEPTION_RECORD,"");
        
        //take the code here from array selection

    }

    //-------------------------------------------------------------------------
    //!
    //! \ingroup dim_layout_classes
    //! \brief create effective shape
    //! 
    //! Returns the effective shape of a dimension layout. This is basically the
    //! counts field with all elements equal 1 stripped of. 
    //!
    //! \tparam STYPE shape container type
    //! \tparam DIMLT dimension layout type
    //! \param layout reference to the dimension layout instance
    //! \return instance of STYPE with the effective shape
    template<typename STYPE,typename DIMLT>
    STYPE effective_shape(const DIMLT &layout)
    {
        typedef typename DIMLT::value_type value_type;
        
        STYPE shape;
        std::copy_if(layout.counts.begin(),
                     layout.counts.end(),
                     std::back_inserter(shape),
                     [](const value_type &x){return x!=value_type(1);});
        return shape;
    }

//end of namespace
}
}