// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// some basic additional functions concerning memory management

namespace maibo
{
    template <typename T>
    void safe_delete(T*& t)
    {
        delete t;
        t = nullptr;
    }

    // the size of the data within a std::vector
    template <typename T>
    size_t data_size(const std::vector<T>& vec)
    {
        return sizeof(T)*vec.size();
    }

    // dynamic cast on debug builds to catch wrong casts
    template <typename Target, typename Source>
    Target debug_cast(Source s)
    {
    #if defined(NDEBUG)
        return static_cast<Target>(s);
    #else
        return dynamic_cast<Target>(s);
    #endif
    }

    // fill memory with zero
    inline void zero_memory(void* mem, size_t size)
    {
        ::memset(mem, 0, size);
    }
}

#define OFFSET(i) ((char *)nullptr + (i))
#define OFFSET_OF(type, member) (&reinterpret_cast<type*>(nullptr)->member)

#if !defined(_MSC_VER)
#    define _countof(x) (sizeof(x)/sizeof(x[0]));
#endif
