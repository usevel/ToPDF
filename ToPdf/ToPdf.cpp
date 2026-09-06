#include "ToPdf.h"

#include <QDragEnterEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QPdfWriter>
#include <QMimeData>
#include <QFileInfo>
#include <QPicture>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QFile>

ToPdf::ToPdf(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToPdfClass())
{
    ui->setupUi(this);
    setAcceptDrops(true);
    this->setFixedWidth(730);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setFixedWidth(710);
    ui->scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout->setAlignment(ui->scrollArea, Qt::AlignHCenter);

    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged,
        this, [this](int min, int max) {
            ui->scrollArea->verticalScrollBar()->setValue(max);
    });

    
    ui->addPicFromDialog->setFixedSize(180,35);
    ui->addPicFromDialog->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    ui->clearPic->setFixedSize(180,35);
    ui->clearPic->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->horizontalButtonsWithPic->setAlignment(Qt::AlignRight);

    connect(ui->addPicFromDialog, &QPushButton::clicked, this, [this]() {
        QStringList paths = QFileDialog::getOpenFileNames(
            this,
            "Выберите фото",
            "",
            "Images (*.png *.jpg *.jpeg *.jfif)"
        );

        for (const QString& path : paths)
            addPhoto(path);
    });


    connect(ui->clearPic, &QPushButton::clicked, this, [this]() {
        QLayoutItem* item;
        while ((item = ui->gridForPhoto->takeAt(0)) != nullptr)
        {
            if (item->widget())
                delete item->widget();

            delete item;
        }
    });


    ui->linePath->setReadOnly(true);
    ui->linePath->setFixedSize(600, 35);
    ui->linePath->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    ui->openDialog->setFixedSize(100, 35);
    ui->openDialog->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->horizontalForPath->setAlignment(Qt::AlignHCenter);

    connect(ui->openDialog, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getSaveFileName(
            this,
            "Сохранить PDF как",
            "",
            "PDF files(*.pdf);;All files(*.*)");

        if (!path.isEmpty())
        {
            if (!path.endsWith(".pdf", Qt::CaseInsensitive))
                path += ".pdf";

            savePath = path;
            ui->linePath->setText(savePath);
        }
    });

    ui->submit->setFixedSize(300, 35);
    ui->submit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->horizontalReady->setAlignment(Qt::AlignHCenter);

    connect(ui->submit, &QPushButton::clicked, this, &ToPdf::createPdf);
}

ToPdf::~ToPdf()
{
    delete ui;
}

void ToPdf::addPhoto(const QString& path)
{
    QPixmap pixmap(path);
    if (pixmap.isNull())
        return;

    QLabel* label = new QLabel(this);
    //label->setStyleSheet("border: 1px solid #4b4b4b;");
    label->setPixmap(pixmap.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    label->setFixedSize(220, 220);
    label->setAlignment(Qt::AlignCenter);
    label->setProperty("photoPath", path);

    int col = 3;
    int count = ui->gridForPhoto->count();
    ui->gridForPhoto->addWidget(label, count / col, count % col);
}

void ToPdf::createPdf()
{
    if (savePath.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Нужно указать путь сохранения PDF");
        return;
    }

    QPdfWriter pdf(savePath);
    pdf.setPageSize(QPageSize::A4);
    pdf.setPageOrientation(QPageLayout::Orientation::Portrait);
    pdf.setResolution(300);

    QPainter paint(&pdf);
    if (!paint.isActive())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл для записи");
        return;
    }

    int maxWidth = 2000;
    int x = (pdf.width() - maxWidth) / 2;
    int y = 100;

    QLayoutItem* item;
    while ((item = ui->gridForPhoto->takeAt(0)) != nullptr)
    {
        QWidget* wid = item->widget();
        if (wid)
        {
            QString originalPath = wid->property("photoPath").toString();
            QImage image(originalPath);
            if (!image.isNull())
            {
                QSize imageSize = image.size();
                QSize maxSize(2000, 1500);

                QSize scaledSize = imageSize.scaled(maxSize, Qt::KeepAspectRatio);

                int drawX = x + (maxSize.width()  - scaledSize.width()) / 2;
                int drawY = y + (maxSize.height() - scaledSize.height()) / 2;

                paint.drawImage(QRect(drawX, drawY, scaledSize.width(), scaledSize.height()), image);
                y += maxSize.height() + 200;

                if (y > pdf.height() - maxSize.height())
                {
                    pdf.newPage();
                    y = 100;
                }
            }

            delete wid;
        }

        delete item;
    }

    paint.end();
    QMessageBox::information(this, "Успех", "PDF успешно создан");
}

void ToPdf::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void ToPdf::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void ToPdf::dropEvent(QDropEvent* event)
{
    const QStringList images = { "png", "jpg", "jpeg", "jfif" };

    for (const auto& url : event->mimeData()->urls())
    {
        QString path = url.toLocalFile();
        QFileInfo info(path);

        if (images.contains(info.suffix().toLower()))
            addPhoto(path);
    }

    event->acceptProposedAction();
}