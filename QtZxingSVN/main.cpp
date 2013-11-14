#include "mainwindow.h"
#include <QApplication>
#include <zxing/MultiFormatReader.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

