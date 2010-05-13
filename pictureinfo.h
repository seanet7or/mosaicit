#ifndef PICTUREINFO_H
#define PICTUREINFO_H

#include <QString>
#include <QDateTime>
#include <QDataStream>

class PictureInfo
{
public:
    PictureInfo();
    void setFile(const QString &filename);
    QString getFile();
    void setProcessed(bool processed);
    bool processed();
    bool validFile();
    void setValidFile(bool valid);
    int getWidth();
    int getHeight();
    void setDimensions(int width, int height);
    void setColor(unsigned char red, unsigned char green, unsigned char blue);
    QDateTime lastChanged();
    void setLastChanged(const QDateTime &lastChanged);
    void toStream(QDataStream &out);
private:
    QString m_filename;
    bool m_processed;
    bool m_validFile;
    QDateTime m_lastChanged;
    int m_width;
    int m_height;
    unsigned char m_red, m_green, m_blue;
};

#endif // PICTUREINFO_H
