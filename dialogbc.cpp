#include "dialogbc.h"
#include "ui_dialogbc.h"

DialogBC::DialogBC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBC)
{
    ui->setupUi(this);
}

DialogBC::~DialogBC()
{
    delete ui;
}

void DialogBC::on_chk_auto_cnb_clicked(bool checked)
{
    emit autoBC_sig(checked);
}


void DialogBC::on_slid_brighter_valueChanged(int value)
{
    emit brighter_sig(value);
}


void DialogBC::on_slid_contrast_valueChanged(int value)
{
    emit contrast_sig(value);
}

void DialogBC::setBrighter(int value)
{
    ui->slid_brighter->setValue(value);
}

void DialogBC::setContrast(int value)
{
    ui->slid_contrast->setValue(value);
}

void DialogBC::setBrightRange(int minv, int maxv)
{
    ui->slid_brighter->setRange(minv,maxv);
}

void DialogBC::setContrastRange(int minv, int maxv)
{
    ui->slid_contrast->setRange(minv,maxv);
}

int DialogBC::getBrighter()
{
    return ui->slid_brighter->value();
}

int DialogBC::getContrast()
{
    return ui->slid_contrast->value();
}

void DialogBC::setslideEnabled(bool b)
{
    ui->slid_brighter->setEnabled(b);
    ui->slid_contrast->setEnabled(b);
    ui->chk_auto_cnb->setEnabled(b);
}

void DialogBC::closeEvent(QCloseEvent* event)
{
    this->hide();	 // 隐藏窗口
    event->ignore(); // 忽视原来的关闭事件
}
