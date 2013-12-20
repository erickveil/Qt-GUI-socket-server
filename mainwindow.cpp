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
    listen_thread.exit();
    //monitor_thread.exit();

    //monitor_thread.wait();
    listen_thread.wait();

    delete ui;
}

/**
 * @brief MainWindow::initThreads
 * Sets up two objects, and assigns each to it sown thread.
 * The listener gets its own thread so that its state can be monitored and controlled
 * by turning the thread on and off.
 */
void MainWindow::initThreads()
{    
    statusBar()->showMessage("Initializing..");

    listener_obj=new SockListen(&listen_thread);
    listener_obj->moveToThread(&listen_thread);
    connect ( listener_obj, SIGNAL(receivedLine(QString)), this, SLOT(eventDataObtainedFromSocket(QString)) );

    monitor_obj=new ThreadMonitor(&listen_thread);
    connect ( monitor_obj, SIGNAL(threadStateChanged(bool)), this, SLOT(eventListenerStateChange(bool)) );
    //monitor_obj->moveToThread(&monitor_thread);
    //monitor_thread.start();
}

void MainWindow::on_MainWindow_destroyed()
{

}

/**
 * @brief MainWindow::on_bu_run_clicked
 * Clicking the button checks the state of the listener thread and taggles that.
 * So if the thread was closed otherwise, we still toggle correctly.
 */
void MainWindow::on_bu_run_clicked()
{
    if(listen_thread.isRunning()){
        listen_thread.exit();
    }
    else{
        listener_obj->port=this->ui->tb_port->text().toUShort();
        listen_thread.start();
    }

}

/**
 * @brief MainWindow::eventListenerStateChange
 * @param state
 * [SLOT]
 * Receives signals from the thread monitor to set the UI text to match.
 */
void MainWindow::eventListenerStateChange(bool state)
{
    if(!state){
        statusBar()->showMessage("Stopped.");
    }
    else{
        statusBar()->showMessage("Running");
    }

}

/**
 * @brief MainWindow::eventDataObtainedFromSocket
 * @param socket_string
 * [SLOT]
 * Receives the data from the socket to set it in the UI dsplay,
 * allowing GUI property control from the socke.
 */
void MainWindow::eventDataObtainedFromSocket(QString socket_string)
{
    this->ui->tb_out->setPlainText(socket_string);
}
