#ifndef PICTUREINFO_H
#define PICTUREINFO_H

#include <QString>

class PictureInfo
{
public:
    PictureInfo();
    void setFile(const QString &filename);
    QString getFile();
    void setValid(bool valid);
    bool getValid();
    int getWidth();
    int getHeight();
    void setDimensions(int width, int height);
    void setColor(unsigned char red, unsigned char green, unsigned char blue);
private:
    QString filename;
    bool valid;
    int width;
    int height;
    unsigned char red, green, blue;
};

#endif // PICTUREINFO_H
