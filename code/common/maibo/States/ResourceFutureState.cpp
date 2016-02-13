// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ResourceFutureState.h"

using namespace maibo;
using namespace std;

void ResourceFutureState::addFuture(ConstFuturePtr future)
{
    m_pendingFutures.push_back(future);
}

void ResourceFutureState::update(uint32_t dt)
{
    if (m_pendingFutures.empty())
        return;

    // move from the last to the first element so we can erase them safely
    size_t i = m_pendingFutures.size();
    do
    {
        --i;

        if (m_pendingFutures[i]->isDone())
        {
            m_doneFutures.push_back(m_pendingFutures[i]);
            m_pendingFutures.erase(m_pendingFutures.begin() + i);
        }

    } while (i != 0);

    if (m_pendingFutures.empty())
    {
        onDone();
    }
}

float ResourceFutureState::calculateProgress() const
{
    float total = float(m_pendingFutures.size() + m_doneFutures.size());
    float current = float(m_doneFutures.size());
    for (auto f : m_pendingFutures)
    {
        current += f->progress();
    }

    return current / total;
}
