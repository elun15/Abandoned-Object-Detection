#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:

    explicit ImageWidget(QWidget *parent = 0) : QWidget(parent) {}

    QSize sizeHint() const { return _qimage.size(); }
    QSize minimumSizeHint() const { return _qimage.size(); }

public slots:

    void showImage(const cv::Mat& image);

protected:
    void paintEvent(QPaintEvent* /*event*/) {
        // Display the image
        QPainter painter(this);
        painter.drawImage(QPoint(0,0), _qimage);
        painter.end();
    }

    QImage _qimage;
    cv::Mat _tmp;
};
