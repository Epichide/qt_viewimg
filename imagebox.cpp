#include "imagebox.h"
#include "switchformat.h"
ImageBox::ImageBox(QObject *parent)
    : QObject{parent}
{

}

ImageBox::ImageBox(QImage *img, QObject *parent)
: QObject{parent}
{
    oriimg=img;
}

ImageBox::~ImageBox()
{
    delete oriimg;
    delete bcimg;

}

void ImageBox::set_bcimg(QImage img)
{
    delete bcimg;
    bcimg= new QImage(img);
}

void ImageBox::set_bc(cv::Mat &mat)
{
    bcmat=mat.clone();
    delete bcimg;
    bcimg= new QImage(Mat2QImage(bcmat));

}

void ImageBox::clearitems()
{

}

