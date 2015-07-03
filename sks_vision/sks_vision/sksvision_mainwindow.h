#ifndef SKSVISION_MAINWINDOW_H
#define SKSVISION_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include "func.h"

#define PI 3.14159

namespace Ui {
class sksVision_MainWindow;
}

class sksVision_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit sksVision_MainWindow(QWidget *parent = 0);
    ~sksVision_MainWindow();

protected:
    void timerEvent(QTimerEvent *e);

private:
    Ui::sksVision_MainWindow *ui;
};

#endif // SKSVISION_MAINWINDOW_H
