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
 *  Created on: Jan 21, 2013
 *      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */
#pragma once

#include <sstream>
#include <pni/core/Types.hpp>
#include <pni/core/TypeInfo.hpp>

using namespace pni::core;

template<typename T,bool is_int> struct uniform_distribution_map;

template<typename T> struct uniform_distribution_map<T,true>
{
    typedef std::uniform_int_distribution<T> distribution_type;
};

template<typename T> struct uniform_distribution_map<T,false>
{
    typedef std::uniform_real_distribution<T> distribution_type;
};

template<typename T> class uniform_distribution
{
    private:
        std::mt19937_64 _engine;
        typename uniform_distribution_map<T,TypeInfo<T>::is_integer>::distribution_type _distribution;
    public:
        uniform_distribution():
            _engine(),
            _distribution(0.2*TypeInfo<T>::min(),0.2*TypeInfo<T>::max())
        { }

        T operator()() 
        {
            return _distribution(_engine);
        }
};

template<typename T> class uniform_distribution<std::complex<T> >
{
    private:
        std::mt19937_64 _engine;
        typename uniform_distribution_map<T,TypeInfo<T>::is_integer>::distribution_type _distribution;
    public:
        uniform_distribution():
            _engine(),
            _distribution(0.2*TypeInfo<Float32>::min(),0.2*TypeInfo<Float32>::max())
        {}
        
        std::complex<T> operator()()
        {
            return std::complex<T>(_distribution(_engine),
                                   _distribution(_engine));
        }
};

template<> class uniform_distribution<Bool>
{
    private:
        std::mt19937_64 _engine;
        std::uniform_int_distribution<> _distribution;
    public:
        uniform_distribution():
            _engine(),
            _distribution(0,1)
        {}

        Bool operator()()
        {
            return _distribution(_engine);
        }
};

template<> class uniform_distribution<Binary>
{
    private:
        std::mt19937_64 _engine;
        std::uniform_int_distribution<UInt8> _distribution;
    public:
        uniform_distribution():
            _engine(),
            _distribution(0,255)
        {}

        Binary operator()() 
        {
            return _distribution(_engine);
        }
};

template<> class uniform_distribution<String>
{
    private:
        uniform_distribution<unsigned long> _distribution;
    public:
        uniform_distribution(){}

        String operator()()
        {
            std::stringstream ss;
            ss<<_distribution();
            return ss.str();
        }
};
