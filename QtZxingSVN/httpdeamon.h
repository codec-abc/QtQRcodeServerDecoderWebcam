#ifndef HTTPDEAMON_H
#define HTTPDEAMON_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QRegExp>
#include <QStringList>
#include <QApplication>
#include <mainwindow.h>

class MainWindow;

class HttpDaemon : public QTcpServer
{
    Q_OBJECT
public:
    HttpDaemon(quint16 port, QObject* parent = 0);
    void incomingConnection(int socket);
    void pause();
    void resume();
    void setMainWindow(MainWindow* mainWindow);

public slots:
    void answerRequest(QTcpSocket* socket, QString result);

private slots:
    void readClient();
    void discardClient();

private:
    bool disabled;
    MainWindow* _mainWindow;
};
#endif // HTTPDEAMON_H
