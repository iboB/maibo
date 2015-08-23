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

    // time for the figure to fall a unit when dropped
    int figureDropTime() const { return m_figureDropTime; }

    const mathgp::vector4& layerColor(uint32_t layer) const;

private:
    int m_figureDropTime;

    std::vector<mathgp::vector4> m_levelLayerColors;
};