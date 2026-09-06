#include "ToPdf.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/ToPdf/icon.ico"));

    QString path = QCoreApplication::applicationDirPath() + "/style.qss";
    QFile style(path);

    if (!style.exists())
        style.setFileName("style.qss");

    if (style.open(QFile::ReadOnly | QFile::Text))
    {
        QString styleSheet = style.readAll();
        app.setStyleSheet(styleSheet);
        style.close();
    }
    else
        qWarning() << "Warning: style.qss file not found";

    ToPdf window;
    window.show();
    return app.exec();
}
