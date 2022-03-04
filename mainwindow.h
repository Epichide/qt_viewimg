#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stdxfm.h"
#include "imageprocessor.h"
#include <QRgb>
#include "dialogbc.h"
#include"paintboard.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPixmap curPixmap; //当前的图片
    QImage * curimg;
    QPixmap curskele;
    DialogBC * dlgbc=new DialogBC(); //Bright and Contrast dialog
    float pixRatio;
private slots:
    void on_dock_layer_visibilityChanged(bool visible);


    void on_actDockerLay_triggered(bool checked);


    void on_actOpenFile_triggered();

    void on_actZoomRealSize_triggered();



    void on_slider_frame_valueChanged(int value);

    void on_actZoomIn_triggered();

    void on_actZoomOut_triggered();

    void on_actCloseImg_triggered();

    void on_cb_ori_clicked(bool checked);


    void on_actFit_Width_triggered();

    void on_actFit_Height_triggered();

    void slot_sig_CurrentPos(int x,int y);



    void slot_slid_contrast_valueChanged(int value);

    void slot_slid_brighter_valueChanged(int value);

    void slot_chk_auto_cnb_clicked(bool checked);


    void on_actB_C_triggered();

    void on_cb_border_clicked(bool checked);



    void on_actFit_Page_triggered();

protected:


private:
    void imgZoomFitW();
    void on_ImgOpened_triggered();

    //void printGeometry();
    Ui::MainWindow *ui;

    ImageProcessor imgprocessor;


    void JumpToFrame(int idx);
    //layer

    QLabel *lab_skele;
    QLabel *lab_spine;
    double pixratio=1.0;

    //-----Info widget------//
    QLabel *lab_imgpath;
    QLabel *lab_pospix;
    QLabel *lab_imginfo;


};
#endif // MAINWINDOW_H
