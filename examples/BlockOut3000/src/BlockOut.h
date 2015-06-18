// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/Application.h>

// main application class

class BlockOut : public maibo::Application
{
public:
    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual void update() override;
};
