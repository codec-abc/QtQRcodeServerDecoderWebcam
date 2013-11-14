#ifndef QIMAGETOLUMINANCESOURCE_H
#define QIMAGETOLUMINANCESOURCE_H

#include <zxing/LuminanceSource.h>
#include <zxing/common/Array.h>
#include <QImage>

class QImageToLuminanceSource : public zxing::LuminanceSource
{
public:
    QImageToLuminanceSource(QImage image,int width,int height);
    virtual zxing::ArrayRef<char> getRow(int y, zxing::ArrayRef<char> row) const;
    virtual zxing::ArrayRef<char> getMatrix() const;
private:
    QImage _image;
    int _width;
    int _height;

};

#endif // QIMAGETOLUMINANCESOURCE_H
