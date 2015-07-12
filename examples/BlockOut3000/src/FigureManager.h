// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// management of figures. lifetime... generation,,,

#include <maibo/lib/ExplicitSingleton.h>
#include <maibo/Future.h>

class FigureTemplate;
class FigureSet;
class Lexer;

class FigureManager : public maibo::ExplicitSingleton<FigureManager>
{
public:
    //const FigureSet* GetFigureSet(const std::string& setName) const;    

    void prepareFigureTemplatesPhysicalData();

    std::vector<FigureTemplate*> m_allFigureTemplates; // debug here
private:
    MAIBO_DECLARE_EXPLICIT_SINGLETON(FigureManager);

    friend class LoadFigureSetsTask;

    std::unordered_map<std::string, FigureSet*> m_figureSets;
};