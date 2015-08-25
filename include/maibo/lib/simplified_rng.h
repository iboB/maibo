// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <random>

// simplified random number generator class (based on a template parameter an actual random number generator)
// used where the need for a random number is in a basic case, to avoid writing lots of code
// for the most common distributions

namespace maibo
{
    template <typename Rng>
    class simplified_rng
    {
    public:

        uint32_t rndu32()
        {
            return m_uint32Dist(rng);
        }

        uint32_t rndu32(uint32_t a, uint32_t b)
        {
            std::uniform_int_distribution<uint32_t> dist(a, b);
            return dist(rng);
        }

        float rndf()
        {
            return m_floatDist(rng);
        }

        Rng rng;

    private:
        std::uniform_int_distribution<uint32_t> m_uint32Dist;
        std::uniform_real_distribution<float> m_floatDist;
    };
}