#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QSizePolicy>
#include <QMessageBox>
#include <QTime>
#include <QFileDialog>
#include "boardwidget.h"

GameWidget::GameWidget(QWidget *parent, MainWindow *mainWindow):
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    this->mainWindow = mainWindow;
    //this->setStyleSheet("background-image:url(:/node/back.jpg)");

    QVBoxLayout *layout = new QVBoxLayout(this);
    buttonWidget = new QWidget(this);//操作键
    boardWidget = new BoardWidget(this, this);//棋盘

    buttonWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);

    /*认输键*/
    loseButton = new QPushButton(this);
    loseButton->setText("认输");
    loseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout->addWidget(loseButton);
    connect(loseButton, SIGNAL(clicked(bool)), this, SLOT(Lose()));

    /*计时开始键*/
    startButton = new QPushButton(this);
    startButton ->setText("开始");
    startButton ->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout->addWidget(startButton);
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(Start()));


    /*文件读入操作键*/
    loadFileButton = new QPushButton(this);
    loadFileButton ->setText("导入残局");
    loadFileButton ->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout ->addWidget(loadFileButton);
    connect(loadFileButton,SIGNAL(clicked(bool)),this,SLOT(FileInitBoard()));


    /*文件保存操作键*/
    saveFileButton = new QPushButton(this);
    saveFileButton ->setText("保存残局");
    saveFileButton ->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout ->addWidget(saveFileButton);
    connect(saveFileButton,SIGNAL(clicked(bool)),this,SLOT(FileSaveSlot()));

    /*对战回合标签*/
    label = new QLabel(this);
    label->setText("对方回合");
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    buttonLayout->addWidget(label);






    buttonWidget->setLayout(buttonLayout);

    boardWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);//期盼设置

    layout->addWidget(buttonWidget);
    layout->addWidget(boardWidget);//增加棋盘界面

    /*计时器*/

    timer = new QTimer;
    ui->TimeLCD->setDigitCount(4);
    time = QTime(0,0,0);
    counting_time = time.addSecs(time_limit);
    qDebug()<<counting_time.toString("mm:ss");
    ui->TimeLCD->display(counting_time.toString("mm:ss"));
    connect(timer,SIGNAL(timeout()),this,SLOT(updatesolt()));
   // connect(game,SIGNAL(restartTime()),this,SLOT(restartSlot()));//?????

    QFont ft("华康娃娃体W5", 12);
    ft.setItalic(true);
    this->setFont(ft);
    this->saveFileButton->setFont(ft);
    this->loadFileButton->setFont(ft);
    this->startButton->setFont(ft);
    this->loseButton->setFont(ft);
    this->label->setFont(ft);

    loseButton->setDisabled(true);
    saveFileButton->setDisabled(true);
    game = NULL;


    connect(this, SIGNAL(close()), mainWindow, SLOT(gotoLogin()));
    //connect(game,SIGNAL(restartTime()),this,SLOT(restartSlot()));

    this->setLayout(layout);
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::Lose()
{
    game->lose = 1;
    setStatus(1);
    qDebug()<<"*******************106**********";
}


void GameWidget::setGame(Game *game)
{
    if (this->game != game)
    {
        if (this->game)
            delete this->game;
        this->game = game;
        this->boardWidget->game = game;
        qDebug() << "update the game";
    }
    //boardWidget->update();
    boardWidget->repaint();
    connect(game,SIGNAL(restartTime()),this,SLOT(restartSlot()));
    connect(game,SIGNAL(playMusic()),this,SLOT(playMusicSlot()));
    connect(game,SIGNAL(moveMusic()),this,SLOT(playMoveSlot()));
}

void GameWidget::setStatus(int status)
{
    qDebug() << "GameWidget: set status : " << status;
    if (!status)
    {
        game->curX = game->curY = 0;
        game->status = 0;
       // game->startBoth = 0;
        memset(game->available, 1, sizeof(game->available));
        loseButton->setDisabled(true);
        saveFileButton->setDisabled(true);
        //peaceButton->setDisabled(true);
        label->setText("对方回合");
        qDebug() <<"对方回合";
    }
    else if (status == 1)
    {
        game->curX = game->curY = 0;
        game->status = 1;
       // game->startBoth = 1;
        game->curstep = 0;
        loseButton->setDisabled(false);
        saveFileButton->setDisabled(false);
        memset(game->available, 1, sizeof(game->available));
        //peaceButton->setDisabled(false);
        //judge if win or lose or peace
        judge();
        label->setText("我方回合");
        qDebug() <<"我方回合";

    }
    else if (status == 2)
    {
        game->status = 2;
       // game->startBoth = 2;
       // game->settle();
        game->curX = game->curY = 0;
        label->setText("回合结束");
        setStatus(0);
        send();
        qDebug()<<"回合结束";
    }
    //boardWidget->update();
    boardWidget->repaint();
}
void GameWidget::initTime(){
    timer->start(1000);

    counting_time = time.addSecs(time_limit);
    qDebug()<<counting_time.toString("mm:ss");
    ui->TimeLCD->display(counting_time.toString("mm:ss"));
}

void GameWidget::receive()
{
    QString text;
    text += socket->readAll();
    if (!game)
    {
        game = new Game;
        boardWidget->game = game;
    }
    game->setText(text);//important
    if(game->startBoth == 1)
        if(this->startState == 0){
            game -> startBoth = 1;
            this -> startState = 1;
            timer->start(1000);
            this->startButton->setDisabled(true);
    }
    if(game->restartMess == 1)
        this-> initTime();
    game->reverse();//！！！！翻转！
    //boardWidget->update();
    boardWidget ->repaint();
    setStatus(1);
    qDebug() << "GameWidget: finish receiving";
}

void GameWidget::send()
{
    QByteArray *array = game->getText();//important
    socket->write(array->data());
    qDebug() << "finish sending";
}

/*胜负判断*/
void GameWidget::judge()
{
    if (game->lose == 2)
    {
        QMessageBox::information(this, "游戏结束", "你赢了");
        emit close();
    }
    else if (game->lose == 1)
    {
        send();
        QMessageBox::information(this, "游戏结束", "你输了");
        emit close();
    }
}


void GameWidget::Start()
{
    game -> startBoth = 1;
    this -> startState = 1;
    send();
    //setStatus(1);
   // if(game->startBoth == 1)
    this->startButton->setDisabled(true);
    initTime();


   /* timer->start(100);

    counting_time = time.addSecs(time_limit);
    qDebug()<<counting_time.toString("mm:ss");
    ui->TimeLCD->display(counting_time.toString("mm:ss"));*/
}
void GameWidget::updatesolt()
{
   counting_time= counting_time.addSecs(-1);
   game -> cur_time = counting_time.second();
   qDebug()<<"time: "<<game->cur_time;
   time = QTime(0,0,0);
   counting_time = time.addSecs(game->cur_time);
   ui->TimeLCD->display(counting_time.toString("mm:ss"));
   if(game->cur_time == 0){
       if(game->status == 1){
           qDebug() << "######254#####";


           game->lose = 1;
           setStatus(1);
       }
      // else
       //if(game->status == 0){
        //   game ->lose = 2;
          // setStatus(0);
      // }
   }
}

void GameWidget::restartSlot(){
    initTime();
   // game->restartMess = 0;
}

/*导入残局*/
void GameWidget::FileInitBoard(){
    QString fileName;//文件拓展名要展开
    fileName = QFileDialog::getOpenFileName(this,tr("File"),"",tr("text(*.txt)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if(!file.open((QFile::ReadOnly|QFile::Text))){
            QMessageBox::warning(this,tr("Error"),tr("read file error:&1").arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QStringList lineStr;//如果电脑中有autodesk360这个软件，会导致报错
        int count = 0;
        while(!in.atEnd())
        {
            //lineStr = in.readLine();
            lineStr.append(in.readLine());
            qDebug() << lineStr[count++];
        }
        fileName = nullptr;

        game->clear();
        //1:represent red,you chess; 2:represent black,the other's chess
        int color_1 = 1;
        int color_2 = 2;

        if(lineStr[0] == "black"){
            color_1 = 2;
            game->playerColor = 2;
            //setStatus(1);
            //if(game->status == 1)
              //  setStatus(0);
            //setStatus(0);
        }
        else if(lineStr[0] == "red"){
            color_1 = 1;
            game->playerColor = 1;
            //game->status = 1;
            //if(game->status == 2)
                //setStatus(1);
        }
        if(lineStr[8] == "red")
            color_2 = 1;
        for(int i = 0; i < 7; i ++){
            QStringList line = lineStr[i+1].split(QRegExp(",|\\s+|<|>"), QString::SkipEmptyParts);
           // int num = line[0].toInt();
            qDebug()<<"line: "<<i+1<<" "<<line;
            int num = line[0].toInt();
            for(int j = 0; j < num; j ++){
                int x = line[j*2+1].toInt();
                int y = line[j*2+2].toInt();
                game->chess[9-y][x] = color_1;
                game->chessType[9-y][x] = i;
            }
        }
        for(int i = 0; i < 7; i ++){
            QStringList line = lineStr[i+9].split(QRegExp(",|\\s+|<|>"), QString::SkipEmptyParts);
           // int num = line[0].toInt();
            qDebug()<<"line: "<<i+9<<" "<<line;
            int num = line[0].toInt();
            for(int j = 0; j < num; j ++){
                int x = line[j*2+1].toInt();
                int y = line[j*2+2].toInt();
                game->chess[9-y][x] = color_2;
                game->chessType[9-y][x] = i;
            }
        }
        file.close();
       // boardWidget->repaint();
        //for(int i = 0; i < 3; i ++){
       /* num = lineStr[0].toInt();
        QStringList list1 = lineStr[1].split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
        in1 = list1[0].toInt();
        in2 = list1[1].toInt();
        QStringList list2 = lineStr[2].split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
        out1 = list2[0].toInt();
        out2 = list2[1].toInt();
        out3 = list2[2].toInt();
        //qDebug()<<num<<" "<<in1<<" "<<in2<<" ";
        //}
        board = new Board(this);
        setCentralWidget(board);
        board->initBoard(num,in1,in2,out1,out2,out3);
        //qDebug()<<board->getPipes();
       // board->setGeometry(100,100,800,1000);
        board->show();*/

    }
}

/*保存残局*/
void GameWidget::FileSaveSlot(){
    //QTime nowTime = QTime::currentTime();
    QString name = "board";
    QString addr = QFileDialog::getSaveFileName(nullptr,"SaveFile",name+".txt");
    QFile file(addr);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"save file failed: cannot open file!";
            return;
    }
   // QTextStream txts(&file);
    QString color[2] = {"red","black"};
    int player_Color[2];
    if(game->playerColor == 1){//对方，黑方
        player_Color[0] = 1;//黑色
        player_Color[1] = 0;//红色
    }
    if(game->playerColor == 2){//己方，红方
        player_Color[0] = 0;
        player_Color[1] = 1;
    }
    QVector<QString> info(16);
    QVector<int> num_1(7);
    QVector<int> num_2(7);
    info[0] = color[player_Color[0]];
    info[8] = color[player_Color[1]];
    qDebug()<<"playcolor:"<<info[0];
    for(int x  = 0; x < 9; x ++)
        for(int y = 0; y <10; y++){
            QString pos_x = QString::number(x);
            QString pos_y = QString::number(y);
            if(game->chess[9-y][x] == player_Color[0]+1){
                num_1[game->chessType[9-y][x]] ++;
                info[game->chessType[9-y][x]+1] += (" <"+pos_x+","+pos_y+">");
            }
            if(game->chess[9-y][x] == player_Color[1]+1){
                num_2[game->chessType[9-y][x]]++;
                info[game->chessType[9-y][x]+9]+= (" <"+pos_x+","+pos_y+">");
            }
        }


    QTextStream txts(&file);
    txts << info[0]<<endl;
    for(int i = 1; i < 8; i ++){
        QString num = QString::number(num_1[i-1]);
        txts <<num<<info[i]<<endl;
    }
    txts << info[8]<<endl;
    for(int i = 9; i < 16; i ++){
        QString num = QString::number(num_2[i-9]);
        txts <<num<<info[i]<<endl;
    }
    file.close();
}

void GameWidget::playMusicSlot(){
    killer = new QSound(":/bgm/checkmate.wav");
    killer->play();
}

void GameWidget::playMoveSlot(){
    move = new QSound(":/bgm/operate.wav");
    move->play();
}
