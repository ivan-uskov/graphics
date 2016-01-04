#include "modifiedscenenode.h"
#include <algorithm>

using namespace std;

ModifiedSceneNode::ModifiedSceneNode(SceneNode * parent)
    : SceneNode(parent)
{
}

void ModifiedSceneNode::advance(int64_t msec)
{
    for_each(m_modifiers.begin(), m_modifiers.end(), [msec](shared_ptr<INodeModifier> & modifier){
        modifier->advance(msec);
    });
}

void ModifiedSceneNode::prepareVertex(MyMath::SimpleVertex * vertex)
{
    for_each(m_modifiers.begin(), m_modifiers.end(), [vertex](shared_ptr<INodeModifier> & modifier){
        modifier->modify(vertex);
    });
}

void ModifiedSceneNode::addModifier(shared_ptr<INodeModifier> && modifier)
{
    m_modifiers.push_back(modifier);
}

void ModifiedSceneNode::prepareVertexArray(MyMath::SimpleVertexArray vertexArray)
{
    for (int i = 0; i < m_vertexCount; ++i)
    {
        auto vert = vertexArray + i;
        prepareVertex(vert);
    }
}
