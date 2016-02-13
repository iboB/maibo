// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// management of figures. lifetime... generation,,,

#include <maibo/Common/ExplicitSingleton.h>
#include <maibo/Common/Future.h>

class FigureTemplate;
class FigureSet;
class Lexer;

class FigureManager : public maibo::ExplicitSingleton<FigureManager>
{
public:
    //const FigureSet* GetFigureSet(const std::string& setName) const;

    void prepareFigureTemplatesPhysicalData();

    // return nullptr if no such set exists
    const FigureSet* getFigureSet(const std::string& name) const;

private:
    MAIBO_DECLARE_EXPLICIT_SINGLETON(FigureManager);

    friend class LoadFigureSetsTask;

    std::unordered_map<std::string, FigureSet*> m_figureSets;
    std::vector<FigureTemplate*> m_allFigureTemplates;
};