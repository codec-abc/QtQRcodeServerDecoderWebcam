#include "httpdeamon.h"
#include <iostream>
#include <QThread>

HttpDaemon::HttpDaemon(quint16 port, QObject* parent)
    : QTcpServer(parent), disabled(false)
{
    listen(QHostAddress::Any, port);
}

void HttpDaemon::incomingConnection(int socket)
{
    //qDebug() << "incomingConnection thread : " << QThread::currentThreadId();
    static int nb = 0;
    if (disabled)
        return;
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
    nb++;
}

void HttpDaemon::pause()
{
    disabled = true;
}

void HttpDaemon::resume()
{
    disabled = false;
}


void HttpDaemon::readClient()
{
    if (disabled)
        return;
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine())
    {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET")
        {
            this->_mainWindow->addRequest(socket);
            this->_mainWindow->processRequests();
        }
    }
}
void HttpDaemon::discardClient()
{
    std::cout << "discardClient" << std::endl;
    QTcpSocket* socket = (QTcpSocket*)sender();
    //socket->deleteLater();
}

void HttpDaemon::setMainWindow(MainWindow* mainWindow)
{
    this->_mainWindow = mainWindow;
}

void HttpDaemon::answerRequest(QTcpSocket* socket, QString result)
{
   // qDebug() << "answerRequest thread " << QThread::currentThreadId();
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    /*
    os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n"
        "<h1>coucou</h1>\n";
     //   << QDateTime::currentDateTime().toString() << "\n";
     */
    QString returnHtml = " <!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\"><HTML><HEAD></HEAD><BODY><P>";
    returnHtml.append(result);
    returnHtml.append("</P></BODY></HTML>");
    os << "HTTP/1.0 200 Ok\r\n"
          "Content-Type: text/html; charset=\"utf-8\"\r\n"
          "\r\n"
       << result << "\n";
    socket->close();
    if (socket->state() == QTcpSocket::UnconnectedState)
    {
        delete socket;
    }
}
