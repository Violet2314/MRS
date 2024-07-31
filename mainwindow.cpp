#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <iostream>
#include <QMediaMetaData>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    scrollPos(0)
{
    ui->setupUi(this);

    //设置日期
    /*QDateTime dateTime = QDateTime::currentDateTime();
    QString str = dateTime.date();*/
    QDate date(QDate::currentDate());
    int year = date.year();
    int month = date.month();
    int day = date.day();
    ui->label_12->setText(QString::number(day));
    ui->label_13->setText("/ " + QString::number(month));

    // 去掉标题栏,去掉工具栏，窗口置顶
    setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowStaysOnTopHint);// | Qt::Tool | Qt::WindowStaysOnTopHint
    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明化
    //setWindowOpacity(0.7); //设置窗体透明度

    //加窗体标注
    ui->pushButton_2->setToolTip("搜索");
    ui->pushButton_9->setToolTip("隐藏窗口");
    ui->pushButton_8->setToolTip("最大化");
    ui->pushButton_7->setToolTip("关闭");

    // 设置最后一行自动扩展
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    // 设置所有行自动调整大小
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置不能被编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置最后一行自动扩展
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    // 设置所有行自动调整大小
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置不能被编辑
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //连接输出硬件
    mediaPlayer = new QMediaPlayer(this);
    audiooutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audiooutput);
    ui->horizontalSlider_2->setRange(0,100);
    audiooutput->setVolume(0.5);
    setVol();


    //设置真在播放的歌曲标题的滚动播放
    //ui->label_9->setWordWrap(true);
    QObject::connect(timer, &QTimer::timeout, this, [this](){//QObject::加不加都无所谓
        scrollPos += 1;
        if (scrollPos >= originalText.length()) {
            scrollPos = 0;
        }
        QString displayedText = originalText.mid(scrollPos) + originalText.left(scrollPos);
        ui->label_9->setText(displayedText);
    });
    timer->start(300);  // 设置滚动速度

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);//设置专辑图片
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_7_clicked()
{
    this->close();
}

void MainWindow::setuid(int uid,QString username){
    this->uid = uid;
    this->username = username;
    ui->label_11->setText("你好，"+username);
    ui->pushButton_3->setText(username);
    ui->pushButton_23->setText(username);

    myloveid = SQL->getloveid(uid);
    mylovename = SQL->getlovename(myloveid);
    //qDebug() << mylovename.size();
    for(auto it : mylovename){
        islove[it] = true;
    }
}

void MainWindow::setSQL(sql& SQL){
    this->SQL = &SQL;
}

void MainWindow::on_pushButton_8_clicked()
{
    static bool isMaximized = false; // 用于记录窗口状态

    if (isMaximized) {
        // 窗口恢复到默认大小
        this->showNormal();
        ui->pushButton_8->setIcon(QIcon("://images/full-screen.png"));
        ui->pushButton_8->setToolTip("最大化");
    } else {
        // 窗口最大化
        this->showMaximized();
        ui->pushButton_8->setIcon(QIcon("://images/small-screen-white.png"));
        ui->pushButton_8->setToolTip("最小化");
    }

    isMaximized = !isMaximized;
}



void MainWindow::on_pushButton_9_clicked()
{
    this->hide();
}


void MainWindow::on_pushButton_12_pressed()
{

}


void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::region(const QPoint &currentGlobalPoint)
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
void MainWindow::mousePressEvent(QMouseEvent *event)
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
        MainWindow::mousePressEvent(event);
    }
}



//鼠标移动事件
void MainWindow::mouseMoveEvent(QMouseEvent *event)
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
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
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




void MainWindow::on_pushButton_10_clicked()
{
    can = false;

    ui->tableWidget->setRowCount(mylovename.size());
    QTableWidget* noow = ui->tableWidget;
    for(int i = 0; i < mylovename.size(); i++){
        //list->push_back(dir.absoluteFilePath(files[i].fileName()));
        noow->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::to_string(i+1))));
        noow->setItem(i,1,new QTableWidgetItem(mylovename[i]));
    }
    ui->label_6->setText("我喜欢的音乐");
    ui->stackedWidget->setCurrentIndex(1);

    if(now == -1 && !mylovename.empty()){
        now = 0;
        list->clear();
        for(auto it : mylovename){
            list->append(it);
        }
        //QUrl::fromLocalFile是用来读取不在qt资源文件里面的，就是用户选择的，如果是自己qt项目里面的资源就用qrc就可以了
        QString musicFilePath = mylovename[now];
        std::cout << musicFilePath.toStdString() << "\n";
        mediaPlayer->setSource(QUrl(file + musicFilePath));
        settime();
        setSlider();//不能放在status == QMediaPlayer::LoadedMedia里面
        setname();
    }
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if(can == false){
        can = true;
        list->clear();
        for(auto it : mylovename){
            list->append(it);
        }
        size = list->size();
    }
    now = row;
    //QUrl::fromLocalFile是用来读取不在qt资源文件里面的，就是用户选择的，如果是自己qt项目里面的资源就用qrc就可以了
    QString musicFilePath = list->at(now);

    ui->pushButton_21->setIcon(QIcon("://images/favorite-big-white.png"));
    ui->pushButton_21->setIconSize(QSize(18, 41));

    //std::cout << musicFilePath.toStdString() << "\n";
    mediaPlayer->setSource(QUrl(file + musicFilePath));

    setname();

    mediaPlayer->play();
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia) {
        QMediaMetaData metaData = mediaPlayer->metaData();//可以查看详细信息
        QVariant coverArt = metaData.value(QMediaMetaData::CoverArtImage);
        QString title = metaData.stringValue(QMediaMetaData::Title);
        //std::cout << "Title: " << title.toStdString() << "\n";

        if (coverArt.isValid()) {
            QImage image = qvariant_cast<QImage>(coverArt);// ? 不知道为啥总是设置不了
            //ui->label_7->setPixmap(QPixmap::fromImage(image).scaled(ui->label_7->size(), Qt::KeepAspectRatio));
        } else {
            //ui->label_7->setText("No album art found");
        }

        // 断开信号连接，避免重复处理
        disconnect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
    }
}

void MainWindow::on_pushButton_17_clicked()
{
    QMediaPlayer::PlaybackState state = mediaPlayer->playbackState();
    switch (state) {
    case QMediaPlayer::PlayingState:
        mediaPlayer->pause();
        ui->pushButton_17->setIcon(QIcon("://images/stop.png"));
        break;
    case QMediaPlayer::PausedState:
        mediaPlayer->play();
        ui->pushButton_17->setIcon(QIcon("://images/pause.png"));
        break;
    case QMediaPlayer::StoppedState:
        //停止状态
        break;
    }
}


void MainWindow::on_pushButton_16_clicked()
{
    getnum(1);
    QString musicFilePath = list->at(now);
    if(islove[musicFilePath] == true){
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big-white.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }else{
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }
    //std::cout << musicFilePath.toStdString();
    mediaPlayer->setSource(QUrl(file + musicFilePath));
    mediaPlayer->play();
    setname();
}


void MainWindow::on_pushButton_18_clicked()
{
    getnum(-1);
    QString musicFilePath = list->at(now);
    if(islove[musicFilePath] == true){
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big-white.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }else{
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }
    //std::cout << musicFilePath.toStdString();
    mediaPlayer->setSource(QUrl(file + musicFilePath));
    mediaPlayer->play();
    setname();
}

void MainWindow::settime(){
    QObject::connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::LoadedMedia) {
            qint64 duration = mediaPlayer->duration();
            ui->label_5->setText(gettime(duration));
            //setSlider();不能放在这里来，这样的话会在第一次初始化的时候出问题
        }
    });
}

void MainWindow::setSlider(){
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        if (!ui->horizontalSlider->isSliderDown()) {  // 检查滑块是否被用户拖动
            ui->horizontalSlider->setValue(position);
            ui->label_4->setText(gettime(position));//跟踪显示
        }
    });
    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, [this](int position) {
        mediaPlayer->setPosition(position);
    });
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->horizontalSlider->setRange(0, duration);
        //std::cout << duration << "\n";
    });
}

void MainWindow::setVol(){
    ui->horizontalSlider_2->setValue(0.5 * 100);
    connect(ui->horizontalSlider_2, &QSlider::sliderMoved, this, [this](int position) {
        float n = (float)position/100.0;
        audiooutput->setVolume(n);
    });
    connect(audiooutput, &QAudioOutput::volumeChanged, this, [this](float n) {
        if (!ui->horizontalSlider_2->isSliderDown()) {  // 检查滑块是否还在被用户拖动
            int position = n * 100;
            ui->horizontalSlider_2->setValue(position);
        }
    });
}

QString MainWindow::gettime(qint64 duration){
    qint64 seconds = duration / 1000;
    qint64 minn = seconds/60;
    seconds = seconds%60;
    QString time = "";
    if(minn < 10){
        time += "0";
    }
    time += QString::number(minn);
    time += ":";
    if(seconds < 10){
        time += "0";
    }
    time += QString::number(seconds);
    return time;
}

void MainWindow::setname(){
    ui->label_9->setText(list->at(now));
    originalText = ui->label_9->text();
    scrollPos = 0;  // 重置滚动位置
}
void MainWindow::on_pushButton_20_clicked()
{
    num += 1;
    //std::cout << num << "\n";
    if(num == 4) num = 0;
    if(num == 0){
        ui->pushButton_20->setText("顺序播放");
    }else if(num == 1){
        ui->pushButton_20->setText("随机播放");

        //生成随机数列
        rpl.clear();
        sett.clear();
        std::random_device rd;
        //unsigned int seed = rd();
        std::mt19937 mt_r(rd());
        std::uniform_int_distribution<> distrib(4, 10);
        int size = distrib(mt_r);
        std::uniform_int_distribution<> distrib2(0, list->size()-1);
        std::cout << "Size: " << size << "\n";
        while(sett.size() != size){
            int pos = distrib2(mt_r);
            sett.insert(pos);
        }
        for(auto it : sett){
            //std::cout << it << "\n";
            rpl.push_back(it);
        }
        rplpos = 0;

    }else if(num == 2){
        ui->pushButton_20->setText("定向推荐");
    }else if(num == 3){
        ui->pushButton_20->setText("漫游推荐");
    }
}

void MainWindow::getnum(int n){
    switch (num) {
    case 0:
        if(n == 1) now = (now+1) % size;
        else{
            now = now - 1;
            if(now == -1) now = size - 1;
        }
        break;
    case 1:
        if(n == 1){
            now = rpl[rplpos++];
            if(rplpos == rpl.size()) rplpos = 0;
        }else{
            now = rpl[rplpos--];
            if(rplpos == -1) rplpos = rpl.size()-1;
        }
        break;
    }
}

void MainWindow::on_pushButton_35_clicked()
{
    can = false;

    list2->clear();
    QDir dir(QString(":/new/prefix1/Music/"));
    QFileInfoList files = dir.entryInfoList(QStringList() << "*.mp3" << "*.wav" << "*.flac", QDir::Files);
    *list2 = dir.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac", QDir::Files);
    ui->tableWidget_2->setRowCount(files.size());

    QTableWidget* noow = ui->tableWidget_2;
    for(int i = 0; i < files.size(); i++){
        //list2->push_back(dir.absoluteFilePath(files[i].fileName()));
        noow->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::to_string(i+1))));
        noow->setItem(i,1,new QTableWidgetItem(files[i].fileName()));
    }
    ui->stackedWidget->setCurrentIndex(2);
    if(now == -1){
        list = list2;
        can = true;
        size = list->size();

        now = 0;
        //QUrl::fromLocalFile是用来读取不在qt资源文件里面的，就是用户选择的，如果是自己qt项目里面的资源就用qrc就可以了
        QString musicFilePath = list->at(now);
        std::cout << musicFilePath.toStdString() << "\n";
        mediaPlayer->setSource(QUrl(file + musicFilePath));
        settime();
        setSlider();//不能放在status == QMediaPlayer::LoadedMedia里面
        setname();
    }
}


void MainWindow::on_tableWidget_2_cellDoubleClicked(int row, int column)
{
    if(can == false){
        can = true;
        list->clear();
        *list = *list2;
        size = list->size();
    }
    now = row;
    //QUrl::fromLocalFile是用来读取不在qt资源文件里面的，就是用户选择的，如果是自己qt项目里面的资源就用qrc就可以了
    QString musicFilePath = list->at(now);
    if(islove[musicFilePath] == true){
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big-white.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }else{
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }
    //std::cout << musicFilePath.toStdString() << "\n";
    mediaPlayer->setSource(QUrl(file + musicFilePath));

    setname();

    mediaPlayer->play();
}


void MainWindow::on_pushButton_21_clicked()
{
    QString musicFilePath = list->at(now);
    if(islove[musicFilePath] == true){
        islove[musicFilePath] = false;
        mylovename.erase(std::remove(mylovename.begin(), mylovename.end(), musicFilePath), mylovename.end());
        ui->tableWidget->setRowCount(mylovename.size());
        int musicid = SQL->getmusicid(musicFilePath);
        SQL->deletelove(uid,musicid);
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }else{
        islove[musicFilePath] = true;
        mylovename.push_back(musicFilePath);
        ui->tableWidget->setRowCount(mylovename.size());
        int musicid = SQL->getmusicid(musicFilePath);
        SQL->uploadlove(uid,musicid);
        ui->pushButton_21->setIcon(QIcon("://images/favorite-big-white.png"));
        ui->pushButton_21->setIconSize(QSize(18, 41));
    }
}

