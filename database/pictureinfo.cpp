/***************************************************************************************************
*
* FILE:     pictureinfo.cpp
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
#include "pictureinfo.h"
#include <QDir>

PictureInfo::PictureInfo(const QString &path, int width, int height, quint32 rgb)
{
    m_path = path;
    m_width = width;
    m_height = height;
    m_rgb = rgb;
}

int PictureInfo::getBlue() const
{
    return m_rgb & 0xFF;
}

int PictureInfo::getRed() const
{
    return (m_rgb & 0xFF0000) >> 16;
}

int PictureInfo::getGreen() const
{
    return (m_rgb & 0xFF00) >> 8;
}

const QString &PictureInfo::getPath() const
{
    return m_path;
}

QString PictureInfo::getDirectory() const
{
    int pathSepIndex = m_path.lastIndexOf('\\');
    pathSepIndex = std::max(pathSepIndex, m_path.lastIndexOf('/'));
    if (pathSepIndex > -1) {
        return m_path.left(pathSepIndex);
    } else {
        return "";
    }
}

bool PictureInfo::enabled() const
{
    return true;
}

QString PictureInfo::filename() const
{
    int pathSepIndex = m_path.lastIndexOf('\\');
    pathSepIndex = std::max(pathSepIndex, m_path.lastIndexOf('/'));
    if (pathSepIndex > -1) {
        return m_path.mid(pathSepIndex);
    } else {
        return "";
    }
}
