#pragma once

#include <QtWidgets/QMainWindow>
#include <QPdfWriter>


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

public slots:
    void addPhoto(const QString& path);
    void createPdf();

signals:


protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    Ui::ToPdfClass *ui;

    QString savePath = "";
};

