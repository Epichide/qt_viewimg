#include "imageprocessor.h"
using cv::Mat;
using cv::Scalar;
using cv::Size;
using cv::Point;
using cv::Vec4i;
using cv::Vec3b;
ImageProcessor::ImageProcessor(QObject *parent)
    : QObject{parent}
{

}

void ImageProcessor::JumpTo(int idx)
{
    if (cur_idx==idx)return;
    ImageBox *imgbox=imgbox_list.at(idx);

    //curimg=imgbox->oriimg;
    //curPixmap=QPixmap::fromImage(*(imgbox->bcimg));
   m_paintbord->setimage( imgbox->bcimg);
   m_paintbord->LoadData(imgbox);
    m_paintbord->showData();
}
void ImageProcessor::clearBuffImg()
{
    // clear all !!! include ori img picture
    imgbox_list.clear();

}


void ImageProcessor::loadFromFile(QString imgname)
{
    cur_idx=-1;
    imgbox_list.clear();
    qDeleteAll(imgbox_list);
    //qDeleteAll(&m_imglist);
    if (imgname.isEmpty()) return;
    m_imgname=imgname;

    if (imgname.endsWith(".tif") | imgname.endsWith(".tiff"))
    {

        imgreader->iamgeinfo(imgname, m_width, m_height, m_channel, m_frames, m_bitdepth);
        if (m_frames==1 )
        {

            QImage *img=new QImage(imgname);
            if (m_channel==1 && img->depth()==8)
                img->convertTo(QImage::Format_Grayscale8);
            else if (m_channel==1 && img->depth()==16)
                img->convertTo(QImage::Format_Grayscale16);
//            else
//                img->convertTo(QImage::Format_RGB32);
            ImageBox *imgbox=new ImageBox(img);
            imgbox_list.append(imgbox);



        }
        else
        {
            //imgreader->loadImageList(imgname,&imglist);
            QImageReader reader;
            reader.setFileName(imgname);
            QImage::Format format= reader.imageFormat();

            int nCount = reader.imageCount();//wrong
            std::cout<<"image frame :"<<nCount;
            for (int i = 0; i < m_frames; ++i)
            {
                // 跳到顺序号为i的图像
                bool ret = reader.jumpToImage(i);
                if (reader.canRead())
                {
                    // 读取图像
                    QImage *image = new QImage(reader.read());
                    QImage::Format format= image->format();
                    ImageBox *imgbox=new ImageBox(image);
                    imgbox_list.append(imgbox);
                    // 保存图像
                    //(*image).save(QString("Loading%1.tif").arg(i + 1));
                    // 下一帧动画等待的毫秒数
                    //int nDelay = reader.nextImageDelay();
                    //std::cout<< QString("Number %1 Delay : %2\n").arg(i + 2).arg(nDelay).toStdString();
                }
                else
                {
                    // 获取错误信息
                    QImageReader::ImageReaderError error = reader.error();
                    QString strError = reader.errorString();
                    std::cout << "Open Image Attach data  Error : " << strError.toStdString()<<std::endl;
                }
            }


        }

    }
    else
    {
        QImage *img=new QImage(imgname);
        ImageBox *imgbox=new ImageBox(img);
        imgbox_list.append(imgbox);



        m_height=img->height();
        m_bitdepth=img->depth();
        if(img->allGray())
            m_channel=1;
        else
        {
            m_channel=3;
            img->convertTo(QImage::Format_RGB32);
        }


    }
    std::cout<<"img frames number :"<<imgbox_list.length()<<std::endl;
    imglist2Mat();
    ContrastAndBright(0,100);

}

QString ImageProcessor::getImgInfoText()
{
    if (m_imgname.isEmpty())return "";
    return QString::asprintf("Frame=%d,W=%d,H=%d,C=%d,bitdepth=%d",
                             m_frames,m_width,m_height,m_channel,m_bitdepth);

}


void ImageProcessor::imglist2Mat()
{

    if(imgbox_list.isEmpty())return;
    for (int i=0;i<imgbox_list.length();i++)
    {
        ImageBox *imgbox=imgbox_list.at(i);
        QImage * imgp=imgbox->oriimg;
        Mat mat=QImage2cvMat(*imgp);
        imgbox->orimat=mat.clone();

        // to 255 gray
        Mat matgray=mat.clone();
        int c=mat.channels();
        if (c==3)
            cvtColor(matgray,matgray,cv::COLOR_BGR2GRAY);
        if(c==4)
            cvtColor(matgray,matgray,cv::COLOR_BGRA2GRAY);
        cv::normalize(matgray, matgray, 0, 255, cv::NORM_MINMAX);
        cv::convertScaleAbs(matgray, matgray);
        imgbox->graymat=matgray.clone();


    }

}

void ImageProcessor::ContrastAndBright(int brighter, int contract)
{

    if (imgbox_list.isEmpty()) return;
    for(int i=0;i<imgbox_list.length();i++)
    {
        ImageBox * imgbox=imgbox_list.at(i);
        Mat mat=imgbox->orimat;

        Mat newmat;
        ContrastAndBrightMat(mat,newmat,brighter,contract);


        imgbox->set_bc(newmat);

    }

}




void ImageProcessor::getContrastBrightHint(int & brighter, int & contrast)
{
    if (imgbox_list.isEmpty()) return;
    ImageBox * imgbox=imgbox_list.at(0);
    Mat mat=imgbox->orimat;
    double min_val=0,max_val=0;
    Scalar m=cv::mean(mat);
    double meann=m[0];
    cv::minMaxLoc(mat,&min_val,&max_val);
    if (mat.depth()==CV_8U)
    {
        contrast=std::numeric_limits<uint8_t>::max()/(max_val-min_val)*100;
        brighter=std::numeric_limits<uint8_t>::max()/2-meann*contrast/100;
    }
    else if (mat.depth()==CV_16U)
    {
        contrast=std::numeric_limits<uint16_t>::max()/(max_val-min_val)*100;
        brighter=std::numeric_limits<uint16_t>::max()/2-meann*contrast/100;
    }
}


//------------Mat process----------//
// only for 3-8bit or 1-8bit/16bit channel//
void ContrastAndBrightMat(const Mat &image, Mat &dst, int brighter, int constract)
{
    //执行运算 new_image(i,j) = alpha*image(i,j) + beta
    dst=image.clone();
    int channels=image.channels();
    if (channels==3)
    {
        for( int y = 0; y < image.rows; y++ )
        {
            for( int x = 0; x < image.cols; x++ )
            {
                for( int c = 0; c < channels; c++ )
                {
                    dst.at<Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( 0.01*constract*( image.at<Vec3b>(y,x)[c] ) + brighter );
                }
            }
        }
    }
    else if (channels==1)
    {
        if(image.type()==CV_8UC1)
        {
            for (int i = 0; i < image.rows; i++)
            {

                uint8_t *dataout = dst.ptr<uchar>(i);
                for (int j = 0; j <image.cols; j++) {

                    dataout[j] = cv::saturate_cast<uchar>(0.01*constract*dataout[j] + brighter);
                }
            }

        }
        else if(image.type()==CV_16UC1)
        {
            for (int i = 0; i < image.rows; i++)
            {

                uint16_t *dataout = (uint16_t*)dst.ptr<uchar>(i);
                for (int j = 0; j <image.cols; j++) {

                    dataout[j] = cv::saturate_cast<uint16_t>(0.01*constract*dataout[j] + brighter);
                }
            }
        }

    }

    //cv::imshow("contrast and bright",dst);


}
void AdaptiveThreshold(cv::Mat& src, cv::Mat& dst, double Maxval, int Subsize, double c, adaptiveMethod method ){

    if (src.channels() > 1)
        cv::cvtColor(src, src, CV_RGB2GRAY);
    //caculate noise

    Mat m,s;
    meanStdDev(src,m,s);
    double mean1=m.at<double>(0,0);
    double std1=s.at<double>(0,0);
    Mat mask=src.clone();
    cv::threshold(src,mask,3*std1+mean1,1,cv::THRESH_TOZERO_INV);
    meanStdDev(src,m,s,mask);
    double mean2=m.at<double>(0,0);
    double std2=s.at<double>(0,0);
    double back_threshold=mean1;

    //adaptive
    cv::Mat smooth;
    switch (method)
    {
    case  meanFilter:
        cv::blur(src, smooth, cv::Size(Subsize, Subsize));  //均值滤波
        break;
    case gaussianFilter:
        cv::GaussianBlur(src, smooth, cv::Size(Subsize, Subsize),0,0); //高斯滤波
        break;
    case medianFilter:
        cv::medianBlur(src, smooth, Subsize);   //中值滤波
        break;
    default:
        break;
    }

    smooth = smooth - c;

    //阈值处理
    src.copyTo(dst);
    for (int r = 0; r < src.rows;++r){
        const uchar* srcptr = src.ptr<uchar>(r);
        const uchar* smoothptr = smooth.ptr<uchar>(r);
        uchar* dstptr = dst.ptr<uchar>(r);
        for (int c = 0; c < src.cols; ++c){
            if (srcptr[c]>smoothptr[c] && srcptr[c]>back_threshold){
                dstptr[c] = Maxval;
            }
            else
                dstptr[c] = 0;
        }
    }

}
