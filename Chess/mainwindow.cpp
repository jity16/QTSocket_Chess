#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"
#include "gamewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loginWidget = new LoginWidget(this, this);//登录界面
    this->setStyleSheet("background-image:url(:/node/1.png)");//border:2px groove gray;border-radius:10px;padding:2px 4px;");
    gameWidget = new GameWidget(this,this);//游戏界面
    ui->stackedWidget->addWidget(loginWidget);
    ui->stackedWidget->addWidget(gameWidget);
    ui->stackedWidget->setCurrentWidget(loginWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gotoLogin()
{
    this->close();
}

void MainWindow::gotoGame(QTcpSocket *socket, Game *game)//跳转到游戏界面
{
    //gameWidget = new GameWidget(this,this);//游戏界面
    qDebug()<<"MainWindow: goto Game";
    gameWidget->socket = socket;
    gameWidget->setGame(game);

    if (!game)
    {
        qDebug()<<"mainwindow:: !game";
        gameWidget->setGame(new Game);
        gameWidget->setStatus(0);
    }
    else
    {
        gameWidget->setStatus(0);
        gameWidget->send();
    }
    ui->stackedWidget->setCurrentWidget(gameWidget);//将当前界面切换到游戏界面
}
