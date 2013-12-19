/**
  * cstrings __PRETTY_FUNCTION__ is gcc compiler specific
  */
#include "socklisten.h"

SockListen::SockListen(QThread *listen_thread)
{
    listen_state=true;

    connect ( listen_thread, SIGNAL(started()), this, SLOT(eventListenThreadStarted()) );
}

void SockListen::listenLoop()
{
    QTcpServer server;

    if(server.listen(QHostAddress::Any,port)){
        printf("%s: listen() succeeded\n",__PRETTY_FUNCTION__);

        while(server.isListening() && listen_state)
        {

            if(server.waitForNewConnection(100)){
                printf("%s: got a TCP connection\n",__PRETTY_FUNCTION__);
                QTcpSocket *client=server.nextPendingConnection();
                getSocketdata(client);
            }

            else{
                usleep(100000);
            }
        }
        printf("%s: listen state interrupted.",__PRETTY_FUNCTION__);
    }
    else{
        printf("%s: listen operation failed",__PRETTY_FUNCTION__);
    }
}

void SockListen::getSocketdata(QTcpSocket *client)
{
    QString line;
    line=readLine(client);
    emit receivedLine(line);

    if (line.length()>0){
        writeResponse(line);
    }
}

QString SockListen::readLine(QTcpSocket *socket)
{
    QString line="";
    int bytesAvail = waitForInput( socket );

    if (bytesAvail > 0) {
        int cnt = 0;
        bool endOfLine = false;
        bool endOfStream = false;

        while (cnt < bytesAvail && (!endOfLine) && (!endOfStream)) {
            char ch;
            int bytesRead = socket->read(&ch, sizeof(ch));

            if (bytesRead == sizeof(ch)) {
                cnt++;
                line.append( ch );
                if (ch == '\r') {
                    endOfLine = true;
                }
            }
            else {
                endOfStream = true;
            }
        }
    }
    return line;
}

void SockListen::writeResponse(QString line)
{
    printf("%s: Obtained from socket: %s",__PRETTY_FUNCTION__,line.toStdString().c_str());
}

int SockListen::waitForInput( QTcpSocket *socket )
{
    int bytesAvail = -1;

    while (socket->state() == QAbstractSocket::ConnectedState && listen_state && bytesAvail < 0) {

        if (socket->waitForReadyRead( 100 )) {
            bytesAvail = socket->bytesAvailable();
        }
        else {
            // Altered from windows.h Sleep()
            usleep( 500000 );
        }

    }

    return bytesAvail;
}

void SockListen::eventListenThreadStarted()
{
    listen_state=true;
    listenLoop();
}

