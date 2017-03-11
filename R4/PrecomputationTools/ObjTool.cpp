#include "ObjTool.h"
#include "ui_ObjTool.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

using engine::ObjData;
using engine::ObjParser;

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
    QString filename = QFileDialog::getOpenFileName(this, tr("Open .OBJ file"), "", tr("OBJ (*.obj);"));

    if (filename.isEmpty())
        return;

    QMessageBox::information(this, tr("Please WAIT!"),
        "PLEASE WAIT!");

    mObjData.Clear();
    bool loaded = mObjParser.LoadObj(filename.toStdString(), mObjData, false);

    if (!loaded)
    {
        // Loading error.
        QMessageBox::information(this, tr("Unable to open .obj"),
                                 "Make sure it is a valid .OBJ file.");
        return;
    }

    // Update UI data.
    ui->label_Status->setText(filename);
    ui->lineEditNumVertices->setText(QString::number(mObjData.GetNumVertices()));
    ui->lineEditNumNormals->setText(QString::number(mObjData.GetNumNormals()));
    ui->lineEditNumUVs->setText(QString::number(mObjData.GetNumUVs()));
    ui->lineEditNumGroups->setText(QString::number(mObjData.GetNumGroups()));
    ui->lineEditNumFaces->setText(QString::number(mObjData.GetNumFaces()));
    ui->lineEditNumObjs->setText("???");
    ui->lineEditMTL->setText("???");

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

    for (int i = 0; i < mObjData.GetNumGroups(); i++)
    {
        bool smooth_shading = (ui->cb_SmoothLighting->checkState() == Qt::Checked);
        QString objFilename = ui->label_Status->text(); // .obj
        QString folderName = objFilename.mid(0, objFilename.size()-4);
        QString name =  folderName + "/g" + QString::number(i) + ".glb";

        if (!QDir(folderName).exists())
        {
            QDir().mkpath(folderName);
        }

        mObjData.ExportToMeshGroupGLB(i, smooth_shading, name);
    }
}

void ObjTool::OnGenerateBBH()
{

}