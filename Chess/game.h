#ifndef GAME_H
#define GAME_H

#include <QString>
#include <iostream>
#include <vector>
#include <QWidget>
using namespace std;

class Game:public QWidget{

    Q_OBJECT
public:
    Game(int init = 1);
    //static int initboard[10][10];
    static int initboard[10][9];
    ~Game();
    //static int initboardType[10][9];

public:
    // 0 represent empty, 1 represent your chess, 2 represent the other's chess, 3 represnet dead chess
    int chess[10][9];
    int chessType[10][9];
    // 1 represent king
    //int isKing[10][10];
    // 1 represent current available position
    int available[10][10];
    // 1 represent black and 2 represent red
    int playerColor;
    // current moving chess
    int curX, curY;

    // 0 no lose 1 I lose 2 the other one lose
    int lose = 0;

    // 0 not my turn 1 my turn 2 finish
    int status = 0;

    int curstep = 0;

    int startBoth = 0;

    int cur_time = 30;

    int restartMess = 0;


    vector<vector<int> > ways;
signals:
    void restartTime();
    void playMusic();
    void moveMusic();


public:
    //Game(int init = 0);
    void clear();
    void init();
    void reverse();
    int getChess(int x, int y);//获得棋子属性：0-空，1-我方，2-对方；
    int getType(int x, int y); //获得棋子种类
    // set current chess, return false if the chess is not the player's or the chess cannot move
    bool chooseChess(int x, int y);
    // move must obey the rules, and set the dead chess
    void moveChess(int nxtX, int nxtY);
    // check if the chess becoming the king and delete the dead chess
   // void settle();

    // get the available map, return false if there is no current chess
    void bin_available(int x,int y);
    void pao_available(int x,int y);
    void ju_available(int x,int y);
    void ma_available(int x,int y);
    void xiang_available(int x, int y);
    void si_available(int x, int y);
    void jiang_available(int x,int y);

    bool music = false;
    bool findRound(int x,int y,int t);
    bool KingDanger();
    void getAvailable(int x,int y);

    void freshAvailable();
    QByteArray *getText();
    void setText(QString text);
    //void dfs(int, int, int, int);
};

#endif // GAME_H
