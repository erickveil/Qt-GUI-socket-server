#ifndef SOCKLISTEN_H
#define SOCKLISTEN_H

#include <QObject>

#include "QMutex"
#include "QThread"
#include "QTcpServer"
#include "QTcpSocket"
#include "QVector"
#include "QString"
#include "unistd.h"

class SockListen:public QObject
{
    Q_OBJECT
public:
    SockListen(QThread *listen_thread);
    ~SockListen(){server->close();}
    //void setState(bool onoff){listen_state=onoff;}
    void listenLoop();
    ushort port;
    QTcpServer *server;

private:
    //bool listen_state;
    void getSocketdata(QTcpSocket * client);
    QString readLine(QTcpSocket *client);
    void writeResponse(QString line);
    int waitForInput( QTcpSocket *socket );

signals:
    void receivedLine(QString);

public slots:
    void eventListenThreadStarted();
    void eventNewConnection();
    void eventListenThreadFinished();
    void eventListenThreadTerminated();
};

#endif // SOCKLISTEN_H
