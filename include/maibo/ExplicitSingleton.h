// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// ExplicitSingleton is a class whose method instance doesn't create the object
// instead you need to explicitly create it with createInstance and destroy it 
// with destroyInstance
// usage:
// class SomeClass : public maibo::ExplicitSingleton<SomeClass>
// {
//      MAIBO_DECLARE_EXPLICIT_SINGLETON(SomeClass);
// public:
//      ...other stuff...

namespace maibo
{
    template <typename T>
    class ExplicitSingleton
    {
    public:
        static void createInstance()
        {
            assert(!m_instance);
            m_instance = new T();
        }

        static T& instance()
        {
            assert(m_instance);
            return *m_instance;
        }

        static void destroyInstance()
        {
            delete m_instance;
            m_instance = nullptr;
        }

    private:
        static T* m_instance;
    };

    template <typename T>
    T* ExplicitSingleton<T>::m_instance = nullptr;
}

#define MAIBO_DECLARE_EXPLICIT_SINGLETON(type) \
    private: \
        type(); \
        ~type(); \
        friend class ::maibo::ExplicitSingleton<type>
