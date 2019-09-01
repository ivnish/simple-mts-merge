#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QDir(a.applicationDirPath() + "/ffmpeg").exists("ffmpeg.exe"))
    {
        QMessageBox::critical(nullptr, a.translate("QMessageBox", "Error"), a.translate("QMessageBox", "ffmpeg not found! Please reinstall application"));
        return 1;
    }

    QTranslator qtLanguageTranslator;
    qtLanguageTranslator.load(QString("ru_RU"));
    qApp->installTranslator(&qtLanguageTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
