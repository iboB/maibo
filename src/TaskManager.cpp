// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/TaskManager.h>
#include <maibo/Task.h>

using namespace maibo;

TaskManager::~TaskManager()
{
    while (!m_tasks.empty())
    {
        delete m_tasks.front();
        m_tasks.pop();
    }
}

void TaskManager::update()
{
    for (unsigned i = 0; i < m_numTasksPerUpdate; ++i)
    {
        if (m_tasks.empty())
            break;

        if (m_tasks.front()->Execute())
        {
            delete m_tasks.front();
            m_tasks.pop();
        }
    }
}

void TaskManager::pushTask(Task* t)
{
    m_tasks.push(t);
}
