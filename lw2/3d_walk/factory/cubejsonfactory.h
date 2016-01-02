#pragma once
#include <QJsonObject>
#include "../nodes/cubenode.h"
#include "../model/cube.h"

class CubeJsonFactory
{
public:
    static bool create(SceneNode * root, QJsonObject const& object);

private:
    static bool isCubeValid(QJsonValue const& lengthJson, QJsonArray const& positionJson);
};