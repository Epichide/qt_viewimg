#ifndef IMAGEREADER_H
#define IMAGEREADER_H



#include<iostream>


#include <QObject>
#include "tinytiffwriter.h"
#include "tinytiffreader.h"
#include "tinytiffreader.hxx"


#include<iostream>
#include <QImage>
#include <QMessageBox>
#include <QList>
#include <QImageReader>




enum DATATYPE{IMAGE8 ,IMAGE16};



class ImageReader : public QObject
{
    Q_OBJECT
public:
    explicit ImageReader(QObject *parent = nullptr);

signals:

public slots:

public:
    //    void writeITK(); //write work
    //    void readITK(); // read error will crash
    //TODO : get image xyz
    void readtifflib(std::string tifname="");//can read information, like xyztc , not header. cannot read iamge :error
    void readQtiff();
    void iamgeinfo(QString tifname,int &width,int &height,int &channel,int&frames,int &bitdepth);

    void loadImageList(QString tifname, QList<QImage> *imglist);
};

#endif // IMAGEREADER_H
