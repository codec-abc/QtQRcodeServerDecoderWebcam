#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QCamera>
#include <QCameraImageCapture>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void decodeFile(QString fileName);
    void decodeFile2(QString fileName);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void processSavedImage(int a,QString string);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    bool useDecode1;
    Ui::MainWindow *ui;
    QCamera* m_camera;
    QCameraImageCapture* m_imageCapture;
    QSet<int> listOfCallBack;
};

#endif // MAINWINDOW_H
