#ifndef PIC_H
#define PIC_H
#include "ImageSliderLabel.h"

#include <QWidget>

namespace Ui {
class Pic;
}

class Pic : public QWidget
{
    Q_OBJECT

public:
    explicit Pic(QWidget *parent = nullptr);
    ~Pic();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Pic *ui;
    ImageSliderLabel* imageSliderLabel;
};

#endif // PIC_H
