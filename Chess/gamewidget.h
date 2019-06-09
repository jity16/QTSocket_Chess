#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QtNetwork>
#include <QTimer>
#include <QTime>
#include <QtMultimedia/QSound>
//#include <QLCDNumber>


class GameWidget;

#include "boardwidget.h"
#include "mainwindow.h"
#include "game.h"

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0, MainWindow *mainWindow = 0);
    ~GameWidget();
    void setGame(Game *game);
    void setStatus(int status);
    void initTime();
    QTcpSocket *socket;
    Game *game;//游戏存储类，负责游戏状态的存储和判定
    QPushButton *loseButton, *startButton, *loadFileButton, *saveFileButton;
    //QLCDNumber *TimeLCD;
    QLabel *label;

private:
    Ui::GameWidget *ui;
    QWidget *buttonWidget;//存储各种按钮
    BoardWidget *boardWidget;//棋盘
    MainWindow *mainWindow;

    QTimer* timer;
    QTime time;
    QTime counting_time;//倒计时:当前显示时间
    QSound *killer;
    QSound *move;
    int time_limit = 20;//设置时间限制
    int startState = 0;



signals:
    void close();

public slots:
    void Lose();
    void Start();
    void receive();
    void send();
    void judge();//判断胜负
    void updatesolt();//更新计时器
    void restartSlot();//重新启动计时器
    void FileInitBoard();//读入文件操作键
    void FileSaveSlot();//文件保存操作键
    void playMusicSlot();//播放将军音乐
    void playMoveSlot();//走步音效
private slots:
    //void on_StartTimeButton_clicked();
};

#endif // GAMEWIDGET_H
