// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// ImplicitSingleton provides a static method instance
// the first call of instance will create the object
// usage: crtp: class SomeClass : public ImplicitSingleton<SomeClass>

namespace
{
    template <typename T>
    class ImplicitSingleton
    {
    public:
        static T& instance()
        {
            static T theInstance;
            return theInstance;
        }
    };
}
