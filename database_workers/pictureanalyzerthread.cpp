#include "pictureanalyzerthread.h"
#include <QFileInfo>
#include <QDebug>
#include <QImage>
#include "../settings.h"

PictureAnalyzerThread::PictureAnalyzerThread()
{
    this->m_database = new PictureDatabase();
}

PictureAnalyzerThread::~PictureAnalyzerThread()
{

}

void PictureAnalyzerThread::run()
{
    //qDebug() << "Analyzing" << m_file;
    QFileInfo fileInfo(m_file);

    if (!fileInfo.exists()) {
        m_database->deletePicture(m_file);
    } else {
        QImage image(m_file);

        if (image.isNull()) {
            m_database->setPictureProperties(m_file,
                                             fileInfo.lastModified(),
                                             PICTURE_STATE_CORRUPT,
                                             0,
                                             0,
                                             0);
        } else {
            if (fileInfo.size() < Settings::minimumFileSize()) {
                m_database->setPictureProperties(m_file,
                                             fileInfo.lastModified(),
                                             PICTURE_STATE_FILETOOSMALL,
                                             0,
                                             0,
                                             0);
            } else {
                if (((float)image.width() / (float)image.height() > Settings::maxAspectRatio())
                        || ((float)image.height() / (float)image.width() > Settings::maxAspectRatio())) {
                    m_database->setPictureProperties(m_file,
                                                 fileInfo.lastModified(),
                                                 PICTURE_STATE_WRONGASPECTRATIO,
                                                 0,
                                                 0,
                                                 0);
                } else if ((image.height() < Settings::minimumPictureHeight()) || (image.width() < Settings::minimumPictureWidth())) {
                    m_database->setPictureProperties(m_file,
                                                 fileInfo.lastModified(),
                                                 PICTURE_STATE_TOOSMALL,
                                                 0,
                                                 0,
                                                 0);
                } else {
                    unsigned long long red = 0, green = 0, blue = 0;
                    for (int i = 0; i < image.width(); i++) {
                        for (int j = 0; j < image.height(); j++) {
                            QRgb color = image.pixel(QPoint(i, j));
                            red += qRed(color);
                            green += qGreen(color);
                            blue += qBlue(color);
                        }
                    }
                    unsigned int pixels = image.width() * image.height();
                    red /= pixels;
                    green /= pixels;
                    blue /= pixels;
                    quint8 blueByte = (quint8)blue;
                    quint8 greenByte = (quint8)green;
                    quint8 redByte = (quint8)red;
                    quint32 rgb = blueByte + (greenByte << 8) + (redByte << 16);
                    m_database->setPictureProperties(m_file,
                                                     fileInfo.lastModified(),
                                                     PICTURE_STATE_PROCESSED,
                                                     image.width(),
                                                     image.height(),
                                                     rgb);
                    //qDebug() << "Analyzed" << m_file;
                }
            }
        }
        exit(0);
    }   
}

void PictureAnalyzerThread::startAnalyzer(const QString &file)
{
    m_file = file;
    if (isRunning())
    {
        qWarning() << "PictureAnalyzerThread is running, but received a new request.";
    }
    else
    {
        start();
    }
}
