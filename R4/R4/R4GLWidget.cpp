#include "R4GLWidget.h"
#include "ui_R4GLWidget.h"

#include <QTimer>
#include <QEvent>
#include <QMouseEvent>

#include <iostream>
#include "core/gl_core.h"

using engine::glCore;
using engine::GameEngine;
using engine::MouseState;

R4GLWidget::R4GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    ui = new Ui::R4GLWidget();
    ui->setupUi(this);

    // Init OpenGL context.
    QSurfaceFormat format;
    format.setDepthBufferSize(24 /* bits */);
    format.setVersion(4, 3); // OpenGL 4.3.
    format.setSamples(16);   // Supersampling factor.
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    this->setFormat(format);

    // Install event filter.
    this->installEventFilter(this);

    // Set up timer.
    mGameLoopTimer = new QTimer;
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoopIteration()));
    double FPS = 60.0;
    int period = static_cast<int>(1000.0/FPS);
    mGameLoopTimer->start(period);

    // Build game engine.
    mEngine = new GameEngine();
}

R4GLWidget::~R4GLWidget()
{
    delete ui;
    delete mEngine;
    delete mGameLoopTimer;
}

void R4GLWidget::gameLoopIteration()
{
    // Update game engine.
    mEngine->Update();

    QOpenGLWidget::update();
}

void R4GLWidget::initializeGL()
{
    // Set global OpenGL core.
    glCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    glCore->glClearColor(0.12, 0.12, 0.12, 0.0);

    // Load engine.
    mEngine->Load();
}

void R4GLWidget::resizeGL(int w, int h)
{
    // Forward resize event to engine.
    mEngine->Resize(w, h);
}

void R4GLWidget::paintGL()
{
    glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render engine.
    mEngine->Render();
}

void R4GLWidget::keyPressEvent(QKeyEvent* event)
{
    // Set context -> mandatory in keyboard events.
    makeCurrent();

    switch (event->key())
    {
    case Qt::Key::Key_Escape:
        exit(0);  // TODO.
        break;
    case Qt::Key::Key_F1:
        if (!QOpenGLWidget::isFullScreen())
            this->showFullScreen();
        else
            this->showNormal();
        break;

    default:
        // Forward keyboard pressed keys.
        mEngine->OnKeyboardPress(mPressedKeys);
        break;
    }

    QOpenGLWidget::update();
}

void R4GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    // Set context -> mandatory in keyboard events.
    makeCurrent();

    switch (event->key())
    {
    default:
        // Forward keyboard pressed keys.
        mEngine->OnKeyboardRelease(mPressedKeys);
        break;
    }

    QOpenGLWidget::update();
}

void R4GLWidget::mousePressEvent(QMouseEvent* event)
{
    // Filter mouse event to engine.
    if (event->buttons() & Qt::LeftButton)  // Left down.
    {
        mMouseState.mLeftDown = true;
        mEngine->OnMouseLeftDown(mMouseState);
    }
    if (event->buttons() & Qt::RightButton)  // Right down.
    {
        mMouseState.mRightDown = true;
        mEngine->OnMouseRightDown(mMouseState);
    }
    if (event->buttons() & Qt::MiddleButton) // Middle down.
    {
        mMouseState.mMiddleDown = true;
        mEngine->OnMouseMiddleDown(mMouseState);
    }

    // Update content (i.e. render model).
    QOpenGLWidget::update();
}

void R4GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // Filter mouse event to engine.
    if (mMouseState.mLeftDown && !(event->buttons() & Qt::LeftButton))  // Left up.
    {
        mMouseState.mLeftDown = false;
        mEngine->OnMouseLeftUp(mMouseState);
    }
    if (mMouseState.mRightDown && !(event->buttons() & Qt::RightButton))  // Right up.
    {
        mMouseState.mRightDown = false;
        mEngine->OnMouseRightUp(mMouseState);
    }
    if (mMouseState.mMiddleDown && !(event->buttons() & Qt::MiddleButton)) // Middle up.
    {
        mMouseState.mMiddleDown = false;
        mEngine->OnMouseMiddleUp(mMouseState);
    }
   
    QOpenGLWidget::update();
}

void R4GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Update mouse speed during motion.
    mMouseState.mSpeed = (event->pos() - mMouseState.mLastPos);  // Divide by time?
    
    // Forward mouse event to the engine.
    mEngine->OnMouseMove(mMouseState);

    // Update content (i.e. render model).
    QOpenGLWidget::update();

    // Save last position.
    mMouseState.mLastPos = event->pos();
}

// Multikeys: 
// http://stackoverflow.com/questions/23816380/using-multiple-keys-in-qt-c
bool R4GLWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        mPressedKeys += ((QKeyEvent*)event)->key();
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        mPressedKeys -= ((QKeyEvent*)event)->key();
    }

    // Check if command keys are pressed.
    mMouseState.mCtrlDown  = mPressedKeys.contains(Qt::Key_Control);
    mMouseState.mShiftDown = mPressedKeys.contains(Qt::Key_Shift);

    return false;
}