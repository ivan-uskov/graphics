#pragma once
#include <QGuiApplication>
#include "ui/window3d.h"

class GameApplication : public QGuiApplication
{
    Q_OBJECT
public:
    GameApplication(int argc, char *argv[]);

    int enterGameLoop();

private slots:
    void loadScene();
    void moveCamera(QPointF const& deltha);
    void zoomCamera(int delthaZoom);
    void keyPressed(Qt::Key key);

private:
    Window3D m_window;
    std::shared_ptr<BaseScene> m_scene;
    QVector3D m_eye = QVector3D(6, -2, 2);
};
