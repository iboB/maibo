// MaiBo - BlockOut3000
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <maibo/Application.h>

// Game level class

class Level
{
public:
    Level(const mathgp::uvector3& size);

    void createBuffers();

private:
    GLuint m_wireBuffer;
    unsigned m_numWireVertices;
    GLuint m_solidBuffer;
    unsigned m_numSolidVertices;

    const mathgp::uvector3 m_size;
};
