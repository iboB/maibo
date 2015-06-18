// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#include <maibo/ResourceManager.h>

using namespace maibo;
using namespace std;

#if defined(MAIBO_PLATFORM_DESKTOP)

int ResourceManager::GetFile(const string& path)
{
    auto f = fopen(path.c_str(), "rb");
    if (!f)
        return 1;

    fclose(f);
    return 0;
}

#elif defined(__EMSCRIPTEN__)
#endif
