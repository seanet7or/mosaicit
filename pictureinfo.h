#ifndef PICTUREINFO_H
#define PICTUREINFO_H

#include <QString>

class PictureInfo
{
public:
    PictureInfo();
    void setFile(const QString &filename);

private:
    QString filename;
    bool valid;
};

#endif // PICTUREINFO_H
