#ifndef DIALOGBC_H
#define DIALOGBC_H
#include "stdxfm.h"

namespace Ui {
class DialogBC;
}

class DialogBC : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBC(QWidget *parent = nullptr);
    ~DialogBC();
signals:
    void brighter_sig(int brighter);
    void contrast_sig(int contrast);
    void autoBC_sig(bool checked);
private slots:
    void on_chk_auto_cnb_clicked(bool checked);

    void on_slid_brighter_valueChanged(int value);

    void on_slid_contrast_valueChanged(int value);
public :
    void setBrighter(int value);
    void setContrast(int value);
    void setBrightRange(int minv,int maxv);
    void setContrastRange(int minv,int maxv);
    int getBrighter();
    int getContrast();
    void setslideEnabled(bool );
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::DialogBC *ui;
};

#endif // DIALOGBC_H
