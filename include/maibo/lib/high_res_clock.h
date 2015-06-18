//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <chrono>

namespace maibo
{
#if defined(_MSC_VER)

    // MSVC doesn't support a the high resolution timer from chrono
    // (well it does, but it's "high" resolution is about a millisecond)
    // Thus we're forced to use this silly class when we're on msvc

    struct high_res_clock
    {
        typedef long long rep;
        typedef std::nano period;
        typedef std::chrono::duration<rep, period> duration;
        typedef std::chrono::time_point<high_res_clock> time_point;
        static const bool is_steady = true;

        static time_point now();
    };
#elif defined(__EMSCRIPTEN__)

    // On the other hand Emscripten works very slowly (and isn't at all precise) with 64 bit integers
    // due to Javascript limitations.
    // Hence we'll define a high resolution clock with the best emcc has to offer
    
    struct high_res_clock
    {
        typedef double rep;
        typedef std::milli period;
        typedef std::chrono::duration<rep, period> duration;
        typedef std::chrono::time_point<high_res_clock> time_point;
        static const bool is_steady = true;

        static time_point now();
    };

#else
    typedef std::chrono::high_resolution_clock high_res_clock;
#endif
}