#include "PrecomputationTools.h"

#include "ObjTool.h"

PrecomputationTools::PrecomputationTools(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(OnActionQuit()), 
        Qt::UniqueConnection);

    connect(ui.actionLoadObj, SIGNAL(triggered()), ui.objTool, SLOT(OnLoadObj()), Qt::UniqueConnection);
    connect(ui.actionGenerateGLB, SIGNAL(triggered()), ui.objTool, SLOT(OnGenerateGLB()), Qt::UniqueConnection);
}

void PrecomputationTools::OnActionQuit()
{

}