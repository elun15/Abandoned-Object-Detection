#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "imagewidget.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     Mat black;

private:
    Ui::MainWindow *ui;
     QTimer* Timer;


public slots:
    void mainProcess();
    Mat resizeImageToWidget(Mat image, ImageWidget *widget);
    const string currentDateTime();



private slots:

    void on_PauseButton_clicked();
    void on_actionOpen_triggered();

};

#endif // MAINWINDOW_H
