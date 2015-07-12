// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "FigureManager.h"
#include "FigureTemplate.h"
#include "FigureSet.h"

using namespace std;
using namespace mathgp;
using namespace maibo;

FigureManager::FigureManager()
{
}

FigureManager::~FigureManager()
{
    for(auto pair : m_figureSets)
        delete pair.second;

    for(auto fig : m_allFigureTemplates)
        delete fig;
}

void FigureManager::prepareFigureTemplatesPhysicalData()
{
    for(auto fig : m_allFigureTemplates)
    {
        fig->preparePhysicalData();
    }
}