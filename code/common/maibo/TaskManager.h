// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Async task manager singleton
// Since the application is single threaded, the taks will be executed
// in the same thread in Application::Update

#include "Common/ImplicitSingleton.h"

namespace maibo
{

class Task;

class TaskManager : public ImplicitSingleton<TaskManager>
{
public:
    TaskManager();
    ~TaskManager();

    // sets how many calls of Task::execute should there ber per TaskManager::update
    void setNumTasksPerUpdate(int numTasksPerUpdate) { m_numTasksPerUpdate = numTasksPerUpdate; }

    // called each frame to execute m_numTasksPerUpdate takss
    void update();

    void pushTask(Task* task);

private:
    static TaskManager* m_instance;

    std::list<Task*> m_tasks;
    std::list<Task*>::iterator m_taskIterator;

    unsigned m_numTasksPerUpdate = 1;

};

}