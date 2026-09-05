#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ToPdf.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ToPdfClass; };
QT_END_NAMESPACE

class ToPdf : public QMainWindow
{
    Q_OBJECT

public:
    ToPdf(QWidget *parent = nullptr);
    ~ToPdf();

private:
    Ui::ToPdfClass *ui;
};

