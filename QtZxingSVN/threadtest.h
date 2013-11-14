#ifndef THREADTEST_H
#define THREADTEST_H

#include <QThread>
#include <mainwindow.h>

class ThreadTest : public QThread
{
    Q_OBJECT
public:
    explicit ThreadTest(QObject *parent = 0);
    MainWindow* mainWindow;
signals:

public slots:

protected:

     void run();

};

#endif // THREADTEST_H
