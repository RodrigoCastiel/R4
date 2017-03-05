#pragma once

#include <QSet>
#include <QOpenGLWidget>
namespace Ui { class R4GLWidget; };

#include "core/game_engine.h"
#include "interaction/mouse_state.h"

class R4GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    R4GLWidget(QWidget *parent = Q_NULLPTR);
    ~R4GLWidget();

    // Rendering/Window events.
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    // User interface - keyboard.
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

    // User interface - mouse.
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

    virtual bool eventFilter(QObject* obj, QEvent* event);

private slots:

    // Game update events.
    void gameLoopIteration();

private:
    QTimer* mGameLoopTimer;
    QSet<int> mPressedKeys;
    engine::GameEngine* mEngine;
    engine::MouseState mMouseState;

    Ui::R4GLWidget *ui;
};
