// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LoadFigureSetsTask.h"

#include "FigureManager.h"
#include "FigureTemplate.h"
#include "SimpleLexer.h"
#include "FigureSet.h"

using namespace std;
using namespace mathgp;
using namespace maibo;

LoadFigureSetsTask::LoadFigureSetsTask(ConstResourceFuturePtr<std::vector<char>> readFigureSets)
    : future(new Future)
    , m_readFigureSetsFuture(readFigureSets)
{
}

bool LoadFigureSetsTask::execute()
{
    assert(m_readFigureSetsFuture->isDone());

    int error = m_readFigureSetsFuture->errorCode();
    if (error != 0)
    {
        future->setProgress(1.f);
        future->setErrorCode(error);
        future->setDone();
        return true;
    }

    string figureData(m_readFigureSetsFuture->resource().begin(), m_readFigureSetsFuture->resource().end());
    istringstream fin(figureData);

    Lexer lex(fin);

    while (true)
    {
        string token = lex.getNextToken();
        if (token == Lexer::EoF)
        {
            break;
        }
        else if (token == "set")
        {
            FigureSet* set = parseSet(lex);
            FigureManager::instance().m_figureSets[set->name()] = set;
        }
        else
        {
            cerr << "Expected \"set\", but got " << token << endl;
            m_errorCode = 1;
            break;
        }
    }

    future->setProgress(1.f);
    future->setErrorCode(m_errorCode);
    future->setDone();
    return true;
}

void LoadFigureSetsTask::readOpenBlock(Lexer& lex)
{
    string token = lex.getNextToken();
    if (token != "{")
    {
        cerr << "Expected \"{\"" << endl;
        m_errorCode = 1;
    }
}

ivector3 LoadFigureSetsTask::parseFigureElement(Lexer& lex)
{
    ivector3 result;
    result.x() = lex.getInt();
    result.y() = lex.getInt();
    result.z() = lex.getInt();
    return result;
}

vector3 LoadFigureSetsTask::parseFigureRotationCenter(Lexer& lex)
{
    vector3 result;
    result.x() = lex.getFloat();
    result.y() = lex.getFloat();
    result.z() = lex.getFloat();
    return result;
}

FigureTemplate* LoadFigureSetsTask::parseFigureTemplate(Lexer& lex)
{
    std::unique_ptr<FigureTemplate> theFigure(new FigureTemplate);

    theFigure->setName(lex.getQuotedString());

    readOpenBlock(lex);

    while (true)
    {
        string token = lex.getNextToken();

        if (token == "e")
        {
            theFigure->addElement(parseFigureElement(lex));
        }
        else if (token == "c")
        {
            auto c = parseFigureRotationCenter(lex);
            theFigure->setRotationCenter(c);            
        }
        else if (token == "}")
        {
            return theFigure.release();
        }
        else
        {
            cerr << "Expected \"e\" or \"c[axis]\" or \"}\", but got " << token << endl;
            m_errorCode = 1;
            return nullptr;
        }
    }

    assert(false); // wtf
    return nullptr;
}

FigureSet* LoadFigureSetsTask::parseSet(Lexer& lex)
{
    std::unique_ptr<FigureSet> theSet(new FigureSet);

    theSet->setName(lex.getQuotedString());
    auto& sets = FigureManager::instance().m_figureSets;

    auto found = sets.find(theSet->name());
    if (found != sets.end())
    {
        cerr << "Set \"" << theSet->name() << "\" has already been defined" << endl;
        m_errorCode = 1;
        return nullptr;
    }

    readOpenBlock(lex);

    while (true)
    {
        string token = lex.getNextToken();
        if (token == "desc")
        {
            theSet->setDescription(lex.getQuotedString());
        }
        else if (token == "figure")
        {
            FigureTemplate* fig = parseFigureTemplate(lex);
            FigureManager::instance().m_allFigureTemplates.push_back(fig);
            theSet->addFigure(fig);
        }
        else if (token == "}")
        {
            return theSet.release();
        }
        else if (token == "include")
        {
            string inc = lex.getQuotedString();
            if (inc == theSet->name())
            {
                cerr << "Trying to include an the set \"" << inc << "\" in itself" << endl;
                m_errorCode = 1;
                return nullptr;
            }

            auto found = sets.find(inc);

            if (found == sets.end())
            {
                cerr << "Trying to include an unknown set \"" << inc << "\"" << endl;
                m_errorCode = 1;
                return nullptr;
            }

            const FigureSet* includeSet = found->second;
            theSet->addSet(includeSet);
        }
        else
        {
            cerr << "Expected \"figure\" or \"desc\" or \"}\", but got " << token << endl;
            m_errorCode = 1;
            return nullptr;
        }
    }

    assert(false); // shouldn't get here
    return nullptr;
}

