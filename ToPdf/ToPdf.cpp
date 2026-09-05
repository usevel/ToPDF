#include "ToPdf.h"

#include <QDragEnterEvent>
#include <QFileDialog>
#include <QScrollBar>
#include <QMimeData>
#include <QFileInfo>
#include <QPicture>
#include <QLabel>
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
    label->setPixmap(pixmap.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    label->setFixedSize(220, 220);
    label->setAlignment(Qt::AlignCenter);

    int col = 3;
    int count = ui->gridForPhoto->count();
    ui->gridForPhoto->addWidget(label, count / col, count % col);
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