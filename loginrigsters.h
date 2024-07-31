#ifndef LOGINRIGSTERS_H
#define LOGINRIGSTERS_H

#include <QWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include "direction.h"
#include "sql.h"

namespace Ui {
class loginrigsters;
}

class loginrigsters : public QWidget
{
    Q_OBJECT

public:
    explicit loginrigsters(QWidget *parent = nullptr);
    ~loginrigsters();
    void region(const QPoint &currentGlobalPoint);  //鼠标的位置,改变光标
    void setSQL(sql& SQL);
signals:
    void loginSuccessful(int userId,QString username);
protected:
    //鼠标按下移动及释放事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::loginrigsters *ui;

    QPoint m_movePoint;  //鼠标的位置
    bool isLeftPressDown;  // 判断左键是否按下
    Direction dir;        // 窗口大小改变时，记录改变方向
    sql* SQL;
};

#endif // LOGINRIGSTERS_H
