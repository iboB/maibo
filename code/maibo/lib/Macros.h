// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// useful macros

#define MAIBO_PP_CAT(a, b) _MAIBO_INTERNAL_PP_CAT(a, b)
#define _MAIBO_INTERNAL_PP_CAT(a, b) a##b

// macros for macro lists
#define MAIBO_ENUM1(e) e,
#define MAIBO_ENUM2(e, v) e,
#define MAIBO_VALUE(e, v) v,
