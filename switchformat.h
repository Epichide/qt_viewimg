#ifndef SWITCHFORMAT_H
#define SWITCHFORMAT_H
#include "stdxfm.h"


#include "QPolygonF"


cv::Mat QImage2cvMat(const QImage& image);
QImage Mat2QImage(cv::Mat image);


QString point2string(const QPointF &p) ;

#endif // SWITCHFORMAT_H
