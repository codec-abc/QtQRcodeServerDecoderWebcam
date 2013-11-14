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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    useDecode1 = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "./", tr("All Files (*.*)"));
    if(useDecode1)
    {
        decodeFile(fileName);
    }
    else
    {
        decodeFile2(fileName);
    }


}


void MainWindow::decodeFile(QString fileName)
{
    QImage qimage(fileName);
    QPixmap pixmap(fileName);
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
        result.append(QString::fromStdString(symbol->get_data()));
        // do something useful with results
        //std::cout << "decoded " << symbol->get_type_name() << " symbol \"" << symbol->get_data() << '"' << std::endl;
    }
    delete[] raw;
    if(result.isEmpty())
    {
        //QMessageBox::information(this,"No QRCode decoded","No QRCode can be decode from the picture");
    }
    else
    {
        QString appendedResult="";
        for (int i = 0; i < result.length(); i++)
        {
            appendedResult.append(result.at(i));
        }
        QMessageBox::information(this,"QRCode decoded",appendedResult);
    }
}


void MainWindow::decodeFile2(QString fileName)
{
    QImage qimage(fileName);
    QPixmap pixmap(fileName);
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
        QMessageBox::information(this,"QRCode decoded",QString(stringResult.c_str()));
    }
    catch (zxing::ChecksumException e)
    {
        QMessageBox::critical(this,"ChecksumException",QString(e.what()));
        std::string message(e.what());
        std::cout << "something wrong happened with checksum" << std::endl;
        std::cout << message << std::endl;
    }
    catch (zxing::ReaderException e)
    {
        QMessageBox::critical(this,"ReaderExecption",QString(e.what()));
        std::string message(e.what());
        std::cout << "something wrong happened with the reader" << std::endl;
        std::cout << message << std::endl;
    }
#ifdef alwayscatch
    catch (zxing::Exception e)
    {
        std::string message(e.what());
        std::cout << "something wrong happened" << std::endl;
        std::cout << message << std::endl;
    }
#endif
#else
        zxing::Ref<zxing::Result> result = reader->decode(refBitmap,hints);
        std::cout << "reading did not crash" << std::endl;
        zxing::Ref<zxing::String> string = result->getText();
        std::string stringResult = string->getText();
        std::cout << "result is " << stringResult << std::endl;
        QMessageBox::information(this,"QRCode decoded",QString(stringResult.c_str()));
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

        connect(m_imageCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(processSavedImage(int,QString)));

        m_camera->start();
        init = true;
    }
    m_imageCapture->capture();
}

void MainWindow::processSavedImage(int a,QString string)
{
    if(!(listOfCallBack.contains(a)))
    {
        listOfCallBack.insert(a);
        std::cout << "get call back : " << QString::number(a).toStdString() <<  std::endl;
        QString fileName = "img.jpg";
        if(useDecode1)
        {
            decodeFile(fileName);
        }
        else
        {
            decodeFile2(fileName);
        }
    }
    QSetIterator<int> i(listOfCallBack);
    while (i.hasNext())
        qDebug() << i.next();
}
