// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Optional class to use with signals
// You can iherit from this class and when your object is destroyed
// It will automatically disconnect from all signals

namespace maibo
{
    namespace internal
    {
        struct AutoSlotConnection
        {
            virtual ~AutoSlotConnection() {}
        };
    }

    class AutoSlot
    {
    protected:
        ~AutoSlot() {}

    private:
        template <typename...>
        friend class Signal;

        std::vector<std::unique_ptr<internal::AutoSlotConnection>> m_connections;
    };
}
