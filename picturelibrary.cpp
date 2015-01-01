/***************************************************************************************************
*
* FILE:     picturelibrary.cpp
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
#include "picturelibrary.h"
#include "database/picturedatabase.h"

PictureLibrary::PictureLibrary()
{
    PictureDatabase database;
    m_processedPictures = database.picturesProcessed();
}

PictureLibrary::~PictureLibrary()
{
   foreach (PictureInfo* p, m_processedPictures)
   {
       delete p;
   }
   m_processedPictures.clear();
}

int PictureLibrary::size() const
{
    return m_processedPictures.size();
}

PictureInfo* PictureLibrary::pictureAt(int index) const
{
    return m_processedPictures.at(index);
}

 QMap<QString, QVector<PictureInfo*> > PictureLibrary::picturesByDirectory() const
 {
     QMap<QString, QVector<PictureInfo*> > result;
     foreach (PictureInfo *p, m_processedPictures) {
        QString directory = p->getDirectory();
        if (!result.contains(directory)) {
            result.insert(directory, QVector<PictureInfo*>());
        }
        result[directory].append(p);
     }
     return result;
 }
