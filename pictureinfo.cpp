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
