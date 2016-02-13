// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "TaskManager.h"
#include "Task.h"

using namespace maibo;

TaskManager::TaskManager()
{
    m_taskIterator = m_tasks.begin();
}

TaskManager::~TaskManager()
{
    for (auto task : m_tasks)
    {
        delete task;
    }
}

void TaskManager::update()
{
    for (unsigned i = 0; i < m_numTasksPerUpdate; ++i)
    {
        if (m_tasks.empty())
            break;

        auto task = *m_taskIterator;

        if (task->execute())
        {
            delete task;
            m_taskIterator = m_tasks.erase(m_taskIterator);
        }
        else
        {
            ++m_taskIterator;
        }

        if (m_taskIterator == m_tasks.end())
        {
            m_taskIterator = m_tasks.begin();
        }
    }
}

void TaskManager::pushTask(Task* t)
{
    m_tasks.push_back(t);

    if (m_taskIterator == m_tasks.end())
    {
        m_taskIterator = m_tasks.begin();
    }
}
