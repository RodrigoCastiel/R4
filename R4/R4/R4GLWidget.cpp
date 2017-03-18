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
    format.setSamples(4);   // Supersampling factor.
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(1);  // VSync!
    QSurfaceFormat::setDefaultFormat(format);
    this->setFormat(format);

    // Install event filter.
    this->installEventFilter(this);

    // Set up timer.
    mGameLoopTimer = new QTimer;
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoopIteration()));
    double FPS = 500.0;
    int period = static_cast<int>(1000.0/FPS);
    mGameLoopTimer->setInterval(0);
    mGameLoopTimer->start();
    mOverrideMouseLock = false;

    // References:
    // http://stackoverflow.com/questions/17167194/how-to-make-updategl-realtime-in-qt
    // http://gamedev.stackexchange.com/questions/132831/what-is-the-point-of-update-independent-rendering-in-a-game-loop

    //this->grabMouse();

    // Build game engine.
    mEngine = new GameEngine();
    mUpdateTimer.start();
    mRenderingTimer.start();

    QApplication::setOverrideCursor(Qt::BlankCursor);
}

R4GLWidget::~R4GLWidget()
{
    delete ui;
    delete mEngine;
    delete mGameLoopTimer;
}

void R4GLWidget::gameLoopIteration()
{
    qint64 T = mUpdateTimer.elapsed();
    double FPS = 1000.0 / (T > 0 ? T : 1);
    qDebug() << "UPDATE FPS = " << FPS << "\n";
    this->setWindowTitle("R4 | " + QString::number(static_cast<int>(FPS)) + " Hz");

    makeCurrent();

    // Update game engine.
    mEngine->Update(mMouseState, mPressedKeys);

    QOpenGLWidget::update();

    mUpdateTimer.restart();
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
    qint64 T = mRenderingTimer.elapsed();
    double FPS = 1000.0/(T > 0 ? T : 1);
    qDebug() << "RENDERING FPS = " << FPS << "\n";
    //this->setWindowTitle("R4 | " + QString::number(static_cast<int>(FPS)) + " Hz");

    // Render engine.
    mEngine->Render();
    /*QOpenGLContext::swapBuffers();*/

    mRenderingTimer.restart();
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

    case Qt::Key::Key_F2:
        mOverrideMouseLock = !mOverrideMouseLock;
        break;

    default:
        // Forward keyboard pressed keys.
        mEngine->OnKeyboardPress(mPressedKeys);
        break;
    }

    //QOpenGLWidget::update();
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

    //QOpenGLWidget::update();
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
    //QOpenGLWidget::update();
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
   
    //QOpenGLWidget::update();
}

void R4GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (mEngine->LockMouse() && !mOverrideMouseLock)
    {
        const QRect & rect = geometry();
        QPoint position = QCursor::pos();

        int xo = rect.left();
        int yo = rect.top();

        QPoint center = rect.center();
        QCursor::setPos(center);

        // Update mouse speed during motion.
        mMouseState.mSpeed = position - center;  // Divide by time?

        // Forward mouse event to the engine.
        mEngine->OnMouseMove(mMouseState);
    }
    else
    {
        // Update mouse speed during motion.
        mMouseState.mSpeed = (event->pos() - mMouseState.mLastPos);  // Divide by time?

        // Forward mouse event to the engine.
        mEngine->OnMouseMove(mMouseState);

        // Save last position.
        mMouseState.mLastPos = event->pos();
    }

    // Update content (i.e. render model).
    //QOpenGLWidget::update();
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
    else if (event->type() == QEvent::Move)
    {
        //QPoint position = QCursor::pos();
        //printf("%d\t%d\t\t (w=%d)\n", position.x(), position.y(), this->width());
    }

    // Check if command keys are pressed.
    mMouseState.mCtrlDown  = mPressedKeys.contains(Qt::Key_Control);
    mMouseState.mShiftDown = mPressedKeys.contains(Qt::Key_Shift);

    return false;
}

void R4GLWidget::leaveEvent(QEvent * event)
{
    if (mEngine->LockMouse() && !mOverrideMouseLock)
    {
        const QRect & rect = geometry();
        QPoint position = QCursor::pos();

        int xo = rect.left();
        int yo = rect.top();

        QPoint center = rect.center();
        QCursor::setPos(center);

        // Update mouse speed during motion.
        mMouseState.mSpeed = position - center;  // Divide by time?
    }
    else
    {
        // Do nothing.
    }
    
    event->accept();
    QWidget::leaveEvent(event);
}
