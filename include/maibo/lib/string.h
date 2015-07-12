// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <cctype>

// useful string ops

namespace maibo
{
    namespace str
    {
        inline std::string& trim(std::string& str)
        {
            str.erase(str.begin(), find_if(str.begin(), str.end(),
                [](char& ch)->bool { return !std::isspace(ch); }));
            str.erase(find_if(str.rbegin(), str.rend(),
                [](char& ch)->bool { return !std::isspace(ch); }).base(), str.end());
            return str;
        }
    }
}