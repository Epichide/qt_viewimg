#ifndef IMAGEBOX_H
#define IMAGEBOX_H

#include <QObject>

#include "stdxfm.h"
#include <QImage>
#include "imagereader.h"


#include <vector>

/* =↓=↓=↓=↓=↓=↓=  ImageBox  =↓=↓=↓=↓=↓=↓= */
// store ori image , b&C image ,final image
// triangles, spine, dendrite and so on

using cv::Mat;
using cv::Scalar;
using cv::Size;
using cv::Point;
using cv::Vec4i;
using cv::Vec3b;



class ImageBox : public QObject
{
    Q_OBJECT
public:
    explicit ImageBox(QObject *parent = nullptr);
    explicit ImageBox(QImage *img,QObject *parent = nullptr);
    ~ImageBox();
    QImage * oriimg=nullptr;//for showing pixel value
    QImage * bcimg=nullptr;//for showing
    QImage * result_img=nullptr;
    void set_bcimg(QImage img);
    void set_bc(Mat &mat);

    Mat orimat;
    Mat graymat;// for process and analysis

    Mat bcmat;
    Mat result_mat;


    void clearitems();

signals:

};
cv::Mat QImage2cvMat(const QImage& image);
QImage Mat2QImage(cv::Mat image);
#endif // IMAGEBOX_H
