// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Used in some tasks to indicate progress

namespace maibo
{
    class Future
    {
    public:
        Future() {}
        Future(const Future&) = delete;
        Future(Future&&) = delete;
        virtual ~Future() {}

        // Is the looading job done
        // (could be either because the resource is loaded or because of an error)
        bool isDone() const { return m_isDone; }

        // Between 0 and 1
        float progress() const { return m_progress; }

        // 0 = no error
        int errorCode() const { return m_errorCode; }

        // The following are supposed to be called by the various resource tasks
        // Calling them from the outside may lead to undefined behaviour
        void setDone() { m_isDone = true; }
        void setProgress(float progress) { m_progress = progress; }
        void setErrorCode(int code) { m_errorCode = code; }

    private:

        bool m_isDone = false;
        float m_progress = 0;
        int m_errorCode = 0;
    };

    typedef std::shared_ptr<Future> FuturePtr;
    typedef std::shared_ptr<const Future> ConstFuturePtr;
}
