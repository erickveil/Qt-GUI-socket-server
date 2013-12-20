#include "threadmonitor.h"

ThreadMonitor::ThreadMonitor(QThread* watch){

    watch_thread=watch;
    startLoop();
}

void ThreadMonitor::startLoop()
{
    QTimer *mon_loop=new QTimer(this);
    connect(mon_loop, SIGNAL(timeout()), this, SLOT(eventListenerStateMonitor()));
    mon_loop->start(1000);
}

void ThreadMonitor::eventListenerStateMonitor()
{

    bool current_state=watch_thread->isRunning();


    if(current_state==last_state){
        return;
    }

    last_state=current_state;
    emit threadStateChanged(current_state);

}
