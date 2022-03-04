#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include "stdxfm.h"
#include "imagereader.h"
#include "paintboard.h"
#include "imagebox.h"

using cv::Mat;
class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessor(QObject *parent = nullptr);

    //Imagebox : strore all image informatio and result
    QVector<ImageBox*> imgbox_list;




    QString m_imgname;//current image file name

    ImageReader *imgreader=new ImageReader(this);// read tif image file info, like w,h,frame :c,t,z
    PaintBoard * m_paintbord;
    int cur_idx=0;
    void JumpTo(int idx);
    void loadFromFile(QString imgname);
    QString getImgInfoText();
    int m_width,m_height,m_channel=0,m_frames=0,m_bitdepth=0;
    //-------clear buffer--------//
    void clearBuffImg();
    //----------Image process--------------//

    void Countour();
    void skeletonlized();
    void imglist2Mat();
    void ContrastAndBright(int brighter,int constract);

    void getContrastBrightHint(int & brighter, int & contrast);

signals:

};


void ContrastAndBrightMat(const Mat &image, Mat &dst,int brighter, int constract);
enum adaptiveMethod{medianFilter,gaussianFilter,meanFilter};
void AdaptiveThreshold(cv::Mat& src, cv::Mat& dst, double Maxval, int Subsize, double c, adaptiveMethod method = meanFilter);



#endif // IMAGEPROCESSOR_H
