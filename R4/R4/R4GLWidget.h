#pragma once

#include <QOpenGLWidget>
namespace Ui { class R4GLWidget; };

struct MouseState
{
    QPoint mLastPos;
    QPoint mSpeed;
};

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

private slots:

    // Game update events.
    void gameLoopIteration();

private:
    Ui::R4GLWidget *ui;
    MouseState mMouseState;
    QTimer* mGameLoopTimer;
};
