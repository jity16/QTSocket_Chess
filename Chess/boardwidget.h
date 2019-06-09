#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include "game.h"

class BoardWidget;

#include "gamewidget.h"

namespace Ui {
class BoardWidget;
}

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWidget(QWidget *parent = 0, GameWidget *gameWidget = 0);
    ~BoardWidget();

    Game *game;
    GameWidget *gameWidget;

   /* QTimer* timer;
    QTime time;
    QTime counting_time;//倒计时:当前显示时间
    int time_limit = 30;//设置时间限制*/

protected:
    float offset_x = 50;
    float offset_y = 50;
    void paintEvent(QPaintEvent* paintEvent);
    void mousePressEvent(QMouseEvent *event);

private slots:
    //void on_startButton_clicked();
    //void updateslot();

private:
    Ui::BoardWidget *ui;
};

#endif // BOARDWIDGET_H
