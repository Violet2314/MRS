#ifndef LOGINREGISTER_H
#define LOGINREGISTER_H

#include <QWidget>
#include <QMouseEvent>

#include "direction.h"

namespace Ui {
class loginregister;
}

class loginregister : public QWidget
{
    Q_OBJECT

public:
    explicit loginregister(QWidget *parent = nullptr);
    ~loginregister();
    void region(const QPoint &currentGlobalPoint);  //鼠标的位置,改变光标
protected:
    //鼠标按下移动及释放事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void on_pushButton_3_pressed();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::loginregister *ui;

    QPoint m_movePoint;  //鼠标的位置
    bool isLeftPressDown;  // 判断左键是否按下
    Direction dir;        // 窗口大小改变时，记录改变方向
};

#endif // LOGINREGISTER_H
