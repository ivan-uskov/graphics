#include "scenecamera.h"
#include "glhelper.h"
#include <QMatrix4x4>

SceneCamera::SceneCamera(QSize viewport, QObject *parent)
    : QObject(parent)
    , m_viewport(viewport)
{
}

void SceneCamera::loadMatrix()
{
    QMatrix4x4 matrix;
    matrix.lookAt(m_eye, m_front + m_eye, m_up);
    GLHelper::setModelViewMatrix(matrix);
}

void SceneCamera::lookAt(QVector3D const& eye, QVector3D const& at, QVector3D const& up)
{
    m_eye = eye;
    m_front = at - eye;
    m_up = up;
    m_front.normalize();
}

void SceneCamera::advance(int64_t msec)
{
    QVector3D left = QVector3D::crossProduct(m_up, m_front);
    float seconds = float(msec) * 0.001f;
    float dFront = m_speed.x() * seconds;
    float dLeft = m_speed.y() * seconds;
    float dUp = m_speed.z() * seconds;
    m_eye += m_front * dFront;
    m_eye += left * dLeft;
    m_eye += m_up * dUp;
}

QSize SceneCamera::viewport() const
{
    return m_viewport;
}

void SceneCamera::setViewport(QSize viewport)
{
    m_viewport = viewport;
}

QVector3D SceneCamera::eye() const
{
    return m_eye;
}

void SceneCamera::setSpeed(QVector3D speed)
{
    m_speed = speed;
}

QVector3D SceneCamera::speed() const
{
    return m_speed;
}

void SceneCamera::updateCamera(Camera const& newCamera)
{
    lookAt(newCamera.eye(), newCamera.at(), newCamera.up());
}
