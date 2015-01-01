#ifndef PICTUREDATABASEDLG_H
#define PICTUREDATABASEDLG_H
#include <QDialog>
#include <QMap>
#include "database/pictureinfo.h"
#include "picturelibrary.h"

namespace Ui {
class PictureDatabaseDlg;
}

class PictureDatabaseDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PictureDatabaseDlg(QWidget *parent = 0);
    ~PictureDatabaseDlg();

private slots:
    void onDirectorySelected();
    void onFileSelected();

private:
    Ui::PictureDatabaseDlg *ui;
    QMap<QString, QVector<PictureInfo*> > m_picturesByDir;
    PictureLibrary m_library;
};

#endif // PICTUREDATABASEDLG_H
