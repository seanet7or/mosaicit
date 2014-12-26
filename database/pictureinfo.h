/***************************************************************************************************
*
* FILE:     pictureinfo.h
*
* CREATED:  2010-10-05
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  stores information about a picture database entry
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/
#ifndef PICTUREINFO_H
#define PICTUREINFO_H
#include <QString>
#include <QDateTime>
#include <QDataStream>

class PictureInfo
{
public:
    PictureInfo(const QString &path, int width, int height, quint32 rgb);
    int getRed() const;
    int getGreen() const;
    int getBlue() const;
    const QString &getFile() const;

private:
    QString m_path;
    int m_width;
    int m_height;
    quint32 m_rgb;
};

#endif // PICTUREINFO_H
