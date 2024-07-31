#include "loginrigsters.h"
#include "ui_loginrigsters.h"

loginrigsters::loginrigsters(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginrigsters)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowStaysOnTopHint);// | Qt::Tool | Qt::WindowStaysOnTopHint
    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明化

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
    ui->lineEdit_5->setEchoMode(QLineEdit::Password);
}

loginrigsters::~loginrigsters()
{
    delete ui;
}

void loginrigsters::region(const QPoint &currentGlobalPoint)
{
    // 获取窗体在屏幕上的位置区域，topLeft为坐上角点，rightButton为右下角点
    QRect rect = this->rect();

    QPoint topLeft = this->mapToGlobal(rect.topLeft()); //将左上角的(0,0)转化为全局坐标
    QPoint rightButton = this->mapToGlobal(rect.bottomRight());

    int x = currentGlobalPoint.x(); //当前鼠标的坐标
    int y = currentGlobalPoint.y();

    if(((topLeft.x() + PADDING >= x) && (topLeft.x() <= x))
        && ((topLeft.y() + PADDING >= y) && (topLeft.y() <= y)))
    {
        // 左上角
        dir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置光标形状
    }else if(((x >= rightButton.x() - PADDING) && (x <= rightButton.x()))
               && ((y >= rightButton.y() - PADDING) && (y <= rightButton.y())))
    {
        // 右下角
        dir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }else if(((x <= topLeft.x() + PADDING) && (x >= topLeft.x()))
               && ((y >= rightButton.y() - PADDING) && (y <= rightButton.y())))
    {
        //左下角
        dir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }else if(((x <= rightButton.x()) && (x >= rightButton.x() - PADDING))
               && ((y >= topLeft.y()) && (y <= topLeft.y() + PADDING)))
    {
        // 右上角
        dir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }else if((x <= topLeft.x() + PADDING) && (x >= topLeft.x()))
    {
        // 左边
        dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if((x <= rightButton.x()) && (x >= rightButton.x() - PADDING))
    {
        // 右边
        dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if((y >= topLeft.y()) && (y <= topLeft.y() + PADDING))
    {
        // 上边
        dir = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else if((y <= rightButton.y()) && (y >= rightButton.y() - PADDING))
    {
        // 下边
        dir = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else
    {
        // 默认
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
//三个鼠标事件的重写
//鼠标按下事件
void loginrigsters::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        isLeftPressDown = true;

        if(dir != NONE)
        {
            this->mouseGrabber(); //返回当前抓取鼠标输入的窗口
        }
        else
        {
            m_movePoint = event->globalPos() - this->frameGeometry().topLeft();
            //globalPos()鼠标位置，topLeft()窗口左上角的位置
        }
        break;
    case Qt::RightButton:
        this->setWindowState(Qt::WindowMinimized);
        break;
    default:
        loginrigsters::mousePressEvent(event);
    }
}
//鼠标移动事件
void loginrigsters::mouseMoveEvent(QMouseEvent *event)
{
    QPoint globalPoint = event->globalPos();   //鼠标全局坐标

    QRect rect = this->rect();  //rect == QRect(0,0 1280x720)
    QPoint topLeft = mapToGlobal(rect.topLeft());
    QPoint bottomRight = mapToGlobal(rect.bottomRight());

    if (this->windowState() != Qt::WindowMaximized)
    {
        if(!isLeftPressDown)  //没有按下左键时
        {
            this->region(globalPoint); //窗口大小的改变——判断鼠标位置，改变光标形状
        }
        else
        {
            if(dir != NONE)
            {
                QRect newRect(topLeft, bottomRight); //定义一个矩形  拖动后最大1000*1618

                switch(dir)
                {
                case LEFT:

                    if(bottomRight.x() - globalPoint.x() <= this->minimumWidth())
                    {
                        newRect.setLeft(topLeft.x());  //小于界面的最小宽度时，设置为左上角横坐标为窗口x
                        //只改变左边界
                    }
                    else
                    {
                        newRect.setLeft(globalPoint.x());
                    }
                    break;
                case RIGHT:
                    newRect.setWidth(globalPoint.x() - topLeft.x());  //只能改变右边界
                    break;
                case UP:
                    if(bottomRight.y() - globalPoint.y() <= this->minimumHeight())
                    {
                        newRect.setY(topLeft.y());
                    }
                    else
                    {
                        newRect.setY(globalPoint.y());
                    }
                    break;
                case DOWN:
                    newRect.setHeight(globalPoint.y() - topLeft.y());
                    break;
                case LEFTTOP:
                    if(bottomRight.x() - globalPoint.x() <= this->minimumWidth())
                    {
                        newRect.setX(topLeft.x());
                    }
                    else
                    {
                        newRect.setX(globalPoint.x());
                    }

                    if(bottomRight.y() - globalPoint.y() <= this->minimumHeight())
                    {
                        newRect.setY(topLeft.y());
                    }
                    else
                    {
                        newRect.setY(globalPoint.y());
                    }
                    break;
                case RIGHTTOP:
                    if (globalPoint.x() - topLeft.x() >= this->minimumWidth())
                    {
                        newRect.setWidth(globalPoint.x() - topLeft.x());
                    }
                    else
                    {
                        newRect.setWidth(bottomRight.x() - topLeft.x());
                    }
                    if (bottomRight.y() - globalPoint.y() >= this->minimumHeight())
                    {
                        newRect.setY(globalPoint.y());
                    }
                    else
                    {
                        newRect.setY(topLeft.y());
                    }
                    break;
                case LEFTBOTTOM:
                    if (bottomRight.x() - globalPoint.x() >= this->minimumWidth())
                    {
                        newRect.setX(globalPoint.x());
                    }
                    else
                    {
                        newRect.setX(topLeft.x());
                    }
                    if (globalPoint.y() - topLeft.y() >= this->minimumHeight())
                    {
                        newRect.setHeight(globalPoint.y() - topLeft.y());
                    }
                    else
                    {
                        newRect.setHeight(bottomRight.y() - topLeft.y());
                    }
                    break;
                case RIGHTBOTTOM:
                    newRect.setWidth(globalPoint.x() - topLeft.x());
                    newRect.setHeight(globalPoint.y() - topLeft.y());
                    break;
                default:
                    break;
                }
                this->setGeometry(newRect);
            }
            else
            {
                move(event->globalPos() - m_movePoint); //移动窗口
                event->accept();
            }
        }
    }
}
//鼠标释放事件
void loginrigsters::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isLeftPressDown = false;
        if (dir != NONE)
        {
            this->releaseMouse(); //释放鼠标抓取
            this->setCursor(QCursor(Qt::ArrowCursor));
            dir = NONE;
        }
    }
}


void loginrigsters::on_pushButton_3_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    int so = SQL->loginsql(username,password);
    if(so != -1){
        qDebug() << "Login successful for user ID:" << so;
        this->close();
        emit loginSuccessful(so,username);
    }else{
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
    /*MainWindow *w = new MainWindow();
    w->show();
    this->close();*/
    //这段代码创建了一个新的MainWindow对象，并在堆上分配内存。这样可以确保MainWindow不会在函数结束时被销毁，从而正确显示MainWindow。
/*
    void loginrigsters::on_pushButton_3_clicked()
    {
        MainWindow w;
        w.show();
        this->close();
    }
    这样创建的是局部变量会在函数结束的时候被销毁
*/
}

void loginrigsters::on_pushButton_5_clicked()
{
    QString username = ui->lineEdit_3->text();
    QString password = ui->lineEdit_4->text();
    QString password_2 = ui->lineEdit_5->text();
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    if(password != password_2){
        QMessageBox::warning(this, "注册失败", "两次输入的密码不同.");
    }
    bool can = SQL->registersql(username,password);
    if(can){
        QMessageBox::information(this, "注册成功", "注册成功");
        ui->stackedWidget->setCurrentIndex(0);
    }else{
        QMessageBox::warning(this,"注册失败", "用户名已经被注册.");
    }
}



void loginrigsters::on_pushButton_4_clicked()
{
    this->close();
}


void loginrigsters::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void loginrigsters::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void loginrigsters::setSQL(sql& SQL){
    this->SQL = &SQL;
}

