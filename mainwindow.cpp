#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagereader.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lab_imgpath=new QLabel();
    lab_pospix=new QLabel();
    lab_imginfo=new QLabel();
    ui->statusbar->addWidget(lab_imgpath);
    ui->statusbar->addWidget(lab_imginfo);
    ui->statusbar->addWidget(lab_pospix);

    QGraphicsOpacityEffect *opacityEffect=new QGraphicsOpacityEffect;
    //lab_skele->setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(0.7);

    // bind signal and communication widget and processor
    connect(ui->gView,SIGNAL(sig_CurrentPos(int,int)),this,SLOT(slot_sig_CurrentPos(int,int)));
    imgprocessor.m_paintbord= ui->gView;
    ui->gView->m_imgprocessor=&imgprocessor;

    //lab_skele->setWindowOpacity(0.5);
    setMouseTracking(true);
    connect(dlgbc,SIGNAL(brighter_sig(int)),this,SLOT(slot_slid_brighter_valueChanged(int)));
    connect(dlgbc,SIGNAL(contrast_sig(int)),this,SLOT(slot_slid_contrast_valueChanged(int)));
    connect(dlgbc,SIGNAL(autoBC_sig(bool)),this,SLOT(slot_chk_auto_cnb_clicked(bool)));

        ui->dock_layer->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/* =↓=↓=↓=↓=↓=↓=  ZOOMING  =↓=↓=↓=↓=↓=↓= */
void MainWindow::on_actFit_Width_triggered()
{
    ui->gView->resetTransform();
    pixratio=ui->gView->width()/(ui->gView->scene->width()+30);
    ui->gView->scale(pixratio,pixratio);
}

void MainWindow::on_actFit_Height_triggered()
{
    ui->gView->resetTransform();
    pixratio=ui->gView->height()/(ui->gView->scene->height()+30);
    ui->gView->scale(pixratio,pixratio);
}
void MainWindow::on_actFit_Page_triggered()
{
    pixratio=std::min(ui->gView->height()/(ui->gView->scene->height()+30),ui->gView->width()/(ui->gView->scene->width()+30));
    ui->gView->resetTransform();
    ui->gView->scale(pixratio,pixratio);

}
void MainWindow::on_actZoomRealSize_triggered()
{
    pixratio=1;  //恢复显示比例为1
    ui->gView->resetTransform();
}

void MainWindow::on_actZoomIn_triggered()
{

    ui->gView->scale(1.1,1.1);
}

void MainWindow::on_actZoomOut_triggered()
{
    ui->gView->scale(.9,.9);

}

/* =↓=↓=↓=↓=↓=↓=  DOCKWIDGET  =↓=↓=↓=↓=↓=↓= */
void MainWindow::on_dock_layer_visibilityChanged(bool visible)
{
    ui->actDockerLay->setChecked(visible);
}


void MainWindow::on_actDockerLay_triggered(bool checked)
{
    ui->dock_layer->setVisible(checked);
}


void MainWindow::on_actOpenFile_triggered()
{
    QString filter="Images(*.jpg *.tiff *.tif *.png *.jpeg)";
    QString imgname=QFileDialog::getOpenFileName(this,"choose image","",filter);
    if (imgname.isEmpty()) return;

    imgprocessor.loadFromFile(imgname);
    //ui->statusbar->showMessage(imgname+imgprocessor.getImgInfoText());
    int frames=imgprocessor.m_frames;
    ui->slider_frame->setMaximum(frames);
    ui->slider_frame->setMinimum(1);
    if (frames>1)
    {

        ui->slider_frame->setEnabled(true);
        ui->slider_frame->setValue(1);
    }
    else
    {
        ui->slider_frame->setEnabled(false);
    }
    JumpToFrame(1);
    ImageBox *imgbox=imgprocessor.imgbox_list.at(0);

    ui->gView->setimage(imgbox->bcimg);
    lab_imgpath->setText(imgname);

   on_ImgOpened_triggered();

    on_actZoomRealSize_triggered();//update show
    //printGeometry();
    int brighter;
    int contrast;
    imgprocessor.getContrastBrightHint(brighter, contrast);
    //dlgbc->setBrightRange(-brighter*2,2*brighter);
    //dlgbc->setContrastRange(0,max(contrast,200)*2);


}

/* =↓=↓=↓=↓=↓=↓=  Bright and Contrast  =↓=↓=↓=↓=↓=↓= */
void MainWindow::slot_slid_contrast_valueChanged(int value)
{
      imgprocessor.ContrastAndBright(dlgbc->getBrighter(),dlgbc->getContrast());

        JumpToFrame(ui->slider_frame->value());
}

void MainWindow::slot_slid_brighter_valueChanged(int value)
{
    imgprocessor.ContrastAndBright(dlgbc->getBrighter(),dlgbc->getContrast());
JumpToFrame(ui->slider_frame->value());
}

void MainWindow::slot_chk_auto_cnb_clicked(bool checked)
{
    //auto bright & contrast
    if (imgprocessor.imgbox_list.isEmpty())return;
    int brighter;
    int contrast;
    imgprocessor.getContrastBrightHint(brighter, contrast);
    dlgbc->setBrightRange(-brighter*2-100,2*brighter+100);
    dlgbc->setContrastRange(0,std::max(contrast,200)*3);
    if (checked)
    {
        dlgbc->setBrighter(brighter);
        dlgbc->setContrast(contrast);
        imgprocessor.ContrastAndBright(brighter,contrast);

    }
    else
    {
        imgprocessor.ContrastAndBright(0,100);
        dlgbc->setBrighter(0);
        dlgbc->setContrast(100);
    }
    JumpToFrame(ui->slider_frame->value());


}



/* =↓=↓=↓=↓=↓=↓=  Multi Frame Jump  =↓=↓=↓=↓=↓=↓= */
void MainWindow::on_slider_frame_valueChanged(int value)
{
    if (value<= imgprocessor.imgbox_list.length())
    {
        JumpToFrame(value);
       // on_actZoomRealSize_triggered();
    }

}

void MainWindow::JumpToFrame(int idx)
{
    //std::cout<<"img length2 "<<imglist.length();
    // idx from 1 to totol frame
    if (idx>imgprocessor.imgbox_list.length()) return;
    imgprocessor.JumpTo(idx-1);

    // update current frame showing widget
    QString strinfo=QString::asprintf("%d/%d",idx,imgprocessor.imgbox_list.length());
    ui->lab_curframe->setText(strinfo);



}

/* =↓=↓=↓=↓=↓=↓=  Layer Show  =↓=↓=↓=↓=↓=↓= */
void MainWindow::on_cb_border_clicked(bool checked)
{
    ui->gView->img_show=checked;
    ui->gView->showData();

}

void MainWindow::on_cb_ori_clicked(bool checked)
{
    ui->gView->img_show=checked;
     ui->gView->showData();
    //ui->lab_img->setVisible(checked);
}



//------------IMage Open and close------//

void MainWindow::on_actCloseImg_triggered()
{
    ui->actZoomIn->setEnabled(false);
    ui->actZoomOut->setEnabled(false);
    ui->actZoomRealSize->setEnabled(false);
    ui->slider_frame->setEnabled(false);
    ui->lab_curframe->setText("");
    ui->actFit_Height->setEnabled(false);
    ui->actFit_Page->setEnabled(false);
    ui->actFit_Width->setEnabled(false);
    ui->gView->setimage(nullptr);
    //imgprocessor.clearBuffImg();
    curimg=nullptr;

    //clear pixmap
//    ui->lab_img->clear();
//    lab_skele->clear();
}

void MainWindow::on_ImgOpened_triggered()
{
    ui->actZoomIn->setEnabled(true);
    ui->actZoomOut->setEnabled(true);
    ui->actZoomRealSize->setEnabled(true);
    ui->actFit_Height->setEnabled(true);
    ui->actFit_Page->setEnabled(true);
    ui->actFit_Width->setEnabled(true);
    dlgbc->setslideEnabled(true);


}

//--------------Show image pixel information------//
void MainWindow::slot_sig_CurrentPos(int x, int y)
{
    QString str;
    QString formatinfo;
    if(imgprocessor.imgbox_list.isEmpty())return;


    //int channel;//=imgprocessor.m_channel;
    ImageBox * imgbox=imgprocessor.imgbox_list.at(ui->slider_frame->value()-1);
    curimg=imgbox->oriimg;

    bool allgray=curimg->allGray();

    int width=curimg->width();
    int  depth=curimg->depth();//channel *bitdepth
    int useddepth=curimg->bitPlaneCount(); //how much bit used , if not use transparency ,will need 32 space ,but 24 used
    int bitdepth=imgprocessor.m_bitdepth;//8 16

//    if (allgray)
//        channel=1;
//    else if (depth %3==0)//rgb
//        channel=3;
//    else
//        channel=4;
    int channel=depth/bitdepth;



    const int bytes = (depth + 7) / 8;
    QString colorinfo;
    const char* addr=(const char*)(curimg->bits())
            + (size_t)y * curimg->bytesPerLine()
            + (size_t)(x * depth + 7) / 8;

    if(x<0 || x>curimg->width())return;
    if(y<0 || y>curimg->height())return;
    if (bitdepth==16)
    {
        formatinfo="16bit";
        quint16* add=(quint16*)addr;
        for (int i=0;i<channel;i++)
        {
            QString str=QString::asprintf(",%d",add[i]);
            colorinfo+=str;
        }
    }
    else if (bitdepth==8)
    {
        formatinfo="8bit";
        quint8* add=(quint8*)addr;
        for (int i=0;i<channel;i++)
        {
            QString str=QString::asprintf(",%d",add[i]);
            colorinfo+=str;
        }
    }
    else if(bitdepth==32)
    {
        formatinfo="8bit";
        quint8* add=(quint8*)addr;
        for (int i=0;i<channel;i++)
        {
            QString str=QString::asprintf(",%d",add[i]);
            colorinfo+=str;
        }
    }
formatinfo+=QString::asprintf(", c = %d ",channel);

    str=QString::asprintf("x=%d,y=%d,value=",x,y)+colorinfo;
    lab_imginfo->setText(formatinfo);

    lab_pospix->setText(str);
}

// TO DO
void MainWindow::on_actB_C_triggered()
{

    dlgbc->show();

}






