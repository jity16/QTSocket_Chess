#include "boardwidget.h"
#include "ui_boardwidget.h"
#include <iostream>
#include <QDebug>
#include <QPolygon>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <QtMultimedia/QSound>
#include <QPainter>
#include <QFont>

BoardWidget::BoardWidget(QWidget *parent, GameWidget *gameWidget) :
    QWidget(parent),
    ui(new Ui::BoardWidget)
{
    ui->setupUi(this);
    this->gameWidget = gameWidget;

   /* timer = new QTimer;
    ui->TimeLCD->setDigitCount(4);
    time = QTime(0,0,0);
    counting_time = time.addSecs(time_limit);
    qDebug()<<counting_time.toString("mm:ss");
    ui->TimeLCD->display(counting_time.toString("mm:ss"));
    connect(timer,SIGNAL(timeout()),this,SLOT(updatesolt()));*/
}

BoardWidget::~BoardWidget()
{
    delete ui;
}

void BoardWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter p(this);
    QPen pen;


    /*draw background*/
    float h = height()/10.0;
    float w = h;

    for(int i = 0; i < 8; i ++)//8列
        for(int j = 0; j < 9; j ++){//9行
            QPolygonF polygon;
            polygon << QPointF(offset_x+w * i,offset_y+ h * j) << QPointF( offset_x+w * (i + 1),offset_y+ h * j)
                    << QPointF( offset_x+w * (i + 1),offset_y+ h * (j + 1)) << QPointF( offset_x+w * i,offset_y+ h * (j + 1));
            p.setPen(Qt::NoPen);
            if(j != 4){
                pen.setColor(QColor(205,92,92));
                pen.setWidth(3);
                p.setPen(pen);
            }
            p.drawPolygon(polygon);
            // cur
            /*if (game->curX == j && game->curY == i)
            {
                p.setBrush(QColor(234, 143, 201));
                //p.setPen(QPen(Qt::red, 1));
                p.drawPolygon(polygon);
            }*/
        }
    p.drawLine(QPointF(offset_x+w * 3,offset_y), QPointF(offset_x+w * 5,offset_y+h*2));
    p.drawLine(QPointF(offset_x+w * 3,offset_y+h*2), QPointF(offset_x+w * 5,offset_y));
    p.drawLine(QPointF(offset_x+w * 3,offset_y+h*7), QPointF(offset_x+w * 5,offset_y+h*9));
    p.drawLine(QPointF(offset_x+w * 3,offset_y+h*9), QPointF(offset_x+w * 5,offset_y+h*7));
    QFont ft("华康娃娃体W5", 30);
    ft.setItalic(true);
    p.setFont(ft);
    p.drawText(offset_x + w,offset_y + h*4+70,tr("楚"));
    p.drawText(offset_x+w * 2,offset_y + h*4+70,tr("河"));
    p.drawText(offset_x+w*5,offset_y+ h*4+70,tr("汉"));
    p.drawText(offset_x+w*6,offset_y+ h*4+70,tr("界"));

    qDebug()<<"--------------------";
    if (!game)
         qDebug() <<"BoardWidget : game == nullptr";




    /*for(int i = 0; i < 10; i ++)
        for(int j = 0; j < 9; j ++){
           if(game->getChess(i,j)!= 0)
           qDebug()<<"i = " << i <<" j = " <<j <<" "<<game->getChess(i,j)<<" "<<game->getType(i,j);
        }*/


    qDebug()<<"--------------------";
    /* load black chess image */
    QPixmap bzu = QPixmap::fromImage(QImage(":/node/bzu.png"));
    QPixmap bpao = QPixmap::fromImage(QImage(":/node/bpao.png"));
    QPixmap bju = QPixmap::fromImage(QImage(":/node/bju.png"));
    QPixmap bma = QPixmap::fromImage(QImage(":/node/bma.png"));
    QPixmap bxiang = QPixmap::fromImage(QImage(":/node/bxiang.png"));
    QPixmap bsi = QPixmap::fromImage(QImage(":/node/bsi.png"));
    QPixmap bjiang = QPixmap::fromImage(QImage(":/node/bjiang.png"));



    /* load red chess image */
    QPixmap hbin =  QPixmap::fromImage(QImage(":/node/hbin.png"));
    QPixmap hpao =  QPixmap::fromImage(QImage(":/node/hpao.png"));
    QPixmap hju = QPixmap::fromImage(QImage(":/node/hju.png"));
    QPixmap hma = QPixmap::fromImage(QImage(":/node/hma.png"));
    QPixmap hxiang = QPixmap::fromImage(QImage(":/node/hxiang.png"));
    QPixmap hsi = QPixmap::fromImage(QImage(":/node/hsi.png"));
    QPixmap hshuai = QPixmap::fromImage(QImage(":/node/hshuai.png"));
    /* draw chesses */
    for(int i = 0; i < 10; i ++)
        for( int j = 0; j < 9; j ++){
            //画我方棋子
            if(game->getChess(i,j)== 1){//该处棋子为我方棋子
                if(game ->getType(i,j) == 6){//该处棋子为卒
                   // qDebug()<<"draw red bin";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, hbin);
                }
                if(game ->getType(i,j) == 5){//该处棋子为炮
                  //  qDebug()<<"draw red pao";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, hpao);
                }
                if(game ->getType(i,j) == 4){//该处棋子为车
                  //  qDebug()<<"draw red ju";
                     p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, hju);
                }
                if(game ->getType(i,j) == 3){//该处棋子为马
                  //  qDebug()<<"draw red ma";
                     p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, hma);
                }
                if(game ->getType(i,j) == 2){//该处棋子为象
                   // qDebug()<<"draw red xiang";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, hxiang);
                }
                if(game ->getType(i,j) == 1){//该处棋子为士
                  //  qDebug()<<"draw red si";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, hsi);
                }
                if(game ->getType(i,j) == 0){//该处棋子为帅
                   // qDebug()<<"draw red shuai";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, hshuai);
                }
            }
            //画对方棋子
            if(game->getChess(i,j)== 2){//该处棋子为我方棋子
                if(game ->getType(i,j) == 6){//该处棋子为卒
                   // qDebug()<<"draw black zu";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, bzu);
                }
                if(game ->getType(i,j) == 5){//该处棋子为炮
                  //  qDebug()<<"draw black pao";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, bpao);
                }
                if(game ->getType(i,j) == 4){//该处棋子为车
                   // qDebug()<<"draw black ju";
                     p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, bju);
                }
                if(game ->getType(i,j) == 3){//该处棋子为马
                   // qDebug()<<"draw black ma";
                     p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, bma);
                }
                if(game ->getType(i,j) == 2){//该处棋子为象
                  //  qDebug()<<"draw black xiang";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, bxiang);
                }
                if(game ->getType(i,j) == 1){//该处棋子为士
                   // qDebug()<<"draw black si";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, bsi);
                }
                if(game ->getType(i,j) == 0){//该处棋子为帅
                  //  qDebug()<<"draw black jiang";
                    p.drawPixmap(offset_x+ w * j - w *0.5 , offset_y + h * i - h *0.5 , w, w, bjiang);
                }
            }
     }
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)//左键是选择棋子
    {
        qDebug()<<"mouse_event: "<<event->x()<<" "<<event->y();
        if (!(game->status == 1))
        {
            QMessageBox::information(this, "不可行操作", "当前是对方落子时间");
            return;
        }
        float h = height() / 10.0;
        float w = h;
        int i = (event->y() + h*0.5 - offset_y)/h;
        int j = (event->x() + w * 0.5 - offset_x)/w;
        qDebug() << "mouse press at (" << i << "," << j << ")";
        //game->curstep = 0;
        game->curX = i;
        game->curY = j;
      /* if (!game->chooseChess(i, j))
       {
           QMessageBox::information(this, "不可行操作", "选中不可移动棋子，或选中不可移动到的位置");
       }*/
       //game->curX = i;
       //curY = j;

      /* else{
            game->curstep++;
            if (game->curstep >= 2){//game->ways[0].size())
                gameWidget->setStatus(2);
                game->freshAvailable();
                game->curstep =0;
                qDebug()<<"setStatus changed!";
            }
            update();
       }*/

    }
    else if(event-> button() == Qt::RightButton){//右键是选择位置
        qDebug()<<"mouse_event: "<<event->x()<<" "<<event->y();
        if (!(game->status == 1))
        {
            QMessageBox::information(this, "不可行操作", "当前是对方落子时间");
            return;
        }
        float h = height() / 10.0;
        float w = h;
        int i = (event->y() + h*0.5 - offset_y)/h;
        int j = (event->x() + w * 0.5 - offset_x)/w;
        qDebug() << "mouse press at (" << i << "," << j << ")";
        //game->curstep = 1;
        if (!game->chooseChess(i, j))
        {
           QMessageBox::information(this, "不可行操作", "选中不可移动棋子，或选中不可移动到的位置");
        }
        else{
           //game->curstep++;
           //if (game->curstep >= 2){//game->ways[0].size())
               // game->tie();//判断棋局
           //if (game->lose == 1)
             //   gameWidget->setStatus(1);
           //else if (game->lose == 2)
             //   gameWidget->setStatus(0);
               // gameWidget->judge();//获得正负情况
                gameWidget->setStatus(2);
                game->freshAvailable();
                game->curstep =0;
                qDebug()<<"setStatus changed!";
           // }
           this->repaint();
           //update();
       }
    }
    else
    {
        QWidget::mousePressEvent(event);
    }
}

/*void BoardWidget::on_startButton_clicked()
{
    setUpdatesEnabled(false);
    qDebug()<<"###########";
    timer->start(1000);
    qDebug()<<"time: "<<game->cur_time;
}
void BoardWidget::updateslot()
{
   counting_time= counting_time.addSecs(-1);
   game -> cur_time = counting_time.second();
   qDebug()<<"time: "<<game->cur_time;
   //time = QTime(0,0,0);
   //counting_time = time.addSecs(game->cur_time);
   ui->TimeLCD->display(counting_time.toString("mm:ss"));
}*/
