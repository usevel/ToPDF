#include "PhotoThumbnail.h"

#include <QDragEnterEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QTransform>
#include <QDropEvent>
#include <QMimeData>
#include <QPixmap>
#include <QDrag>

PhotoThumbnail::PhotoThumbnail(const QString& path, QWidget* parent)
	: QLabel(parent)
{
	setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    setProperty("photoPath", path);
    setProperty("rotation", 0);
    refreshPixmap();

    rotate = new QPushButton("⭮", this);
    rotate->setFixedSize(28, 28);
    rotate->move(6, 6);
    rotate->raise();
    rotate->setStyleSheet(R"(
        QPushButton {
            font-size: 20px;
            background-color: rgba(30, 30, 30, 180);
            color: white;
            border-radius: 14px;
            border: none;

        }
        QPushButton:hover {
            background-color: rgba(60, 60, 60, 200);
        }
    )");

    connect(rotate, &QPushButton::clicked, this, [this]() {
        int angle = (this->property("rotation").toInt() + 90) % 360;
        setProperty("rotation", angle);
        refreshPixmap();
    });

    close = new QPushButton("×", this);
    close->setFixedSize(28, 28);
    close->move(ThumbSize - 28 - 6, 6);
    close->raise();
    close->setStyleSheet(R"(
        QPushButton {
            font-size: 20px;
            background-color: rgba(30, 30, 30, 180);
            color: white;
            border-radius: 14px;
            border: none;

        }
        QPushButton:hover {
            background-color: rgba(60, 60, 60, 200);
        }
    )");

    connect(close, &QPushButton::clicked, this, [this]() {
        emit removeRequested(this);
    });
}

void PhotoThumbnail::refreshPixmap()
{
    QPixmap orig(property("photoPath").toString());
    if (orig.isNull())
        return;

    int angle = property("rotation").toInt();
    if (angle != 0)
    {
        QTransform tr;
        tr.rotate(angle);
        orig = orig.transformed(tr, Qt::SmoothTransformation);
    }

    setPixmap(orig.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void PhotoThumbnail::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPos = event->pos();

    QLabel::mousePressEvent(event);
}

void PhotoThumbnail::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance())
        return;

    QDrag* drag = new QDrag(this);
    QMimeData* mime = new QMimeData;

    mime->setData("application/x-photothumb", QByteArray::number(reinterpret_cast<quintptr>(this)));
    drag->setMimeData(mime);
    drag->setPixmap(pixmap());
    drag->exec(Qt::MoveAction);
}

void PhotoThumbnail::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-photothumb"))
        event->acceptProposedAction();
}

void PhotoThumbnail::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-photothumb"))
        event->acceptProposedAction();
}

void PhotoThumbnail::dropEvent(QDropEvent* event)
{
    quintptr ptr = event->mimeData()->data("application/x-photothumb").toULongLong();
    auto* source = reinterpret_cast<PhotoThumbnail*>(ptr);

    if (source && source != this)
        swapContentWith(source);

    event->acceptProposedAction();
}

void PhotoThumbnail::swapContentWith(PhotoThumbnail* other)
{
    QVariant tempPath = property("photoPath");
    QVariant tempRotation = property("rotation");

    setProperty("photoPath", other->property("photoPath"));
    setProperty("rotation",  other->property("rotation"));
    other->setProperty("photoPath", tempPath);
    other->setProperty("rotation",  tempRotation);

    refreshPixmap();
    other->refreshPixmap();
}