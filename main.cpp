#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QFileInfo> // Add this include
#include <QIcon> // Add this include

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application icon from resource
    a.setWindowIcon(QIcon(":/icons/app.ico"));

    MainWindow w;

    // Check for file argument (for "Open with Lino" feature)
    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            w.plainTextEdit->setPlainText(in.readAll());
            file.close();
            w.currentFilePath = filePath;
            w.setWindowTitle(QFileInfo(filePath).fileName() + " - Qt Text Editor");
        }
    }

    w.show();
    return a.exec();
}
