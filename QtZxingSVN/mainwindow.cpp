#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <qimagetoluminancesource.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/Exception.h>
#include <zxing/ChecksumException.h>
#include <zxing/ReaderException.h>
#include <QMessageBox>
#include <QZBarImage.h>
#include <QZBar.h>
#include <ImageScanner.h>
#include <threadtest.h>

#define MAXTRYFORDECODING 20

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    useDecode1 = true;
    _httpDaemon = new HttpDaemon(9090);
    _httpDaemon->setMainWindow(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "./", tr("All Files (*.*)"));
    QImage image = QImage(fileName);
    if(useDecode1)
    {
        decodeFile(image);
    }
    else
    {
        decodeFile2(image);
    }
}


QString MainWindow::decodeFile(QImage image)
{
    QImage qimage = image;
    QPixmap pixmap = QPixmap::fromImage(image);
    this->ui->label->setPixmap(pixmap);
    unsigned int width = qimage.width();
    unsigned int height = qimage.height();
    char* raw = new char[width * height];
    for(unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < height; j++)
        {
            QColor color = QColor(qimage.pixel(i,j));
            int value = color.value();
            char outValue = value;
            int horizontalPos = width -1 -i;
            raw[horizontalPos+j*width]=outValue;
        }
    }
    zbar::Image image2(width, height, "Y800", raw, width * height);
    zbar::ImageScanner imageScanner;
    imageScanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    imageScanner.scan(image2);
    QList<QString> result;
    for(zbar::Image::SymbolIterator symbol = image2.symbol_begin();symbol != image2.symbol_end();++symbol)
    {
        QString string = QString::fromStdString(symbol->get_data());
       // std::cout << string.toStdString() << std::endl;
        result.append(string);
        // do something useful with results
        //std::cout << "decoded " << symbol->get_type_name() << " symbol \"" << symbol->get_data() << '"' << std::endl;
    }
    delete[] raw;
    if(result.isEmpty())
    {
        return QString("");
        //QMessageBox::information(this,"No QRCode decoded","No QRCode can be decode from the picture");
    }
    else
    {
        QString appendedResult="";
        for (int i = 0; i < result.length(); i++)
        {
            QString resultI = result.at(i);
        //    std::cout << "ITERATE" << std::endl;
        //    std::cout << "result I = " << resultI.toStdString() << std::endl;
            appendedResult.append(resultI);
        }
        //std::cout << "RESULT" << appendedResult.toStdString() << std::endl;
        return appendedResult;
        //QMessageBox::information(this,"QRCode decoded",appendedResult);
    }
}


QString MainWindow::decodeFile2(QImage image)
{
    QImage qimage = image;
    QPixmap pixmap = QPixmap::fromImage(image);
    this->ui->label->setPixmap(pixmap);
    int width = qimage.width();
    int height = qimage.height();
    QImageToLuminanceSource* lumSource = new QImageToLuminanceSource(qimage,width,height);
    zxing::Ref<zxing::LuminanceSource> refSource = zxing::Ref<QImageToLuminanceSource>(lumSource);
    zxing::HybridBinarizer* hybridBinazer = new zxing::HybridBinarizer(refSource);
    zxing::Ref<zxing::Binarizer> refBinazer = zxing::Ref<zxing::HybridBinarizer>(hybridBinazer);
    zxing::BinaryBitmap* binaryBitmap = new zxing::BinaryBitmap(refBinazer);
    zxing::Ref<zxing::BinaryBitmap> refBitmap = zxing::Ref<zxing::BinaryBitmap>(binaryBitmap);
    zxing::qrcode::QRCodeReader* reader = new zxing::qrcode::QRCodeReader();
    zxing::DecodeHints hints = zxing::DecodeHints::DEFAULT_HINT;
    hints.setTryHarder(true);
#define trycatch
#ifdef trycatch
    try
    {
        zxing::Ref<zxing::Result> result = reader->decode(refBitmap,hints);
        std::cout << "reading did not crash" << std::endl;
        zxing::Ref<zxing::String> string = result->getText();
        std::string stringResult = string->getText();
        std::cout << "result is " << stringResult << std::endl;
        return QString::fromStdString(stringResult);
        //QMessageBox::information(this,"QRCode decoded",QString(stringResult.c_str()));
    }
    catch (zxing::ChecksumException e)
    {
        //QMessageBox::critical(this,"ChecksumException",QString(e.what()));
        std::string message(e.what());
        std::cout << "something wrong happened with checksum" << std::endl;
        std::cout << message << std::endl;
        return QString("");
    }
    catch (zxing::ReaderException e)
    {
        //QMessageBox::critical(this,"ReaderExecption",QString(e.what()));
        std::string message(e.what());
        std::cout << "something wrong happened with the reader" << std::endl;
        std::cout << message << std::endl;
        return QString("");
    }
#ifdef alwayscatch
    catch (zxing::Exception e)
    {
        std::string message(e.what());
        std::cout << "something wrong happened" << std::endl;
        std::cout << message << std::endl;
        return QString("");
    }
#endif
#else
        zxing::Ref<zxing::Result> result = reader->decode(refBitmap,hints);
        std::cout << "reading did not crash" << std::endl;
        zxing::Ref<zxing::String> string = result->getText();
        std::string stringResult = string->getText();
        std::cout << "result is " << stringResult << std::endl;
        //QMessageBox::information(this,"QRCode decoded",QString(stringResult.c_str()));
        return QString(stringResult);
#endif

}

void MainWindow::on_pushButton_2_clicked()
{
    static bool init = false;
    if (!init)
    {
        QByteArray cameraDevice;

        QActionGroup *videoDevicesGroup = new QActionGroup(this);
        videoDevicesGroup->setExclusive(true);
        int i = 0;
        foreach(const QByteArray &deviceName, QCamera::availableDevices())
        {
            QString number = QString::number(i);
            std::cout << "going through webcam : " << number.toStdString() << std::endl;
            if(i == 0)
            {
                cameraDevice = deviceName;
            }
            QString someString(cameraDevice);
            i++;
            //this->ui->textEdit->append(someString);
        }
        m_camera = new QCamera(cameraDevice);
        // QVideoWidget* someWidget = new QVideoWidget();
        //someWidget->show();
        m_camera->setViewfinder(this->ui->widget);
        //this->ui->textEdit->append(QString::number(m_camera->availability()));
        m_camera->load();

        m_imageCapture = new QCameraImageCapture(m_camera);
        //QImageEncoderSettings encoder_setting = m_imageCapture->encodingSettings();
        //encoder_setting.setCodec("image/jpeg");
        //encoder_setting.setQuality(QtMultimedia::NormalQuality);
        //encoder_setting.setResolution(800,600);
        //m_imageCapture->setEncodingSettings(encoder_setting);

        //connect(m_imageCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(processSavedImage(int,QString)));
        connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)), this,SLOT(processCapturedImage(int,QImage)));
        m_camera->start();
        init = true;
    }
    //m_imageCapture->capture();
    ThreadTest* test = new ThreadTest();
    test->mainWindow = this;
    test->start();

}

void MainWindow::processSavedImage(int a,QString string)
{

}

void MainWindow::processCapturedImage(int a, QImage image2)
{
    QImage image = image2.scaled(600, 600,Qt::KeepAspectRatio);
    static int nb = 0;
    if(!(listOfCallBack.contains(a)))
    {
        listOfCallBack.insert(a);
        QString result;
        if(useDecode1)
        {
            result = decodeFile(image);
        }
        else
        {
            result = decodeFile2(image);
        }
        if (!(result.isEmpty()))
        {
            for (int i = 0; i < _requests.size(); i++)
            {
                this->_httpDaemon->answerRequest(_requests[0].socket,result);
                _requests.remove(0);
            }
        }
        else
        {
            for (int i = _requests.size() - 1; i >= 0; i--)
            {
                _requests[i].nbTries++;
                if(_requests[i].nbTries > MAXTRYFORDECODING)
                {
                    this->_httpDaemon->answerRequest(_requests[i].socket,QString("decoding fail"));
                    _requests.remove(i);
                }
            }
        }
    }
}


void MainWindow::addRequest(QTcpSocket* socket)
{
    request a;
    a.nbTries = 0;
    a.socket = socket;
    this->_requests.push_back(a);
}

void MainWindow::processRequests()
{

}
