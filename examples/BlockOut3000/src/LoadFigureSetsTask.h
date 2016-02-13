// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Task.h>
#include <maibo/Resources/ResourceFuture.h>

// Loads figure sets form a data buffer
// Saves them to the figure manager

class Lexer;
class FigureTemplate;
class FigureSet;

class LoadFigureSetsTask : public maibo::Task
{
public:
    LoadFigureSetsTask(maibo::ConstResourceFuturePtr<std::vector<char>> readFigureSets);

    bool execute() override;

    maibo::FuturePtr future;

private:

    void readOpenBlock(Lexer& lex);
    mathgp::ivector3 parseFigureElement(Lexer& lex);
    mathgp::vector3 parseFigureRotationCenter(Lexer& lex);
    FigureTemplate* parseFigureTemplate(Lexer& lex);
    FigureSet* parseSet(Lexer& lex);

    maibo::ConstResourceFuturePtr<std::vector<char>> m_readFigureSetsFuture;
    int m_errorCode = 0;
};