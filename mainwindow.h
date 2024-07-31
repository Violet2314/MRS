#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMouseEvent>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QDir>
#include <QAudiooutput>
#include <QTimer>
#include <vector>
#include <set>
#include <QMessageBox>
#include "sql.h"

#include "direction.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void region(const QPoint &currentGlobalPoint);  //鼠标的位置,改变光标
    void settime();
    void setSlider();
    QString gettime(qint64 duration);
    void setname();
    void setVol();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void getnum(int n);

    void setuid(int uid,QString username);
    void setSQL(sql& SQL);

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_12_pressed();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();
    void on_pushButton_10_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButton_17_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_35_clicked();

    void on_tableWidget_2_cellDoubleClicked(int row, int column);

    void on_pushButton_21_clicked();

protected:
    //鼠标按下移动及释放事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;

    QPoint m_movePoint;  //鼠标的位置
    bool isLeftPressDown;  // 判断左键是否按下
    Direction dir;        // 窗口大小改变时，记录改变方向

    QAudioOutput* audiooutput = nullptr;//连接输出设备
    QMediaPlayer* mediaPlayer = nullptr;//音乐播放媒体

    QStringList* list = new QStringList();//音乐列表
    QStringList* list2 = new QStringList();
    bool can;

    QString file = "qrc:/new/prefix1/Music/";
    int now = -1;//现在在的音乐下表
    int size;
    int num = 0;

    std::set<int> sett;
    //随机播放
    std::vector<int> rpl;//随机的列表
    int rplpos;//随机列表的位置

    std::vector<QString> mylovename;//我喜欢的音乐
    std::vector<int> myloveid;
    std::pmr::unordered_map<QString,bool> islove;

    //字幕滚动播放
    QTimer *timer;
    int scrollPos;//字幕滚动
    QString originalText;

    int uid;
    QString username;
    sql* SQL;
};

#endif // MAINWINDOW_H
