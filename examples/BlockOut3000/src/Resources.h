// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Common/ExplicitSingleton.h>
#include "UniformColorMaterial.h"

// class that contains all resources for the game

class Resources : public maibo::ExplicitSingleton<Resources>
{
    MAIBO_DECLARE_EXPLICIT_SINGLETON(Resources);

public:
    UniformColorMaterial uniformColorMaterial;
};
