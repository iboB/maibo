// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "maibo/Common/Future.h"

// The resource future is used in the asynchronous resource loading operations
// All of those will return a resource future of a given type
// and start an asynchronous task of obtaining the resource (or info)
// Using the resource before the future is done may sometimes be allowed
// It depends on the specific async job

namespace maibo
{
    template <typename Resource>
    class ResourceFuture : public Future
    {
    public:
        ResourceFuture()
            : m_resource()
        {}

        ResourceFuture(const Resource& resource)
            : m_resource(resource)
        {}

        ResourceFuture(Resource&& resource)
            : m_resource(resource)
        {}

        Resource& resource() { return m_resource; }
        const Resource& resource() const { return m_resource; }

    private:

        Resource m_resource;
    };

    template <typename Resource>
    using ResourceFuturePtr = std::shared_ptr<ResourceFuture<Resource>>;

    template <typename Resource>
    using ConstResourceFuturePtr = std::shared_ptr<const ResourceFuture<Resource>>;
}