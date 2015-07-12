// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

// a named set of figures
#include "FigureSet.h"

void FigureSet::addSet(const FigureSet* s)
{
    m_figureTemplates.insert(m_figureTemplates.end(), s->m_figureTemplates.begin(), s->m_figureTemplates.end());
}
