#include "ObjTool.h"
#include "ui_ObjTool.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

using obj_tool::ObjData;
using obj_tool::ObjParser;

ObjTool::ObjTool(QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::ObjTool();
    ui->setupUi(this);

    connect(ui->pb_GenerateGLB, SIGNAL(clicked()), this, SLOT(OnGenerateGLB()));
}

ObjTool::~ObjTool()
{
    delete ui;
}


void ObjTool::OnLoadObj()
{
    // TODO. Optionally: ask for user before proceeding.
    /*if (mObjLoaded)
    {
    
    }*/

    qDebug() << "ObjTool::OnLoadObj()\n";
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open .OBJ file"), "", tr("OBJ (*.obj);"));
    QFileInfo fileInfo(filePath);

    if (filePath.isEmpty())
        return;

    QMessageBox::information(this, tr("Please WAIT!"),
        "PLEASE WAIT!");

    mObjData.Clear();
    bool loaded = mObjParser.LoadObj(fileInfo.absolutePath().toStdString(),
                                     fileInfo.fileName().toStdString(), mObjData, true);

    if (!loaded)
    {
        // Loading error.
        QMessageBox::information(this, tr("Unable to open .obj"),
                                 "Make sure it is a valid .OBJ file.");
        return;
    }

    ui->objInfo->clear();
    std::string log_output;
    mObjData.LogData(log_output);
    ui->objInfo->append(QString::fromStdString(log_output));
    mObjData.LogData();

    // Update UI data.
    ui->label_Status->setText(filePath);
    ui->lineEditNumVertices->setText(QString::number(mObjData.GetNumVertices()));
    ui->lineEditNumNormals->setText(QString::number(mObjData.GetNumNormals()));
    ui->lineEditNumUVs->setText(QString::number(mObjData.GetNumUVs()));
    ui->lineEditNumGroups->setText(QString::number(mObjData.GetNumGroups()));
    ui->lineEditNumFaces->setText(QString::number(mObjData.GetNumFaces()));
    ui->lineEditNumObjs->setText(QString::number(mObjData.GetNumObjs()));
    ui->lineEditMTL->setText(QString::fromStdString(mObjData.GetMaterialFilename()));

    mObjLoaded = true;
}

void ObjTool::OnGenerateGLB()
{
    if (!mObjLoaded)
    {
        // Loading error.
        QMessageBox::information(this, tr("ERROR No .obj file is loaded."),
            "Please use ObjTool > Load .obj file.");
        return;
    } 

    mObjData.TriangulateQuads();
    mObjData.MoveCenterToOrigin();

    // Export to r4o.
    QString objFilepath = ui->label_Status->text();
    QFileInfo objFileInfo(objFilepath);
    QString baseFolderPath = objFileInfo.absolutePath();
    QString groupsFolderPath = objFilepath.mid(0, objFilepath.size()-4);
    QString groupsFolderName = objFileInfo.fileName().mid(0, objFileInfo.fileName().size()-4);
    
    if (!QDir(groupsFolderPath).exists())
    {
        QDir().mkpath(groupsFolderPath);
    }
    mObjData.ExportToR4O(baseFolderPath, groupsFolderName);
}

void ObjTool::OnGenerateBBH()
{

}