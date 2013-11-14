#include "threadtest.h"
#include <QWaitCondition>
#include <QMutex>

ThreadTest::ThreadTest(QObject *parent) :
    QThread(parent)
{
}

void ThreadTest::run()
{
    while(true)
    {
        mainWindow->m_imageCapture->capture();
        //QWaitCondition w;
        //w.wait(new QMutex(),300);
        this->msleep(100);
    }
}
