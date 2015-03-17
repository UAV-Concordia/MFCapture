#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <mfapi.h>
#include <Mfobjects.h>
#include <mfidl.h>
#include <Mferror.h>
#include <mfreadwrite.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateFrame();

private:
    void pass(HRESULT hr);
    Ui::MainWindow *ui;
    QTimer *timer;
    IMFSourceReader *pReader;
};

#endif // MAINWINDOW_H
