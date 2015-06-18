// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// The resource future is used in the asynchronous resource loading operations
// All of those will return a resource future of a given type
// and start an asynchronous task of obtaining the resource (or info)
// Using the resource before the future is done may sometimes be allowed
// It depends on the specific async job

namespace maibo
{
    template <typename Resource>
    class ResourceFuture
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

        ResourceFuture(const ResourceFuture&) = default;
        ResourceFuture(ResourceFuture&&) = default;

        // Is the looading job done 
        // (could be either because the resource is loaded or because of an error)
        bool isDone() const { return m_isDone; }

        // Between 0 and 1
        float progress() const { return m_progress; }

        // 0 = no error
        int errorCode() const { return m_errorCode; }

        Resource& resource() { return m_resource; }

        // The following are supposed to be called by the various resource tasks
        // Calling them from the outside may lead to undefined behaviour
        void setDone() { m_isDone = true; }
        void setProgress(float progress) { m_progress = progress; }
        void setErrorCode(int code) { m_errorCode = code; }

    private:
        bool m_isDone = false;
        float m_progress = 0;
        int m_errorCode = 0;

        Resource m_resource;
    };

    template <typename Resource>
    using ResourceFuturePtr = std::shared_ptr<ResourceFuture<Resource>>;

    template <typename Resource>
    using ConstResourceFuturePtr = std::shared_ptr<const ResourceFuture<Resource>>;
}