// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "maibo/Task.h"
#include "ResourceFuture.h"

// Class used as a parent and shared functionality for resource-loading tasks

namespace maibo
{
    template <typename Resource, typename DependentResource>
    class ResourceTask : public Task
    {
    public:
        ResourceTask(ConstResourceFuturePtr<DependentResource> dependentFuture)
            : future(new ResourceFuture<Resource>)
            , m_dependentFuture(dependentFuture)
        {}

        ResourceFuturePtr<Resource> future;

        virtual bool execute() override
        {
            if (m_dependentFuture)
            {
                if (!m_dependentFuture->isDone())
                {
                    // if there is a getFile async task associated with this one, we must wait for it
                    return false;
                }

                int error = m_dependentFuture->errorCode();
                if (error != 0)
                {
                    future->setProgress(1.f);
                    future->setErrorCode(error);
                    future->setDone();
                    return true;
                }
            }

            // here either the dependent future is available or there was none
            return safeExecute();
        }

        // called after the dependent resource is available
        // without an error
        virtual bool safeExecute() = 0;

    protected:
        ConstResourceFuturePtr<DependentResource> m_dependentFuture;
    };
}
