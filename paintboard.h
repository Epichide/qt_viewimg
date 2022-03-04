#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <QWidget>
#include <QGraphicsView>
#include "stdxfm.h"

#include <QRgb>


#include <QtGlobal>
#include <QLine>
#include "imagebox.h"
// cannot use  scene.clear
//Removes and deletes all items from the scene, but otherwise leaves the state of the scene unchanged
//Removes the item item and all its children from the scene. The ownership of item is passed on to the caller (i.e., QGraphicsScene will no longer delete item when destroyed)
//
class ImageProcessor;
class PaintBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PaintBoard(QWidget *parent = nullptr);


    QGraphicsScene *scene;
    QRect rect;
    QGraphicsPolygonItem *curitem=nullptr;
    QPointF firstpoint;
    QPointF curpoint;
    bool Is_draw=false;
    ImageProcessor * m_imgprocessor;
private:
    int m_maxX=std::numeric_limits<int>::max();
    int m_minX=0;
    int m_minY=0;
    int m_maxY=m_maxX;

    void ResetBorder();
    void CalcuBorder();

protected:


public slots:


public:
    void assignData( ImageBox* imgbox);
    void LoadData(ImageBox* imgbox);
    bool contour_show=true;
    bool triangle_show=false;
    bool spine_show=true;
    bool img_show=true;
    void showData();

    void CleanAll();
    void CleanResult();
    void clearCurrentitem();

signals:
    void sig_CurrentPos(int x, int y);
public:
    void setimage(QImage qimg);
    void setimage(QImage *qimg);
    QGraphicsPixmapItem * pixitem=nullptr;
    QGraphicsPixmapItem * pixitem_backup=nullptr;


protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

};


#endif // PAINTBOARD_H
