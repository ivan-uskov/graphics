#pragma once

#include "modifiedscenenode.h"
#include "../model/cube.h"
#include "../utils/mymath.h"

class CubeNode : public ModifiedSceneNode
{
public:
    CubeNode(SceneNode * parent, Cube const& cube);

    void render(QPainter & painter) override;

private:
    void draw(bool isOnlyBorder);

    Cube m_cube;
};
