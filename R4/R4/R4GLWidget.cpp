#include "R4GLWidget.h"
#include "ui_R4GLWidget.h"

#include <QTimer>
#include <QEvent>
#include <QMouseEvent>

#include <iostream>
#include "core/gl_core.h"

using engine::glCore;

R4GLWidget::R4GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    ui = new Ui::R4GLWidget();
    ui->setupUi(this);

    mMouseState = { {0, 0}, {0, 0} };

    // Init OpenGL.
    QSurfaceFormat format;
    format.setDepthBufferSize(24 /* bits */);
    format.setVersion(4, 3); // OpenGL 4.3.
    format.setSamples(16);   // Supersampling factor.
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    this->setFormat(format);

    // Set up timer.
    mGameLoopTimer = new QTimer;
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoopIteration()));
 
    double FPS = 60.0;
    int period = static_cast<int>(1000.0/FPS);
    mGameLoopTimer->start(period);
}

R4GLWidget::~R4GLWidget()
{
    delete ui;
    delete mGameLoopTimer;
}

void R4GLWidget::gameLoopIteration()
{
    static unsigned sFrame = 0;
    //std::cout << "FRAME " << sFrame << std::endl;

    QOpenGLWidget::update();

    sFrame++;
}

void R4GLWidget::initializeGL()
{
    // Set OpenGL core for engine.
    glCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    glCore->glClearColor(0.12, 0.12, 0.12, 0.0);
}

void R4GLWidget::resizeGL(int w, int h)
{
    
}

void R4GLWidget::paintGL()
{
    glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        break;
    }

    QOpenGLWidget::update();
}

void R4GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    

    QOpenGLWidget::update();
}

void R4GLWidget::mousePressEvent(QMouseEvent* event)
{
    // Initialize mouse motion data.
    
    

    // Update content (i.e. render model).
    QOpenGLWidget::update();
}

void R4GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
   
    QOpenGLWidget::update();
}

void R4GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Update mouse speed during motion.
    mMouseState.mSpeed = (event->pos() - mMouseState.mLastPos);  // Divide by time?
    mMouseState.mLastPos = event->pos();

    // Update content (i.e. render model).
    QOpenGLWidget::update();

    // Save last position.
    //mMouseState.mLastPos = event->pos();
}