#include "loginwidget.h"
#include "game.h"
#include "ui_loginwidget.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QString>
#include <QMessageBox>
//#include "diydialog.h"

LoginWidget::LoginWidget(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->mainWindow = mainWindow;
    QFont ft("华康娃娃体W5", 15);
    ft.setItalic(true);
    this->setFont(ft);
    ui->client->setFont(ft);
    ui->server->setFont(ft);
    ui->label_5->setFont(ft);
    ui->label_6->setFont(ft);
    ui->serverIP->setFont(ft);
    ui->serverPort->setFont(ft);
    ui->serverStatus->setFont(ft);
    ui->serverConnect->setFont(ft);
    ui->label_3->setFont(ft);
    ui->label_4->setFont(ft);
    ui->clientConnect->setFont(ft);

    connect(ui->clientConnect, SIGNAL(clicked(bool)), this, SLOT(onClientConnect()));//客户端连接按钮
    connect(ui->serverConnect, SIGNAL(clicked(bool)), this, SLOT(onServerConnect()));//服务器连接按钮
    connect(this, SIGNAL(gotoGame(QTcpSocket*, Game*)), mainWindow, SLOT(gotoGame(QTcpSocket*, Game*)));//跳转游戏界面

    // get ip address
    QString ip = "";
    qDebug() << QNetworkInterface::allAddresses();
    for (int i = 0; i < QNetworkInterface::allAddresses().size(); i++)
    {
        qDebug() << QNetworkInterface::allAddresses().at(i);
        qDebug() << QNetworkInterface::allAddresses().at(i).protocol();
        if (QNetworkInterface::allAddresses().at(i).protocol() == QAbstractSocket::IPv4Protocol && QNetworkInterface::allAddresses().at(i) != QHostAddress::LocalHost)
        {
            ip = QNetworkInterface::allAddresses().at(i).toString();
           break;
        }
    }
    if (ip == "")
        ip = QString("127.0.0.1");
    qDebug()<<ip;
    ui->serverIP->setText(ip);


    for(int j = 0; j < 9 ; j ++){
        Game::initboard[0][j] = 2;//服务器端是对手方
        Game::initboard[9][j] = 1;//客户端是我方
    }
    for(int j = 0; j < 9; j = j + 2){
        Game::initboard[3][j] = 2;
        Game::initboard[6][j] = 1;
    }
    Game::initboard[2][1] = 2;
    Game::initboard[2][7] = 2;
    Game::initboard[7][1] = 1;
    Game::initboard[7][7] = 1;


    qDebug()<<"LoginWidget:初始化棋盘";
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::onClientConnect()//客户端连接与终止连接按钮
{
    if (ui->clientConnect->text() == "连接")   //客户端连接按钮
    {
        readWriteSocket = new QTcpSocket;
        readWriteSocket->connectToHost(QHostAddress(ui->clientIP->text()), ui->clientPort->text().toInt());
        qDebug() << "connect to " << ui->clientIP->text() << " on port " << ui->clientPort->text();
        connect(readWriteSocket, SIGNAL(connected()), this, SLOT(finishConnection()));
        if(!this->readWriteSocket->waitForConnected()){
            QMessageBox::about(NULL,"","no such room available");
        }
        ui->clientConnect->setText("终止连接"); //终止连接按钮
        ui->clientStatus->setText("连接中……");
    }
    else    //如果按下了终止连接按钮，则返回到连接按钮
    {
        readWriteSocket->close();
        delete readWriteSocket;
        readWriteSocket = NULL;
        qDebug() << "stop connection";
        ui->clientConnect->setText("连接");
        ui->clientStatus->setText("");
    }
}

void LoginWidget::onServerConnect()//服务器连接按钮
{
    if (ui->serverConnect->text() == "创建服务器")//服务器连接按钮
    {
        listenSocket = new QTcpServer;
        listenSocket->listen(QHostAddress::Any, ui->serverPort->text().toInt());
        qDebug() << "listen on port " << ui->serverPort->text().toInt();
        connect(listenSocket, SIGNAL(newConnection()), this, SLOT(accecptConnection()));
        ui->serverConnect->setText("终止");
        ui->serverStatus->setText("等待他人连入");
    }
    else
    {
        listenSocket->close();
        delete listenSocket;
        listenSocket = NULL;
        ui->serverConnect->setText("创建服务器");
        ui->serverStatus->setText("");
    }
}

void LoginWidget::clear()
{
    listenSocket = NULL;
    readWriteSocket = NULL;
    ui->serverStatus->setText("");
    ui->clientStatus->setText("");
    ui->serverConnect->setText("创建服务器");
    ui->clientConnect->setText("连接");
}

void LoginWidget::accecptConnection()
{
    qDebug() << "[server] the connection has been accepted";
    readWriteSocket = listenSocket->nextPendingConnection();
    connect(readWriteSocket, SIGNAL(readyRead()), mainWindow->gameWidget, SLOT(receive()));
    emit gotoGame(readWriteSocket, new Game);
}

void LoginWidget::finishConnection()
{
    qDebug() << "[client] the client has been connected to the server";
    connect(readWriteSocket, SIGNAL(readyRead()), mainWindow->gameWidget, SLOT(receive()));
    emit gotoGame(readWriteSocket, NULL);
}


