// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/lib/ImplicitSingleton.h>
#include <maibo/lib/simplified_rng.h>

class Random : public maibo::ImplicitSingleton<Random>
{
public:
    // random generator for a single game
    // seeded at the start of each one
    // (could potentially be used for replaying a game with the same figure order)
    maibo::simplified_rng<std::minstd_rand> InGameRnd;
};

inline uint32_t InGameRndU32()
{
    return Random::instance().InGameRnd.rndu32();
}
