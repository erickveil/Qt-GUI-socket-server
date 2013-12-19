#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "socklisten.h"
#include "threadmonitor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QThread listen_thread;
    QThread monitor_thread;

    SockListen *listener_obj;
    ThreadMonitor *monitor_obj;

public slots:
    void eventListenerStateChange(bool state);
    void eventDataObtainedFromSocket(QString socket_string);

private slots:
    void on_MainWindow_destroyed();

    void on_bu_run_clicked();

private:
    Ui::MainWindow *ui;
    void initThreads();
};

#endif // MAINWINDOW_H
