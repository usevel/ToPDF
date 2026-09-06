#pragma once

#include <QtWidgets/QMainWindow>
#include <QPdfWriter>
#include <QWidget>
#include <QImage>
#include <QSize>


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

    void    addPhoto(const QString& path);
    QImage  extractRotatedImage(QWidget* container);
    void    drawImageCentered(QPainter& paint, int x, int y, const QSize& maxSize, const QImage& image);
    void    createPdf();



protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    Ui::ToPdfClass *ui;

    QString savePath = "";
};

