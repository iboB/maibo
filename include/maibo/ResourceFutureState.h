// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "AppState.h"

#include "ResourceFuture.h"

// base class for states that load a bunch of resources

namespace maibo
{
    class ResourceFutureState : public AppState
    {
    public:
        virtual void update(uint32_t dt) override;

        // gets the progress of pending futures from 0 to one
        // this function uses the doneFutures size
        // if you remove futures from m_doneFutures, the progress will fluctuate
        float calculateProgress() const;

    protected:
        // called when all futures have been loaded
        virtual void onDone() = 0;

        // call this to add the future to the pending list
        void addResourceFuture(ConstResourceFutureBasePtr future);

        typedef std::vector<ConstResourceFutureBasePtr> ResourceFutureBaseVector;

        // futures that are done
        ResourceFutureBaseVector m_doneFutures;

    private:
        // futures still being loaded
        ResourceFutureBaseVector m_pendingFutures;
    };
}
