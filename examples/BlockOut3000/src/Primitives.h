// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// defines geometric primitives

template <size_t N>
using Primitive = std::array<yama::point3, N>;

typedef Primitive<2> LineSegment;
typedef Primitive<3> Triangle;

// A class that makes primitives avaialbale as keys for sets of maps
// Warning! Using this class may be slow
// In order to compensate for possible primitive winding this class sorts the input primitives
// It doesn't change them, but it copies them to do the sorting.
struct PrimitiveStrictWeakOrdering
{
    template <size_t N>
    bool operator()(const std::array<yama::point3, N>& a, const std::array<yama::point3, N>& b) const
    {
        // cannot just compare triangles a-a b-b c-c, because trignales with different windings must match
        yama::strict_weak_ordering o;

        auto ca = a;
        std::sort(ca.begin(), ca.end(), o);
        auto cb = b;
        std::sort(cb.begin(), cb.end(), o);

        for(size_t i = 0; i < N; ++i)
        {
            if(o(ca.at(i), cb.at(i)))
                return true;
            else if(o(cb.at(i), ca.at(i)))
                return false;
        }

        return false;
    }
};