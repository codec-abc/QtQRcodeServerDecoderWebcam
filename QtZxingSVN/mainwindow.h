#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QCamera>
#include <QCameraImageCapture>
#include <httpdeamon.h>

typedef struct
{
    QTcpSocket* socket;
    int nbTries;
} request;

class HttpDaemon;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString decodeFile(QImage image);
    QString decodeFile2(QImage image);
    explicit MainWindow(QWidget *parent = 0);
    void addRequest(QTcpSocket* socket);
    void processRequests();
    QCameraImageCapture* m_imageCapture;
    ~MainWindow();

private slots:
    void processSavedImage(int a,QString string);
    void processCapturedImage(int a, QImage image);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    bool useDecode1;
    Ui::MainWindow *ui;
    QCamera* m_camera;

    QSet<int> listOfCallBack;
    HttpDaemon* _httpDaemon;
    QVector<request> _requests;
};

#endif // MAINWINDOW_H
