#pragma once

#include <QWidget>

#include "obj-loader/obj_data.h"
#include "obj-loader/obj_parser.h"

namespace Ui { class ObjTool; };

class ObjTool : public QWidget
{
    Q_OBJECT

public:
    ObjTool(QWidget *parent = Q_NULLPTR);
    ~ObjTool();

public slots:

    void OnLoadObj();

    void OnGenerateGLB();
    void OnGenerateBBH();

private:
    Ui::ObjTool *ui;

    bool mObjLoaded { false };
    engine::ObjData mObjData;
    engine::ObjParser mObjParser;
};
