/***************************************************************************************************
*
* FILE:     picturelibrary.h
*
* CREATED:  2014-12-26
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  Helper class for fast access to all valid picture entries in the database
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright by Benjamin Caspari
*
***************************************************************************************************/
#ifndef PICTURELIBRARY_H
#define PICTURELIBRARY_H
#include <QVector>
#include "database/pictureinfo.h"

class PictureLibrary
{
public:
    PictureLibrary();
    ~PictureLibrary();

    PictureInfo* pictureAt(int index) const;
    int size() const;

private:
    QVector<PictureInfo*> m_processedPictures;
};

#endif // PICTURELIBRARY_H
