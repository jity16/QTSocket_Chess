#include "game.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <QDebug>
#include <QMessageBox>

// 0 represent empty, 1 represent your chess, 2 represent the other's chess, 3 represnet dead chess
// int board[10][10];
// 1 represent king
// int isKing[10][10];
// 1 represent current available position
// int available[10][10];
// 1 represent black and 2 represent white
// int playerColor;
// current moving chess
// int curX, curY;
int Game::initboard[10][9];
Game::Game(int init):QWidget(nullptr)//(int init)
{
    clear();
    if (init)
        this->init();
}
Game::~Game(){

}

void Game::clear()
{
    memset(chess,0,sizeof(chess));
    memset(chessType,9,sizeof(chessType));
    memset(available,0,sizeof(available));//棋盘上位置均可以落子
    playerColor = 0;
    curX = curY = 0;
    lose = 0;
    status = 0;
    curstep = 0;
    ways.clear();
}
int _abs(int x)
{
    return x > 0 ? x : -x;
}
void Game::init()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++){
            chess[i][j] = initboard[i][j];
            available[i][j] = 0;//初始时available均为1
        }
    memset(chessType,9,sizeof(chessType));
    chessType[0][4] = chessType[9][4] = 0;//将
    chessType[0][3] = chessType[0][5] = chessType[9][3] = chessType[9][5] = 1;//士
    chessType[0][2] = chessType[0][6] = chessType[9][2] = chessType[9][6] = 2;//象
    chessType[0][1] = chessType[0][7] = chessType[9][1] = chessType[9][7] = 3;//马
    chessType[0][0] = chessType[0][8] = chessType[9][0] = chessType[9][8] = 4;//车
    chessType[2][1] = chessType[2][7] = chessType[7][1] = chessType[7][7] = 5;//炮
    for(int i = 0 ; i < 9; i = i + 2){
        chessType[3][i] = chessType[6][i] = 6;//卒
    }
    playerColor = 1;
}

void Game::reverse()
{
    playerColor = (playerColor == 1 ? 2 : 1);
    //curX = 9 - curX; curY = 9 - curY;
    if (lose == 1)
        lose = 2;
    else if (lose == 2)
        lose = 1;
    if (startBoth == 1)
        startBoth = 2;
    else if (startBoth == 2)
        startBoth = 1;
}

int Game::getChess(int x, int y){
    return chess[x][y];
}

int Game::getType(int x,int y){
    return chessType[x][y];
}
void Game::bin_available(int x,int y){
    if(chess[curX][curY] == 1){//红方，我方
        if(curX <= 4){//过河后
            if(curY - 1 >= 0)
                available[curX][curY-1]=1;
            if(curY +1 <= 8)
                available[curX][curY+1]=1;
        }
        if(curX - 1 >= 0)
            available[curX-1][curY]=1;
    }
    else if(chess[curX][curY]==2){//黑方，对方
        if(curX >= 5){//过河后
            if(curY - 1 >= 0)
                available[curX][curY-1]=1;
            if(curY + 1 <= 8)
                available[curX][curY+1]=1;
        }
        if(curX +1 <= 9)
            available[curX+1][curY] = 1;
    }
}
void Game::pao_available(int x,int y){//炮

    available[curX][curY] = 1;
    for(int i = curX - 1; i >= 0; i --){
        if(chess[i][curY]!= 0)//如果有子阻拦
            break;
        available[i][curY] = 1;
    }
    for(int i = curX + 1; i <= 9; i ++){
        if(chess[i][curY]!= 0)//如果有子阻拦
            break;
        available[i][curY] = 1;
    }
    for(int i = curY + 1; i <= 8; i ++){
        if(chess[curX][i]!= 0)//如果有子阻拦
            break;
        available[curX][i] = 1;
    }
    for(int i = curY - 1; i >= 0; i --){
        if(chess[curX][i]!= 0)//如果有子阻拦
            break;
        available[curX][i] = 1;
    }
    //if(chess[curX][curY] == 1){//红方我方
    if((chess[x][y] != chess[curX][curY]) && chess[x][y]!= 0){//如果目标位置是敌方的子-炮打隔子
        if(curX == x){//如果目标点和当前点在同一行
            int min = (curY < y)? curY:y;
            int max = (curY > y)? curY:y;
            int j = min;
            int count = 0;
            for(j = min + 1; j < max; j ++){
                if(chess[curX][j]!= 0){
                    count++;
                    //available[x][y] = 1;
                       // break;
                  }
             }
             if( count == 1) available[x][y]=1;
             //if( j == max) available[x][y] = 0;
         }
        if(curY == y){//如果目标点和当前点在同一列
            int min = (curX < x)? curX:x;
            int max = (curX > x)? curX:x;
            int j = min;
            int count = 0;
            for(j = min + 1; j < max; j ++){
                if(chess[j][curY]!= 0){
                    count ++;
                    //available[x][y] = 1;
                        //break;
                  }
             }
             if( count == 1) available[x][y] = 1;
             //if( j == max) available[x][y] = 0;
         }
    }
}
void Game::ju_available(int x, int y){//车
    for(int i = curX + 1; i <= 9; i ++){
        available[i][curY] = 1;
        if(chess[i][curY]!= 0)
            break;
    }
    for(int i = curX - 1; i >= 0; i --){
        available[i][curY] = 1;
        if(chess[i][curY] != 0)
            break;
    }
    for(int i = curY +1 ; i <= 8; i ++){
        available[curX][i] = 1;
        if(chess[curX][i] != 0)
            break;
    }
    for(int i = curY -1; i >= 0; i --){
        available[curX][i] = 1;
        if(chess[curX][i] != 0)
            break;
    }

}
void Game::ma_available(int x, int y){//马走日，别马脚

    if(curX -2 >= 0 && curY + 1 <= 8)
        if(chess[curX - 1][curY]== 0 &&chess[curX -1][curY+1]==0)
            available[curX-2][curY+1] = 1;

    if(curX - 2 >= 0 &&curY - 1 >= 0)
        if(chess[curX-1][curY]==0 &&chess[curX-1][curY-1]==0)
            available[curX-2][curY-1]=1;

    if(curX - 1 >= 0 && curY - 2 >= 0)
        if(chess[curX-1][curY-1]==0 && chess[curX][curY-1]==0)
            available[curX-1][curY-2]=1;

    if(curX + 1 <= 9 && curY -2 >= 0)
        if(chess[curX][curY-1]==0&&chess[curX+1][curY-1]==0)
            available[curX+1][curY-2]=1;

    if(curX + 2 <= 9 && curY-1 >=0)
        if(chess[curX+1][curY-1]==0&&chess[curX+1][curY]==0)
            available[curX+2][curY-1]=1;

    if(curX +2 <= 9 && curY + 1<= 8)
        if(chess[curX+1][curY]==0 && chess[curX+1][curY+1]==0)
            available[curX+2][curY+1]=1;

    if(curX +1 <= 9 && curY + 2 <= 8)
        if(chess[curX+1][curY+1]==0 && chess[curX][curY+1]==0)
            available[curX+1][curY+2]=1;

    if(curX -1 >= 0 && curY + 2 <= 8)
        if(chess[curX][curY+1]==0 && chess[curX-1][curY+1]==0)
            available[curX-1][curY+2]=1;

}

void Game::xiang_available(int x, int y){//象走田，塞象眼，不能过河
    if(curX-2 >= 0 && curY +2 <= 8)
        if(chess[curX-1][curY+1]==0)
            available[curX-2][curY+2] = 1;
    if(curX -2 >= 0 && curY- 2>= 0)
        if(chess[curX - 1][curY-1]==0)
            available[curX-2][curY-2] = 1;
    if(curX +2 <= 9 && curY-2 >= 0)
        if(chess[curX+1][curY-1]==0)
            available[curX+2][curY-2] = 1;
    if(curX + 2 <= 9 && curY+ 2 <= 8)
        if(chess[curX+1][curY+1] == 0)
            available[curX+2][curY+2] = 1;
    /*if(chessType[curX][curY]== 1)//红色*/
    if(curX <= 4 && x >= 5) available[x][y] = 0;
    /*if(chessType[curX][curY]== 2)//黑色*/
    if(curX >= 5 && x <= 4) available[x][y] = 0;

}

void Game::si_available(int x, int y){//士走斜线，九宫格内
    if(chess[curX][curY]==1){//红方
        available[9][3]=available[8][4]=available[7][5]= 1;
        available[9][5]= available[7][3] = 1;
        if(_abs(x-curX)>=2 || _abs(y-curY)>=2)
            available[x][y] = 0;
    }
    if(chess[curX][curY]==2){//黑方
        available[0][3]=available[1][4]=available[2][5]= 1;
        available[0][5]= available[2][3] = 1;
        if(_abs(x-curX)>=2 || _abs(y-curY)>=2)
            available[x][y] = 0;
    }

}

void Game::jiang_available(int x,int y){//九宫格内，可上可左可右,如果将帅之间没有隔着的子，就可以飞过去
    if(chess[curX][curY] == 1){//红方
        if(_abs(x- curX)==1&&(y == curY))//上下移动
            if(x >= 7 && x <= 9)
                available[x][y] = 1;
        if(_abs(y-curY)==1 &&(x == curX))//左右移动
            if(y >= 3 && y <= 5)
                available[x][y] = 1;
        for(int i = 0 ; i <= 2; i ++){
            if(chess[i][curY]==2 && chessType[i][curY] == 0){
                int j = i +1;
                for(j = i+1; j < curX; j ++){
                    if(chess[j][curY]!=0){
                       available[i][curY]=0;
                       break;
                    }
                }
                if(j == curX)available[i][curY]=1;
            }
        }
    }
    else if(chess[curX][curY] == 2){//黑方
        if(_abs(x- curX)==1&&(y == curY))//上下移动
            if(x >= 0 && x <= 2)
                available[x][y] = 1;
        if(_abs(y-curY)==1 &&(x == curX))//左右移动
            if(y >= 3 && y <= 5)
                available[x][y] = 1;
        for(int i = 0 ; i <= 2; i ++){
            if(chess[i][curY]==2 && chessType[i][curY] == 0)
                available[i][curY]=1;
        }
        for(int i = 7 ; i <= 9; i ++){
            if(chess[i][curY]==1 && chessType[i][curY] == 0){
                int j = i -1;
                for(j = i-1; j > curX; j --){
                    if(chess[j][curY]!=0){
                       available[i][curY]=0;
                       break;
                    }
                }
                if(j == curX)available[i][curY]=1;
            }
        }
    }
}

void Game::getAvailable(int x,int y){//获得当前可下棋处,x,y为下一部欲落子点
    //memset(available,0,sizeof(available));
    //available[curX][curY]=1;
    if(chessType[curX][curY] == 6){//卒或兵
        bin_available(x,y);
    }
    if(chessType[curX][curY] == 5){//炮
        pao_available(x,y);
    }
    if(chessType[curX][curY] == 4){//车
        ju_available(x,y);
    }
    if(chessType[curX][curY] == 3){//马
        ma_available(x,y);
    }
    if(chessType[curX][curY] == 2){//象
        xiang_available(x,y);
    }
    if(chessType[curX][curY]==1){//士
        si_available(x,y);
    }
    if(chessType[curX][curY]==0){//将
        jiang_available(x,y);
    }
    if(chess[x][y]==chess[curX][curY])//如果按规则可下位置处是自己方的棋，则不可下
        available[x][y] = 0;
}

// set current chess, return false if the chess is not the player's or the chess cannot move
bool Game::chooseChess(int x, int y)
{
    //getAvailable(x,y);
    /*if (!available[x][y])
    {
        qDebug()<< "!available";
        return false;
    }*/
   /* if (!curstep)
        curX = x, curY = y;
    else{*/
        memset(available,0,sizeof(available));
        getAvailable(x,y);
      //  moveChess(x, y);
    // }
        if (!available[x][y])
        {
            qDebug()<< "!available";
            return false;
        }
        moveChess(x, y);
    return true;
}



/*bool Game::kill(){
    return true;
}*/

bool Game::findRound(int pos_x,int pos_y,int type){
    //身边是否有对方的卒
    if(chess[pos_x][pos_y+1]==type && chessType[pos_x][pos_y+1]== 6){
        qDebug()<<"***hongshuai ---- zu****";
        return true;
    }
    if(chess[pos_x][pos_y-1]==type && chessType[pos_x][pos_y-1]== 6){
        qDebug()<<"***hongshuai ---- zu****";
        return true;
    }
    if(chess[pos_x-1][pos_y]==type && chessType[pos_x-1][pos_y]== 6){
        qDebug()<<"***hongshuai ---- zu****";
        return true;
    }
    //身边是否有对方的车
    for(int i = pos_y+1; i <= 8; i ++){
        if(chess[pos_x][i]== type && chessType[pos_x][i]== 4){
            qDebug()<<"***hongshuai ---- ju****";
            return true;
        }
        if(chess[pos_x][i]!= 0)
            break;
    }
    for(int i = pos_y-1; i >= 0; i --){
        if(chess[pos_x][i]== type && chessType[pos_x][i] == 4){
            qDebug()<<"***hongshuai ---- zu****";
            return true;
        }
        if(chess[pos_x][i]!= 0)
            break;
    }
    for(int i = pos_x -1; i >= 0; i --){
        if(chess[i][pos_y]==type && chessType[i][pos_y]==4){
            qDebug()<<"***hongshuai ---- zu****";
            return true;
        }
        if(chess[i][pos_y]!= 0)
            break;
    }
    for(int i = pos_x + 1; i <= 9 ;i++){
        if(chess[i][pos_y] == type && chessType[i][pos_y] == 4){
            qDebug()<<"***hongshuai ---- zu****";
            return true;
        }
        if(chess[i][pos_y]!=0)
            break;
    }

    //身边是否有对方的炮
    int count = 0;
    int flag = 0;
    int pao_x = 0;
    int pao_y = 0;
    for(int i = pos_y+1; i <= 8; i ++){
        if(chess[pos_x][i]== type && chessType[pos_x][i]== 5){
            flag = 1;
            pao_x = pos_x;
            pao_y = i;
            break;
        }
        if(chess[pos_x][i]!= 0)
            count ++;
    }
    if(count == 1&&flag == 1) {
        qDebug()<<"*******pao_1******";
        return true;
    }
    if(count == 0&&flag == 1){
        for(int i = pao_y+1; i <= 8; i ++){
            if(chess[pos_x][i]!= 0 && chessType[pos_x][i]!= 5)
                break;
            if(chess[pos_x][i] == type && chessType[pos_x][i]==5){
                qDebug()<<"#######440######";
                return true;
            }
        }
    }
    count = 0;
    flag = 0;
    for(int i = pos_y-1; i >= 0; i --){
        if(chess[pos_x][i]== type && chessType[pos_x][i]== 5){
            flag = 1;
            pao_x = pos_x;
            pao_y = i;
            break;
        }
        if(chess[pos_x][i]!= 0)
            count ++;
    }
    if(count == 1 && flag == 1) {
        qDebug()<<"*******pao_2******";
        return true;
    }
    if(count == 0&&flag == 1){
        for(int i = pao_y-1; i >= 0; i --){
            if(chess[pos_x][i]!= 0 && chessType[pos_x][i]!= 5)
                break;
            if(chess[pos_x][i] == type && chessType[pos_x][i]==5){
                qDebug()<<"#######464######";
                return true;
            }
        }
    }
    count = 0;
    flag = 0;
    for(int i = pos_x-1; i >= 0; i --){

        if(chess[i][pos_y]== type && chessType[i][pos_y]== 5){
            flag = 1;
            pao_x = pos_x;
            pao_y = i;
            break;
        }
        if(chess[i][pos_y]!= 0)
            count ++;
    }
    if(count == 1 && flag == 1) {
        qDebug()<<"*******pao_3******";
        return true;
    }
    if(count == 0&&flag == 1){
        for(int i = pao_x-1; i >= 0; i --){
            if(chess[i][pos_y]!= 0 && chessType[i][pos_y]!= 5)
                break;
            if(chess[i][pos_y] == type && chessType[i][pos_y]==5){
                qDebug()<<"#######493######";
                return true;
            }
        }
    }
    count = 0;
    flag = 0;
    for(int i = pos_x+1; i <= 9; i ++){
        if(chess[i][pos_y]== type && chessType[i][pos_y]== 5){
            flag = 1;
            pao_x = pos_x;
            pao_y = i;
            break;
        }
        if(chess[i][pos_y]!= 0)
            count ++;
    }
    if(count == 1 && flag == 1) {
        qDebug()<<"*******pao_4******";
        return true;
    }
    if(count == 0&&flag == 1){
        for(int i = pao_x+1; i <= 8; i ++){
            if(chess[i][pos_y]!= 0 && chessType[i][pos_y]!= 5)
                break;
            if(chess[i][pos_y] == type && chessType[i][pos_y]==5){
                qDebug()<<"#######464######";
                return true;
            }
        }
    }

    //身边是否有对方的马
    int x = pos_x;
    int y = pos_y;
    if(x -1 >= 0 && y +2 <= 8){
        if(chess[x-1][y + 2] == type && chessType[x-1][y+2]== 3){
            if(chess[x-1][y+1]==0 &&chess[x][y+1]==0){
                qDebug()<<"*******ma_1*******";
                return true;
            }
        }
    }
    if(x -1 >= 0 && y -2 >= 0){
        if(chess[x-1][y - 2] == type && chessType[x-1][y-2]== 3){
            if(chess[x-1][y-1]==0 &&chess[x][y-1]==0){
                qDebug()<<"*******ma_2*******";
                return true;
            }
        }
    }
    if(x +1 <= 9 && y +2 <= 8){
        if(chess[x+1][y + 2] == type && chessType[x-1][y+2]== 3){
            if(chess[x+1][y+1]==0 &&chess[x][y+1]==0){
                qDebug()<<"*******ma_3*******";
                return true;
            }
        }
    }
    if(x +1 <= 9 && y -2 >= 0){
        if(chess[x+1][y -2] == type && chessType[x+1][y-2]== 3){
            if(chess[x+1][y-1]==0 &&chess[x][y-1]==0){
                qDebug()<<"*******ma_4*******";
                return true;
            }
        }
    }
    if(x -2 >= 0 && y -1 >= 0){
        if(chess[x-2][y-1] == type && chessType[x-2][y-1]== 3){
            if(chess[x-1][y]==0 &&chess[x-1][y-1]==0){
                qDebug()<<"*******ma_5*******";
                return true;
            }
        }
    }
    if(x -2 >= 0 && y +1 <= 8){
        if(chess[x-2][y+1] == type && chessType[x-2][y+1]== 3){
            if(chess[x-1][y]==0 &&chess[x-1][y+1]==0){
                qDebug()<<"*******ma_6*******";
                return true;
            }
        }
    }
    if(x +2 <= 9 && y -1 >= 0){
        if(chess[x+2][y-1] == type && chessType[x+2][y-1]== 3){
            if(chess[x+1][y-1]==0 &&chess[x+1][y]==0){
                qDebug()<<"*******ma_7*******";
                return true;
            }
        }
    }
    if(x +2 <= 9 && y +1 <= 8){
        if(chess[x+2][y+1] == type && chessType[x+2][y+1]== 3){
            if(chess[x+1][y]==0 &&chess[x+1][y+1]==0){
                qDebug()<<"*******ma_8*******";
                return true;
            }
        }
    }

    //身边是否有对方的将
    if(type == 2){
        for(int i = pos_x -1; i -- ; i >= 0){
            if(chess[i][pos_y]==2 && chessType[i][pos_y]== 0){
                qDebug()<<"*******jiang_1 ******";
                return true;
            }
            if(chess[i][pos_y]!= 0)
                break;
        }
    }
    else if(type == 1){
        for(int i = pos_x +1; i ++ ; i <= 9){
            if(chess[i][pos_y]==1 && chessType[i][pos_y]== 0){
                qDebug()<<"*******jiang_2 ******";
                return true;
            }
            if(chess[i][pos_y]!= 0)
                break;
        }
    }

    return false;
}

bool Game::KingDanger(){
    music = false;
    /*对于红方的帅*/
    int pos_x = 9,pos_y = 4;
    for(int i = 7; i <= 9; i ++)
        for(int j = 3; j <= 5; j ++){
            if(chessType[i][j]== 0){
                pos_x = i;
                pos_y = j;
                break;
            }
        }
    music = findRound(pos_x,pos_y,2);
    if(music) {
        qDebug()<<"*********music*******";
        return music;
    }
    pos_x = 0,pos_y = 4;
    for(int i = 0; i <= 2; i ++)
        for(int j = 3; j <= 5; j ++){
            if(chessType[i][j]== 0){
                pos_x = i;
                pos_y = j;
                break;
            }
        }
    music = findRound(pos_x,pos_y,1);
    if(music)
        qDebug()<<"********music_2*******";
    return music;

}

// move must obey the rules, and set the dead chess
void Game::moveChess(int nxtX, int nxtY)
{
    //write the rule
    if (!available[nxtX][nxtY])
    {
        qDebug() << "the move is not available";
        //curstep = 0;
        curstep --;
        return;
    }
    if(chessType[nxtX][nxtY]== 0&&(chess[nxtX][nxtY]!=chess[curX][curY])){
       // if(playerColor == 1){
        //    lose = 2;
        //}
        //else if(playerColor == 2){
        qDebug()<<"######351###############";
       /* if(chess[curX][curY] == 2){
            lose = 1;
            qDebug()<<"*********606******";
        }
        else if(chess[curX][curY]==1)*/
            lose = 2;
        //}
    }
    chess[nxtX][nxtY] = chess[curX][curY];
    chess[curX][curY] = 0;

    chessType[nxtX][nxtY] = chessType[curX][curY];
    curX = nxtX;
    curY = nxtY;
    restartMess = 1;
    emit moveMusic();
    music = KingDanger();//判断是否有将军行为
    if(music == true){//播放音乐
        emit playMusic();
        qDebug()<<"****playmusic";
    }
    emit restartTime();


    //return true;
}

// check if the chess becoming the king and delete the dead chess






QByteArray *Game::getText()
{
    QByteArray *array = new QByteArray;
    array->clear();
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            array->append('0' + chess[i][j]);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            array->append('0' + chessType[i][j]);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            array->append('0' + available[i][j]);
    array->append('0' + playerColor);
    array->append('0' + curX);
    array->append('0' + curY);
    array->append('0' + lose);
    array->append('0' + startBoth);
    array->append('0' + cur_time);
    array->append('0'+ restartMess);
    return array;
}

void Game::setText(QString text)
{
    qDebug() << "text length : " << text.length();
    if (text.length() != 90 + 90 + 90 + 7)

    {
        qDebug() << "text length wrong !!!";
        return;
    }
    int cur = 0;

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            chess[i][j] = text[cur++].toLatin1()-'0';
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            chessType[i][j] = text[cur++].toLatin1() - '0';
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            available[i][j] = text[cur++].toLatin1() - '0';
    playerColor = text[cur++].toLatin1() - '0';
    curX = text[cur++].toLatin1() - '0';
    curY = text[cur++].toLatin1() - '0';
    lose = text[cur++].toLatin1() - '0';
    startBoth = text[cur++].toLatin1() - '0';
    cur_time = text[cur++].toLatin1() - '0';
    restartMess = text[cur++].toLatin1()-'0';

}

void Game::freshAvailable()
{
    memset(available,0,sizeof(available));
}
