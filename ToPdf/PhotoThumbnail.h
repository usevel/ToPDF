#pragma once

#include <QPushButton>
#include <QLabel>

class PhotoThumbnail : public QLabel
{
	Q_OBJECT
public:
	PhotoThumbnail(const QString& path, QWidget* parent = nullptr);

signals:
	void removeRequested(PhotoThumbnail* self);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

private:
	void refreshPixmap();
	void swapContentWith(PhotoThumbnail* other);

	QPushButton* rotate;
	QPushButton* close;
	QPoint dragStartPos;

	static constexpr int ThumbSize = 220;
};