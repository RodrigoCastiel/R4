#pragma once

#include <QWidget>

#include "obj_data.h"
#include "obj_parser.h"

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
    obj_tool::ObjData mObjData;
    obj_tool::ObjParser mObjParser;
};
