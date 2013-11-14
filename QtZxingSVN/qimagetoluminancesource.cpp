#include "qimagetoluminancesource.h"
#include <QColor>


QImageToLuminanceSource::QImageToLuminanceSource(QImage image,int width, int height) : LuminanceSource(width,height)
{
    _image=image;
    _width=width;
    _height=height;
}



zxing::ArrayRef<char> QImageToLuminanceSource::getRow(int y, zxing::ArrayRef<char> row) const
{
    zxing::ArrayRef<char> returnArray = zxing::ArrayRef<char>(_width);

    for(int i = 0; i < _width; i++)
    {
        int currentPixelValue = qGray(_image.pixel(i,y));
        if(currentPixelValue < 0)
        {
            std::cout << "pixel is below 0" << std::endl;
        }
        if (currentPixelValue > 255)
        {
            std::cout << "pixel is above 255" << std::endl;
        }
        returnArray[i]=currentPixelValue;
    }
    return returnArray;
}

zxing::ArrayRef<char> QImageToLuminanceSource::getMatrix() const
{
    zxing::ArrayRef<char> returnArray = zxing::ArrayRef<char>(_width*_height);
    for(int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _height; j++)
        {
            int currentPixelValue = qGray(_image.pixel(i,j));
            QColor color = QColor(_image.pixel(i,j));
            int value = color.value();

            QString pixeli = QString::number(i);
            QString pixelj = QString::number(j);
            QString pixelValue = QString::number(value);

            //std::cout << "pixel (" << pixeli.toStdString() << "," << pixelj.toStdString() << ") is : " << pixelValue.toStdString() << std::endl;
            char outValue = value;
            //outValue = outValue && 0xFF;
            int horizontalPos = _width-1 - i;
            returnArray[horizontalPos+j*_width]=value;
        }
    }
    return returnArray;
}
