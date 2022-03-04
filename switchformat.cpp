#include "switchformat.h"


using cv::Mat;
using cv::Scalar;
using cv::Size;
using cv::Point;
using cv::Vec4i;
using cv::Vec3b;

cv::Mat QImage2cvMat(const QImage& image)
{
    cv::Mat mat;
    switch(image.format())
    {
    // 8-bit, 4 channel
    case QImage::Format_ARGB32://AARRGGBB
        break;
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat mat(image.height(), image.width(),
                    CV_8UC4,
                    (void*)image.constBits(),
                    image.bytesPerLine());
        return mat.clone();
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB32://FFRRGGBB
    {
        mat=Mat(image.height(),image.width(),
                CV_8UC4,
                (void*)image.constBits(),
                image.bytesPerLine());

        // drop the all-white alpha channel
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
        break;
    }
    case QImage::Format_RGB888:
    {
        QImage swapped = image.rgbSwapped();
        mat=Mat(swapped.height(), swapped.width(),
                CV_8UC3,
                (void*)image.constBits(),
                image.bytesPerLine());
        break;
    }

        // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        mat=Mat(image.height(),image.width(),
                CV_8UC1,
                (void*)image.constBits(),
                image.bytesPerLine());

        break;
    }
        // 16-bit , 1 channel
    case QImage::Format_Grayscale16:
    {
        mat=Mat(image.height(),image.width(),
                CV_16UC1,
                (void*)image.constBits(),
                image.bytesPerLine());
        break;
    }
    case QImage::Format_Grayscale8:
    {
        mat=Mat(image.height(),image.width(),
                CV_8UC1,
                (void*)image.constBits(),
                image.bytesPerLine());

        break;
    }


        // wrong
    default:
        qDebug() << "ERROR: QImage could not be converted to Mat.";
        break;
    }


    return mat.clone();

}

QImage Mat2QImage(cv::Mat image)
{
    // Mat rgb;
    QImage img;


    if(image.channels()==3)
    {
        //cvt Mat BGR 2 QImage RGB
        // cv::cvtColor(image,rgb,COLOR_BGR2RGB);
        // cv::imshow("fff",rgb);
        // Here is a bug, if change image.data to rgb.data, will occour error
        QImage timg =QImage((const unsigned char*)(image.data),
                            image.cols,image.rows,
                            image.cols*3,
                            QImage::Format_RGB888);
        img =  QImage(timg.rgbSwapped());
    }
    else if(image.channels()==1)
    {
        // gray scale
        if(image.type()==CV_16UC1)
        {
            img =  QImage((const unsigned char *)(image.data),
                            image.cols,image.rows,
                            image.cols*2,
                            QImage::Format_Grayscale16);
        }
        else if(image.type()==CV_8UC1)
        {
            img =  QImage((const unsigned char *)(image.data),
                            image.cols,image.rows,
                            image.cols,
                            QImage::Format_Grayscale8);
        }

    }
    else if(image.channels()==4)
    {

        img = QImage((const unsigned char*)(image.data),
                        image.cols,image.rows,
                        image.cols*image.channels(),
                        QImage::Format_RGB32);
    }
    return img;

}

