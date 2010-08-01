#include "pictureinfo.h"

PictureInfo::PictureInfo()
{
    m_processed = false;
    m_validFile = false;
    m_filename = "";
}

void PictureInfo::setFile(const QString &filename)
{
    this->m_filename = filename;
    m_processed = false;
    m_validFile = false;
}

bool PictureInfo::validFile()
{
    return this->m_validFile;
}

void PictureInfo::setValidFile(bool valid)
{
    this->m_validFile = valid;
}

QDateTime PictureInfo::lastChanged()
{
    return this->m_lastChanged;
}

void PictureInfo::setLastChanged(const QDateTime &lastChanged)
{
    this->m_lastChanged = lastChanged;
}

void PictureInfo::toStream(QDataStream &out)
{
    out << this->m_blue;
    out << this->m_filename;
    out << this->m_green;
    out << this->m_height;
    out << this->m_lastChanged;
    out << this->m_processed;
    out << this->m_red;
    out << this->m_validFile;
    out << this->m_width;
}

void PictureInfo::fromStream(QDataStream &in)
{
    in >> this->m_blue;
    in >> this->m_filename;
    in >> this->m_green;
    in >> this->m_height;
    in >> this->m_lastChanged;
    in >> this->m_processed;
    in >> this->m_red;
    in >> this->m_validFile;
    in >> this->m_width;
}

QString PictureInfo::getFile()
{
    return this->m_filename;
}

void PictureInfo::setProcessed(bool processed)
{
    this->m_processed = processed;
}

bool PictureInfo::processed()
{
    return this->m_processed;
}

int PictureInfo::getHeight()
{
    return this->m_height;
}

int PictureInfo::getWidth()
{
    return this->m_width;
}

void PictureInfo::setDimensions(int width, int height)
{
    this->m_width = width;
    this->m_height = height;
}

void PictureInfo::setColor(unsigned char red,
                           unsigned char green,
                           unsigned char blue)
{
    this->m_red = red;
    this->m_green = green;
    this->m_blue = blue;
}

unsigned char PictureInfo::getBlue()
{
    return this->m_blue;
}

unsigned char PictureInfo::getRed()
{
    return this->m_red;
}

unsigned char PictureInfo::getGreen()
{
    return this->m_green;
}
