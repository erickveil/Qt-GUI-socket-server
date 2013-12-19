#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initThreads();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initThreads()
{    
    statusBar()->showMessage("Initializing..");

    listener_obj=new SockListen(&listen_thread);
    listener_obj->moveToThread(&listen_thread);
    //listen_thread.start();

    monitor_obj=new ThreadMonitor(&listen_thread);
    connect ( monitor_obj, SIGNAL(threadStateChanged(bool)), this, SLOT(eventListenerStateChange(bool)) );
    monitor_obj->moveToThread(&monitor_thread);
    monitor_thread.start();
}

void MainWindow::on_MainWindow_destroyed()
{
    //monitor_thread.exit();
    //listen_thread.exit();
}

void MainWindow::on_bu_run_clicked()
{
    if(listen_thread.isRunning()){
        listener_obj->setState(false);
        listen_thread.exit();
    }
    else{
        listen_thread.start();
    }

}


void MainWindow::eventListenerStateChange(bool state)
{
    if(!state){
        statusBar()->showMessage("Stopped.");
    }
    else{
        statusBar()->showMessage("Running");
    }

}
