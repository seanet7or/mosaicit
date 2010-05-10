#ifndef PICTUREDATABASE_H
#define PICTUREDATABASE_H

#include <QVector>

#include "pictureinfo.h"

class PictureDatabase
{
public:
    PictureDatabase();
    void addDirectory(QString directory, bool subdirs);
    void addFile(QString file);
private:
    QVector<PictureInfo*> pictureInfo;
};

#endif // PICTUREDATABASE_H
