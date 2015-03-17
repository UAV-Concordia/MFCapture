#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <mfapi.h>
#include <Mfobjects.h>
#include <mfidl.h>
#include <Mferror.h>
#include <mfreadwrite.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
