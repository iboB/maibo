#include "Preferences.h"

using namespace mathgp;
using namespace std;

Preferences::Preferences()
    : m_figureDropTime(100)
{
    m_levelLayerColors =
    {
        vc(0, 0, 0.8f, 1),
        vc(0, 0.85f, 0, 1),
        vc(0, 0.6f, 0.6f, 1),
        vc(0.75f, 0, 0, 1),
        vc(0.70f, 0.1f, 0.55f, 1),
        vc(0.88f, 0.80f, 0.1f, 1),
        vc(0.40f, 0.0f, 0.88f, 1),
        vc(0.98f, 0.5f, 0.0f, 1),
        vc(0.98f, 0.88f, 0.88f, 1),
        vc(0.58f, 0.48f, 0.58f, 1),
        vc(0, 0, 0.8f, 1),
        vc(0, 0.85f, 0, 1),
        vc(0, 0.6f, 0.6f, 1),
        vc(0.75f, 0, 0, 1),
        vc(0.70f, 0.1f, 0.55f, 1),
        vc(0.88f, 0.80f, 0.1f, 1),
        vc(0.40f, 0.0f, 0.88f, 1),
        vc(0.98f, 0.5f, 0.0f, 1),
        vc(0.98f, 0.88f, 0.88f, 1),
        vc(0.58f, 0.48f, 0.58f, 1),
    };
}

const vector4& Preferences::layerColor(uint32_t layer) const
{
    static const vector4 defaultColor = vc(0.5, 0.5, 0.5, 1);
    if (layer >= m_levelLayerColors.size())
    {
        return defaultColor;
    }

    return m_levelLayerColors[layer];
}