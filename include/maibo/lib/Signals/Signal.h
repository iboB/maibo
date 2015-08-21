// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "AutoSlot.h"

// Simple fast signal
// Create a signal: Signal<int, float, string> sig;
// Connect to a signal: sig.connect<MyClass, &MyClass::IntFloatStringMethod>(myClassPtr)
// Disconnect from a signal:
//      sig.disconnect<MyClass, &MyClass::IntFloatStringMethod>(myClassPtr);
//      or
//      sig.disconnectAll(myClassPtr);
//      or
//      inherit from AutoSlot to have all your connections disconnected when the instance is destroyed
// Emit a signal: sig.emit(0, 3.14159f, "foo");

namespace maibo
{
    namespace internal
    {
        template <typename Slot, typename Signal>
        class AutoSlotConnectionInstance : public AutoSlotConnection
        {
        public:
            AutoSlotConnectionInstance(Slot* slot, Signal* signal)
                : m_slot(slot)
                , m_signal(signal)
            {}

            ~AutoSlotConnectionInstance()
            {
                m_signal->disconnectAll(m_slot);
            }

        private:
            Slot* m_slot;
            Signal* m_signal;
        };
    }

    template <typename... Args>
    class Signal
    {
    public:
        // connect a slot to a signal by an instance and method
        template <typename Slot, void (Slot::*Method)(Args...)>
        void connect(Slot* s, int priority = 0)
        {
            checkAutoSlotConnect(s, s);

            PrioritySlot pslot;
            pslot.priority = priority;
            pslot.slot = s;
            pslot.func = &Signal::methodProxy<Slot, Method>;

            insertSlot(pslot);
        }

        // disconnect a slot from a signal by an instance and method
        template <typename Slot, void (Slot::*Method)(Args...)>
        void disconnect(Slot* s)
        {
            // no need to check autoslot disconnect
            // wors case we'll disconnect something that isn't connected
            // which is just a loop to check through all connections.. nothing serious

            PrioritySlot pslot;
            pslot.priority = 0;
            pslot.slot = s;
            pslot.func = &Signal::methodProxy<Slot, Method>;

            removeSlot(pslot);
        }

        //template <typename Slot>
        //void connect(const Slot* s, void (Slot::*method)(Args...) const);

        // connect a slot to a signal by an instance and an external function
        template <typename Slot, void(*Func)(Slot*, Args...)>
        void connect(Slot* s, int priority = 0)
        {
            checkAutoSlotConnect(s, s);

            PrioritySlot pslot;
            pslot.priority = priority;
            pslot.slot = s;
            pslot.func = &Signal::externalProxy<Slot, Func>;

            insertSlot(pslot);
        }

        // disconnect a slot from a signal by an instance and an external function
        template <typename Slot, void(*Func)(Slot*, Args...)>
        void disconnect(Slot* s)
        {
            // no need to check autoslot disconnect
            // wors case we'll disconnect something that isn't connected
            // which is just a loop to check through all connections.. nothing serious

            PrioritySlot pslot;
            pslot.priority = 0;
            pslot.slot = s;
            pslot.func = &Signal::externalProxy<Slot, Func>;

            removeSlot(pslot);
        }

        // disconnect all connected functions for a slot instance
        template <typename Slot>
        void disconnectAll(Slot* s)
        {
            m_connectedSlots.erase(
                std::remove_if(m_connectedSlots.begin(), m_connectedSlots.end(),
                    [s](const PrioritySlot& elem) {
                        return s == elem.slot;
                    }),
                m_connectedSlots.end());
        }

        //template <typename Slot>
        //void connect(const Slot* s, void(*func)(const Slot*, Args...));

        // emit a signal
        void emit(Args&& ...args)
        {
            for (auto& ps : m_connectedSlots)
            {
                ps.func(ps.slot, std::forward<Args>(args)...);
            }
        }

    private:
        template <typename Slot, void(Slot::*Method)(Args...)>
        static void methodProxy(void* s, Args&& ...args)
        {
            Slot* slot = reinterpret_cast<Slot*>(s);
            (slot->*Method)(std::forward<Args>(args)...);
        }

        template <typename Slot, void(*Func)(Slot*, Args...)>
        static void externalProxy(void* s, Args&&... args)
        {
            Slot* slot = reinterpret_cast<Slot*>(s);
            Func(slot, std::forward<Args>(args)...);
        }

        typedef void(*SlotFunc)(void*, Args&&...);

        struct PrioritySlot
        {
            int priority;
            void* slot;
            SlotFunc func;
        };

        struct SmallerSlot
        {
            bool operator()(const PrioritySlot& a, const PrioritySlot& b)
            {
                return a.priority < b.priority;
            }
        };

        void insertSlot(const PrioritySlot& ps)
        {
            auto pos = std::lower_bound(m_connectedSlots.begin(), m_connectedSlots.end(), ps, SmallerSlot());
            m_connectedSlots.insert(pos, ps);
        }

        void removeSlot(const PrioritySlot& ps)
        {
            // erasing all intances of this slot disregarding priority
            m_connectedSlots.erase(
                std::remove_if(m_connectedSlots.begin(), m_connectedSlots.end(),
                    [&ps](const PrioritySlot& elem) {
                        return ps.slot == elem.slot && ps.func == elem.func;
                    }),
                m_connectedSlots.end());
        }

        template <typename Slot>
        void checkAutoSlotConnect(Slot*, void*) {}

        template <typename Slot>
        void checkAutoSlotConnect(Slot* slot, AutoSlot* as)
        {
            as->m_connections.emplace_back(new internal::AutoSlotConnectionInstance<Slot, Signal<Args...>>(slot, this));
        }

        std::vector<PrioritySlot> m_connectedSlots;
    };
}
