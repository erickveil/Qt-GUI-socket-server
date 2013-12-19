#ifndef THREADMONITOR_H
#define THREADMONITOR_H

#include <QObject>
#include <QTimer>

#include "QMutex"
#include "QThread"
#include "QTcpServer"
#include "QTcpSocket"
#include "QVector"
#include "QString"
#include "unistd.h"

class ThreadMonitor:public QObject
{
    Q_OBJECT

public:
    ThreadMonitor(QThread* watch);
    void startLoop();

private:
    bool last_state;
    QThread *watch_thread;

public slots:
    void eventListenerStateMonitor();

signals:
    void threadStateChanged(bool);
};

#endif // THREADMONITOR_H
