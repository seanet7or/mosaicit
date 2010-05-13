#include "pictureinfo.h"

PictureInfo::PictureInfo()
{
    valid = false;
    filename = "";
}

void PictureInfo::setFile(const QString &filename)
{
    this->filename = filename;
    valid = false;
}

QString PictureInfo::getFile()
{
    return this->filename;
}

void PictureInfo::setValid(bool valid)
{
    this->valid = valid;
}

bool PictureInfo::getValid()
{
    return this->valid;
}

int PictureInfo::getHeight()
{
    return this->height;
}

int PictureInfo::getWidth()
{
    return this->width;
}

void PictureInfo::setDimensions(int width, int height)
{
    this->width = width;
    this->height = height;
}

void PictureInfo::setColor(unsigned char red,
                           unsigned char green,
                           unsigned char blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}
