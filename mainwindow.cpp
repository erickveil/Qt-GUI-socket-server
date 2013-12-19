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
    // shut down the threads before leaving
    listener_obj->setState(false);
    listen_thread.exit();
    monitor_thread.exit();

    monitor_thread.wait();
    listen_thread.wait();

    delete ui;
}

void MainWindow::initThreads()
{    
    statusBar()->showMessage("Initializing..");

    listener_obj=new SockListen(&listen_thread);
    listener_obj->moveToThread(&listen_thread);
    connect ( listener_obj, SIGNAL(receivedLine(QString)), this, SLOT(eventDataObtainedFromSocket(QString)) );
    //listen_thread.start();

    monitor_obj=new ThreadMonitor(&listen_thread);
    connect ( monitor_obj, SIGNAL(threadStateChanged(bool)), this, SLOT(eventListenerStateChange(bool)) );
    monitor_obj->moveToThread(&monitor_thread);
    monitor_thread.start();
}

void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::on_bu_run_clicked()
{
    if(listen_thread.isRunning()){
        listener_obj->setState(false);
        listen_thread.exit();
    }
    else{
        listener_obj->port=this->ui->tb_port->text().toUShort();
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

void MainWindow::eventDataObtainedFromSocket(QString socket_string)
{
    this->ui->tb_out->setPlainText(socket_string);
}
