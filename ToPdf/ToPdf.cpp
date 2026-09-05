#include "ToPdf.h"

#include <QScrollBar>

ToPdf::ToPdf(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToPdfClass())
{
    ui->setupUi(this);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setFixedWidth(710);
    ui->scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout->setAlignment(ui->scrollArea, Qt::AlignHCenter);

    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged,
        this, [this](int min, int max) {
            ui->scrollArea->verticalScrollBar()->setValue(max);
    });


    ui->linePath->setReadOnly(true);
    ui->linePath->setFixedWidth(600);
    ui->linePath->setFixedHeight(40);
    ui->linePath->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    ui->openDialog->setFixedWidth(100);
    ui->openDialog->setFixedHeight(40);
    ui->openDialog->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->horizontalForPath->setAlignment(Qt::AlignHCenter);


    ui->submit->setFixedWidth(300);
    ui->submit->setFixedHeight(40);
    ui->submit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->horizontalReady->setAlignment(Qt::AlignHCenter);
}

ToPdf::~ToPdf()
{
    delete ui;
}