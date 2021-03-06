#include "spherenode.h"
#include "../utils/mycast.h"
#include <vector>

using namespace MyMath;
using namespace std;

SphereNode::SphereNode(SceneNode * parent, Sphere const& sphere, int accuracy)
    : ModifiedSceneNode(parent)
    , m_sphere(sphere)
    , m_accuracy(accuracy)
{
}

void SphereNode::render(QPainter &)
{
    auto tetrahedron = m_sphere.getTetrahedron();
    auto verticeses = tetrahedron.getVertices();

    vector<Triangle> faceses =
    {
        {0, 1, 2},
        {0, 2, 3},
        {3, 1, 0},
        {1, 3, 2}
    };

    triangulate(faceses, verticeses);

    auto vertices = MyMath::vector3DToSimpleVertexArray(verticeses);
    auto faces = MyMath::triangleToVertexIndexArray(faceses);
    auto normales = fillNormales(verticeses);

    prepareVertexArray(vertices);

    glVertexPointer(VECTOR_3_SIZE, GL_FLOAT, sizeof(SimpleVertex), &vertices[0].pos);
    glColorPointer(VECTOR_4_SIZE, GL_UNSIGNED_BYTE, sizeof(SimpleVertex), &vertices[0].color);
    glNormalPointer(GL_FLOAT, sizeof(Vec3), normales.data());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glDrawElements(GL_TRIANGLES, (GLsizei)faces.size(), GL_UNSIGNED_INT, faces.data());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void SphereNode::triangulate(vector<Triangle> & triangles, vector<QVector3D> & vertices) const
{
    auto accuracy = m_accuracy;
    while (accuracy-- > 0)
    {
        triangulateOnce(triangles, vertices);
    }
}

void SphereNode::triangulateOnce(vector<Triangle> & triangles, vector<QVector3D> & vertices) const
{
    vector<Triangle> newTriangles;
    for (Triangle const& tr : triangles)
    {
        auto p1 = vertices[tr.p1];
        auto p2 = vertices[tr.p2];
        auto p3 = vertices[tr.p3];
        auto p12 = middle(p1, p2);
        auto p13 = middle(p1, p3);
        auto p23 = middle(p2, p3);

        auto ip12 = (VertexIndex) vertices.size();
        vertices.push_back(sphereProject(p12));
        auto ip13 = (VertexIndex) vertices.size();
        vertices.push_back(sphereProject(p13));
        auto ip23 = (VertexIndex) vertices.size();
        vertices.push_back(sphereProject(p23));

        newTriangles.push_back({tr.p1, ip12, ip13});
        newTriangles.push_back({ip12, tr.p2, ip23});
        newTriangles.push_back({ip12, ip23, ip13});
        newTriangles.push_back({ip13, ip23, tr.p3});
    }

    triangles.clear();
    triangles = newTriangles;
}

QVector3D SphereNode::sphereProject(QVector3D const& vertex) const
{
    auto vertexVector = (vertex - m_sphere.position()).normalized();
    return vertexVector * m_sphere.radius();
}

vector<Vec3> SphereNode::fillNormales(vector<QVector3D> const& vertices)
{
    vector<Vec3> normales;

    for (auto const& vertex : vertices)
    {
        auto n = (vertex - m_sphere.position()).normalized();
        normales.push_back({n.x(), n.y(), n.z()});
    }

    return normales;
}
