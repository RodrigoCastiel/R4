#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PrecomputationTools.h"

class PrecomputationTools : public QMainWindow
{
    Q_OBJECT

public:
    PrecomputationTools(QWidget *parent = Q_NULLPTR);

public slots:
    void OnActionQuit();

private:
    Ui::PrecomputationToolsClass ui;
};
