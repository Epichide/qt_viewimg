#include "paintboard.h"
#include "imageprocessor.h"
PaintBoard::PaintBoard(QWidget *parent)
    : QGraphicsView{parent}
{
    //QRect rect=geometry();
    scene=new QGraphicsScene(this);
    setScene(scene);
    scene->setSceneRect(0,0,width(),height());
    pixitem=new QGraphicsPixmapItem();
    pixitem_backup=new QGraphicsPixmapItem();
    scene->addItem(pixitem);
//    pixitem->setPos(-0.5,-0.5);
//    pixitem_backup->setPos(-0.5,-0.5);



}



/*↓↓↓↓↓↓↓ showing ↓↓↓↓↓↓↓↓↓*/
void PaintBoard::showData()
{

    setimage(pixitem->pixmap().toImage());
    //  scene->addItem(pixitem);

    pixitem->setVisible(img_show);

}

void PaintBoard::setimage(QImage qimg)
{
    if (pixitem==nullptr)pixitem=new QGraphicsPixmapItem();
    QPixmap pix=QPixmap::fromImage(qimg);
    pixitem_backup->setPixmap(pix);

    pixitem->setPixmap(pix);
    scene->setSceneRect(pixitem->boundingRect());

}

void PaintBoard::setimage(QImage *qimg)
{
    if (pixitem==nullptr)pixitem=new QGraphicsPixmapItem();
    if (qimg==nullptr)
    {
        pixitem->setPixmap(QPixmap());
        pixitem_backup->setPixmap(QPixmap());

    }
    else
    {

        QPixmap pix=QPixmap::fromImage(*qimg);
        pixitem->setPixmap(pix);
        pixitem_backup->setPixmap(pix);
        scene->setSceneRect(0,0,qimg->width(),qimg->height());
    }


}

/*↑↑↑↑↑↑↑↑↑ Showing ↑↑↑↑↑↑↑↑↑*/



/* ↓↓↓↓↓↓↓  update/ clean /reset  buffer   ↓↓↓↓↓↓↓ */
void PaintBoard::CleanAll()// reset all data including random point generated
{
    // reset
    CleanResult();


    // remove and destroy all item in scene
    scene->clear();
    pixitem=nullptr;


}

void PaintBoard::CleanResult()
{



}

void PaintBoard::clearCurrentitem()
{
    if(curitem!=nullptr)
    {
        scene->removeItem(curitem);

        delete curitem;
        curitem =nullptr;
    }

}
void PaintBoard::ResetBorder()
{
    m_minX=std::numeric_limits<int>::max();
    m_maxX=std::numeric_limits<int>::min();
    m_minY=m_minX;
    m_maxY=m_maxX;
}

void PaintBoard::CalcuBorder()
{
    ResetBorder();


}

/* ↓↓↓↓↓↓↓ MSg Event process ↓↓↓↓↓↓↓ ↓*/
void PaintBoard::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void PaintBoard::keyPressEvent(QKeyEvent *event)
{


    QGraphicsView::keyPressEvent(event);
}

void PaintBoard::keyReleaseEvent(QKeyEvent *event)
{

}

void PaintBoard::mouseDoubleClickEvent(QMouseEvent *event)
{

    curitem=nullptr;

}

void PaintBoard::mousePressEvent(QMouseEvent *event)
{


    QGraphicsView::mousePressEvent(event);
}

void PaintBoard::mouseMoveEvent(QMouseEvent *event)
{
    QPointF scene_p=mapToScene(event->pos());
    QPointF p = pixitem->mapFromScene(scene_p);

    emit sig_CurrentPos(p.x(),p.y());
    QGraphicsView::mouseMoveEvent(event);
}

void PaintBoard::mouseReleaseEvent(QMouseEvent *event)
{

    QGraphicsView::mouseReleaseEvent(event);
}



void PaintBoard::assignData(ImageBox *imgbox)
{
    //triangle
    imgbox->clearitems();


}

void PaintBoard::LoadData(ImageBox *imgbox)
{
    CleanAll();// clean all

    setimage( imgbox->bcimg);
    scene->addItem(pixitem);

}



