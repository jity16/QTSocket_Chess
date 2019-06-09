#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H
#include <QWidget>
#include <QtNetwork>

class LoginWidget;

#include "mainwindow.h"
#include "game.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0, MainWindow *mainWindow = 0);
    ~LoginWidget();
    void clear();

private:
    Ui::LoginWidget *ui;
    QTcpServer *listenSocket;
    QTcpSocket *readWriteSocket;
    MainWindow *mainWindow;

signals:
    void gotoGame(QTcpSocket *socket, Game *game);

private slots:
    void onServerConnect();
    void onClientConnect();
    void accecptConnection();
    void finishConnection();
};

#endif // LOGINWIDGET_H
