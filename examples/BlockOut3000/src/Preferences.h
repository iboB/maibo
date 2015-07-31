// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/lib/ImplicitSingleton.h>

// class that stores preferences, that the user may edit

class Preferences : public maibo::ImplicitSingleton<Preferences>
{
public:
    Preferences();

    const mathgp::vector4& layerColor(uint32_t layer) const;

private:

    std::vector<mathgp::vector4> m_levelLayerColors;
};