#define _SCL_SECURE_NO_WARNINGS
#include "imagereader.h"



ImageReader::ImageReader(QObject *parent) : QObject(parent)
{
    //itk::ObjectFactoryBase::RegisterFactory(itk::PNGImageIOFactory::New());
}






void ImageReader::iamgeinfo(QString tiffname, int &width, int &height, int &channel, int &frames, int &bitdepth)
{
    std::string tifname=tiffname.toStdString();

    TinyTIFFReaderFile* tiffr=NULL;
    tiffr=TinyTIFFReader_open(tifname.c_str());
    if (!tiffr)
    {
        std::cout<<"    ERROR reading (not existent, not accessible or no TIFF file)\n";
    }
    else
    {
        if (TinyTIFFReader_wasError(tiffr)) {
            std::cout<<"   ERROR:"<<TinyTIFFReader_getLastError(tiffr)<<"\n";
        }
        else
        {
            std::cout<<"    ImageDescription:\n"<< TinyTIFFReader_getImageDescription(tiffr) <<"\n";
            frames=TinyTIFFReader_countFrames(tiffr);
            std::cout<<"    frames: "<<frames<<"\n";
            if (TinyTIFFReader_wasError(tiffr)) {
                std::cout<<"   ERROR:"<<TinyTIFFReader_getLastError(tiffr)<<"\n";
            }
            else {

                width=TinyTIFFReader_getWidth(tiffr);
                height=TinyTIFFReader_getHeight(tiffr);
                channel=TinyTIFFReader_getSamplesPerPixel(tiffr);//channels
                bitdepth=TinyTIFFReader_getBitsPerSample(tiffr, 0);// bit depth 8 16...

            }
            std::cout<<"    width: "<<width<<"x"<<"height"<<height<<"\n";
            std::cout<<"    each pixel has "<<channel<<" samples with "<<bitdepth<<" bits each\n";
        }
        TinyTIFFReader_close(tiffr);

    }
}

void ImageReader::readQtiff()
{
    QImageReader reader;
    reader.setFileName("C:\\Users\\www_0\\Desktop\\Untitled.tif");

    int nCount = reader.imageCount();
    std::cout<<"image frame :"<<nCount;
    for (int i = 0; i < nCount; ++i)
    {
        // 跳到顺序号为i的图像
        bool ret = reader.jumpToImage(i);
        if (reader.canRead())
        {
            // 读取图像
            QImage image = reader.read();

            // 保存图像
            image.save(QString("Loading%1.tif").arg(i + 1));

            // 下一帧动画等待的毫秒数
            int nDelay = reader.nextImageDelay();
            std::cout<< QString("Number %1 Delay : %2").arg(i + 2).arg(nDelay).toStdString();
        }
        else
        {
            // 获取错误信息
            QImageReader::ImageReaderError error = reader.error();
            QString strError = reader.errorString();
            std::cout << "Last Error : " << strError.toStdString();
        }
    }

}
void ImageReader::loadImageList(QString tifname, QList<QImage> *imglist)
{
    QImageReader reader;
    reader.setFileName(tifname);

    int nCount = reader.imageCount();
    std::cout<<"image frame :"<<nCount;
    for (int i = 0; i < nCount; ++i)
    {
        // 跳到顺序号为i的图像
        bool ret = reader.jumpToImage(i);
        if (reader.canRead())
        {
            // 读取图像
            QImage image = reader.read();
            imglist->append(image);
            // 保存图像
            image.save(QString("Loading%1.tif").arg(i + 1));

            // 下一帧动画等待的毫秒数
            int nDelay = reader.nextImageDelay();
            std::cout<< QString("Number %1 Delay : %2").arg(i + 2).arg(nDelay).toStdString();
        }
        else
        {
            // 获取错误信息
            QImageReader::ImageReaderError error = reader.error();
            QString strError = reader.errorString();
            std::cout << "Last Error : " << strError.toStdString()<<std::endl;
        }
    }
    std::cout<<"img legth1"<<imglist->length()<<std::endl;
}

