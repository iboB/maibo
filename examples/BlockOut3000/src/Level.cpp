// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Level.h"

#include "Resources.h"
#include "CubeTemplate.h"
#include "Preferences.h"

#include <maibo/lib/GLSentries.h>
#include <maibo/lib/memory.h>

using namespace mathgp;
using namespace std;
using namespace maibo;

class LevelLayer
{
public:
    LevelLayer(const uvector2& size)
        : m_size(size)
        , m_data(new bool[size.x() * size.y()])
    {
        reset();
        glGenBuffers(1, &m_solidBuffer);
        glGenBuffers(1, &m_wireBuffer);
    }

    ~LevelLayer()
    {
        delete[] m_data;
        glDeleteBuffers(1, &m_solidBuffer);
        glDeleteBuffers(1, &m_wireBuffer);
    }

    bool isEmpty() const
    {
        return !m_numElements;
    }

    bool isFull() const
    {
        return m_numElements == m_size.x() * m_size.y();
    }

    bool at(unsigned x, unsigned y) const
    {
        assert(x < m_size.x() && y < m_size.y());
        return m_data[y * m_size.x() + x];
    }

    void setAt(unsigned x, unsigned y)
    {
        assert(x < m_size.x() && y < m_size.y());
        assert(!at(x, y));
        m_data[y * m_size.x() + x] = true;
        ++m_numElements;
        m_isDirty = true;
    }

    void update(uint32_t dt)
    {
        if (m_isDirty)
        {
            if (!isEmpty())
            {
                // time to recreate buffers
                const auto& ct = CubeTemplate::instance();

                vector<Triangle> solidTriangles;
                solidTriangles.reserve(m_numElements * ct.triangles().size());

                vector<LineSegment> wireSegments;
                wireSegments.reserve(m_numElements * ct.wireSegments().size());

                for (unsigned y = 0; y < m_size.y(); ++y)
                {
                    for (unsigned x = 0; x < m_size.x(); ++x)
                    {
                        if (at(x, y))
                        {
                            auto s = solidTriangles.insert(solidTriangles.end(), ct.triangles().begin(), ct.triangles().end());
                            while (s != solidTriangles.end())
                            {
                                for (auto& elem : *s)
                                {
                                    elem += vc(float(x), float(y), 0);
                                }
                                ++s;
                            }

                            auto w = wireSegments.insert(wireSegments.end(), ct.wireSegments().begin(), ct.wireSegments().end());
                            while (w != wireSegments.end())
                            {
                                for (auto& elem : *w)
                                {
                                    // first make the elements a tiny bit bigger in order to draw them with depth testing enabled
                                    static const float e = 0.005f;
                                    for (auto& c : elem)
                                    {
                                        if (c == 0)
                                        {
                                            c -= e;
                                        }
                                        else
                                        {
                                            c += e;
                                        }
                                    }
                                    elem += vc(float(x), float(y), 0);
                                }
                                ++w;
                            }
                        }
                    }
                }

                glBindBuffer(GL_ARRAY_BUFFER, m_solidBuffer);
                glBufferData(GL_ARRAY_BUFFER, data_size(solidTriangles), solidTriangles.data(), GL_DYNAMIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, m_wireBuffer);
                glBufferData(GL_ARRAY_BUFFER, data_size(wireSegments), wireSegments.data(), GL_DYNAMIC_DRAW);
            }

            m_isDirty = false;
        }
    }

    void draw(const float pos, const vector4& solidColor, const vector4& wireColor)
    {
        assert(!isEmpty());
        UniformColorMaterial& m = Resources::instance().uniformColorMaterial;
        m.setModel(matrix::translation(0, 0, pos));
        m.setColor(solidColor);
        m.prepareBuffer(m_solidBuffer, sizeof(vector3), 0);
        glDrawArrays(GL_TRIANGLES, 0, m_numElements * uint32_t(CubeTemplate::instance().triangles().size()) * 3);

        m.setColor(wireColor);
        m.prepareBuffer(m_wireBuffer, sizeof(vector3), 0);
        glDrawArrays(GL_LINES, 0, m_numElements * uint32_t(CubeTemplate::instance().wireSegments().size()) * 2);
    }

    // animate drop after a layer has been erased
    void drop(float distance)
    {

    }

    // layer has been erased, to be pushed to the end
    void reset()
    {
        zero_memory(m_data, m_size.x() * m_size.y());
        m_isDirty = false;
        m_numElements = 0;
    }

private:
    const uvector2 m_size;
    bool* m_data;

    // number of occupied fields
    unsigned m_numElements;

    bool m_isDirty; // buffers need regen

    GLuint m_solidBuffer = 0;
    GLuint m_wireBuffer = 0;
};

Level::Level(const uvector3& size)
    : m_size(size)
    , m_levelLayers(size.z())
{
    for (auto& l : m_levelLayers)
    {
        l = new LevelLayer(size.xy());
    }
}

Level::~Level()
{
    for (auto& l : m_levelLayers)
    {
        delete l;
    }

    if (m_solidBuffer)
    {
        glDeleteBuffers(1, &m_solidBuffer);
    }

    if (m_wireBuffer)
    {
        glDeleteBuffers(1, &m_wireBuffer);
    }
}

void Level::draw() const
{
    UniformColorMaterial& m = Resources::instance().uniformColorMaterial;

    {
        MAIBO_GL_SENTRY(GLDepthWrite, false);

        m.setModel(matrix::identity());

        m.setColor(vc(0.066f, 0.066f, 0.15f, 1));
        m.prepareBuffer(m_solidBuffer, sizeof(vector3), 0);
        glDrawArrays(GL_TRIANGLES, 0, m_numSolidVertices);

        m.setColor(vc(0.13f, 0.30f, 0.15f, 1));
        m.prepareBuffer(m_wireBuffer, sizeof(vector3), 0);
        glDrawArrays(GL_LINES, 0, m_numWireVertices);
    }

    //const auto& ct = CubeTemplate::instance();
    //m.setModel(matrix::translation(1, 1, 8));
    //m.setColor(vc(1, 0, 1, 1));
    //m.prepareBuffer(ct.solidBuffer(), sizeof(vector3), 0);
    //glDrawArrays(GL_TRIANGLES, 0, ct.triangles().size() * 3);

    for (unsigned i = 0; i < m_levelLayers.size(); ++i)
    {
        auto l = m_levelLayers[i];
        if (!l->isEmpty())
        {
            l->draw(float(i), Preferences::instance().layerColor(i), vc(0, 0, 0, 1));
        }
    }
}

void Level::createBuffers()
{
    float w = float(m_size.x());
    float h = float(m_size.y());
    float d = float(m_size.z());

    // biggest size of w and h tobe trunkated to 1
    float dim = max(w, h)/2;

    m_viewTransfrosm = matrix::scaling_uniform(1/dim) * matrix::translation(-w / 2, -h / 2, -d);

    point3 solidVertices[] =
    {
        //bottom
        vc(0, 0, 0),
        vc(w, 0, 0),
        vc(w, h, 0),

        vc(w, h, 0),
        vc(0, h, 0),
        vc(0, 0, 0),

        //south
        vc(0, 0, 0),
        vc(0, 0, d),
        vc(w, 0, 0),

        vc(w, 0, 0),
        vc(0, 0, d),
        vc(w, 0, d),

        //east
        vc(w, 0, 0),
        vc(w, 0, d),
        vc(w, h, d),

        vc(w, h, d),
        vc(w, h, 0),
        vc(w, 0, 0),

        // north
        vc(0, h, 0),
        vc(w, h, 0),
        vc(w, h, d),

        vc(w, h, d),
        vc(0, h, d),
        vc(0, h, 0),

        // west
        vc(0, 0, 0),
        vc(0, h, 0),
        vc(0, 0, d),

        vc(0, 0, d),
        vc(0, h, 0),
        vc(0, h, d),

    };

    glGenBuffers(1, &m_solidBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_solidBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(solidVertices), solidVertices, GL_STATIC_DRAW);
    m_numSolidVertices = _countof(solidVertices);

    vector<point3> wireVertices;
    wireVertices.reserve(2 * 3 * (m_size.x() + 1) + 2 * 3 * (m_size.y() + 1) + 2 * 4 * (m_size.z() + 1));

    // concentric squares
    for (unsigned i = 0; i<m_size.z() + 1; ++i)
    {
        float d = float(i);
        wireVertices.insert(wireVertices.end(), {
            vc(0, 0, d),
            vc(w, 0, d), vc(w, 0, d),
            vc(w, h, d), vc(w, h, d),
            vc(0, h, d), vc(0, h, d),
            vc(0, 0, d)
        });
    }

    // vertical U
    for (unsigned i = 0; i<m_size.x() + 1; ++i)
    {
        float w = float(i);
        wireVertices.insert(wireVertices.end(), {
            vc(w, 0, d),
            vc(w, 0, 0), vc(w, 0, 0),
            vc(w, h, 0), vc(w, h, 0),
            vc(w, h, d)
        });
    }

    // horizontal U
    for (unsigned i = 0; i<m_size.y() + 1; ++i)
    {
        float h = float(i);
        wireVertices.insert(wireVertices.end(), {
            vc(0, h, d),
            vc(0, h, 0), vc(0, h, 0),
            vc(w, h, 0), vc(w, h, 0),
            vc(w, h, d)
        });
    }

    glGenBuffers(1, &m_wireBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_wireBuffer);
    glBufferData(GL_ARRAY_BUFFER, wireVertices.size()*sizeof(point3), wireVertices.data(), GL_STATIC_DRAW);
    m_numWireVertices = unsigned(wireVertices.size());
}

bool Level::canFitFigure(const vector<ivector3>& elements) const
{
    for (const auto& e : elements)
    {
        if (e.x() < 0 || e.x() >= int(m_size.x())
            || e.y() < 0 || e.y() >= int(m_size.y())
            || e.z() < 0 || e.z() >= int(m_size.z()))
        {
            // element is outside of level
            return false;
        }

        if (m_levelLayers[e.z()]->at(e.x(), e.y()))
        {
            return false;
        }
    }

    return true;
}

void Level::adoptFigure(const vector<ivector3>& elements)
{
    for (const auto& e : elements)
    {
        assert(e.x() >= 0 && e.x() < int(m_size.x())
            && e.y() >= 0 && e.y() < int(m_size.y())
            && e.z() >= 0 && e.z() < int(m_size.z()));

        m_levelLayers[e.z()]->setAt(e.x(), e.y());
    }
}

void Level::update(uint32_t dt)
{
    // custom remove_if so as to drop layers efficiently
    float drop = 0;

    vector<LevelLayer*> fullLayers;
    fullLayers.reserve(m_levelLayers.size());

    auto available = m_levelLayers.begin();
    for (auto i = m_levelLayers.begin(); i != m_levelLayers.end(); ++i)
    {
        if ((*i)->isFull())
        {
            fullLayers.push_back(*i);
        }
        else
        {
            (*i)->drop(float(i - available));
            *available++ = *i;
        }
    }

    assert(m_levelLayers.end() - available == fullLayers.size());
    for (auto l : fullLayers)
    {
        *available++ = l;
        l->reset();
    }

    for (auto l : m_levelLayers)
    {
        l->update(dt);
    }
}
