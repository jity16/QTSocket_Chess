#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork>
#include <QMainWindow>

class MainWindow;

#include "gamewidget.h"
#include "loginwidget.h"
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
    GameWidget  *gameWidget; //游戏界面，负责通信、求和、认输
    LoginWidget *loginWidget;//连接界面，负责服务器监听和客户端连接

public slots:

    void gotoLogin();//切换到连接界面
    void gotoGame(QTcpSocket *socket, Game *game);//切换到游戏界面
};

#endif // MAINWINDOW_H
