/**
  * cstrings __PRETTY_FUNCTION__ is gcc compiler specific
  */
#include "socklisten.h"

/**
 * @brief SockListen::SockListen
 * @param listen_thread
 */
SockListen::SockListen(QThread *listen_thread)
{
    connect ( listen_thread, SIGNAL(started()), this, SLOT(eventListenThreadStarted()) );
    connect ( listen_thread, SIGNAL(finished()), this, SLOT(eventListenThreadFinished()) );
}

/**
 * @brief SockListen::startListener
 * Starts aQTcpServer, which listens on the thread loop and emits a signal
 * whenever it receives data.
 */
void SockListen::startListener()
{
    server=new QTcpServer();

    connect( server, SIGNAL(newConnection()), this, SLOT(eventNewConnection()) );

    if(!server->listen(QHostAddress::Any,port)){
        printf("%s: listen operation on port %d failed\n",__PRETTY_FUNCTION__,port);
    }
    else
    {
        printf("%s: now listening on port %d\n",__PRETTY_FUNCTION__,port);
    }
}

/**
 * @brief SockListen::eventNewConnection
 * [SLOT]
 * Receives the connected signal from QTcpSocket.
 * Starts scooping data from the socket.
 */
void SockListen::eventNewConnection()
{
    printf("%s: got a TCP connection\n",__PRETTY_FUNCTION__);
    QTcpSocket *client=server->nextPendingConnection();
    getSocketdata(client);
}

/**
 * @brief SockListen::getSocketdata
 * @param client
 * Calls readline and emits the data back to the UI in a signal.
 */
void SockListen::getSocketdata(QTcpSocket *client)
{
    QString line;
    line=readLine(client);
    emit receivedLine(line);

    if (line.length()>0){
        writeResponse(line);
    }
}

/**
 * @brief SockListen::readLine
 * @param socket
 * @return
 * Empty's the socket of its data, one byte at a time,
 * converting each to a character and appending it to a string,
 * until there are none left to be read.
 */
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

/**
 * @brief SockListen::writeResponse
 * @param line
 *This is where you define what you do with socket data.
 *Here, we log it, then emit it in a signal for the UI to use.
 */
void SockListen::writeResponse(QString line)
{
    printf("%s: Obtained from socket: %s",__PRETTY_FUNCTION__,line.toStdString().c_str());
    emit receivedLine(line);
}

/**
 * @brief SockListen::waitForInput
 * @param socket
 * @return
 *Blocks between a client connection, and the delivery of bytes from the kernel.
 */
int SockListen::waitForInput( QTcpSocket *socket )
{
    int bytesAvail = -1;

        if (socket->waitForReadyRead( 10000 )) {
            bytesAvail = socket->bytesAvailable();
        }
        else{
            printf("%s: Connection timed out with no data.",__PRETTY_FUNCTION__);
        }

    return bytesAvail;
}

/**
 * @brief SockListen::eventListenThreadStarted
 *[SLOT]
 * When the thread starts, so does the listener.
 */
void SockListen::eventListenThreadStarted()
{
    startListener();
}

/**
 * @brief SockListen::eventListenThreadFinished
 *[SLOT]
 * When the thread ends, so does the listener.
 */
void SockListen::eventListenThreadFinished()
{
    server->close();
}

/**
 * @brief SockListen::eventListenThreadTerminated
 *[SLOT]
 * I've noticed a non-fatal warning thrown that insists that the
 * terminated signal does not exist in QThread, despite its documentation.
 * This slot remains unconnected.
 */
void SockListen::eventListenThreadTerminated()
{
    server->close();
}

