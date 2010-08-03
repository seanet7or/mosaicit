#ifndef CREATEMOSAICDLG_H
#define CREATEMOSAICDLG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class CreateMosaicDlg;
}

class CreateMosaicDlg : public QDialog {
    Q_OBJECT
public:
    CreateMosaicDlg(QWidget *parent = 0);
    ~CreateMosaicDlg();
    bool exitedCorrectly();
    QString image();
    QString database();
    QString outputImage();

protected:
    void changeEvent(QEvent *e);

private slots:
    void selectDBBnPressed();
    void selectImageBnPressed();
    void selectOutputBnPressed();
    void imageChanged();
    void nextBnPressed();
    void cancelBnPressed();

private:
    void readSettings();
    void writeSettings();

    Ui::CreateMosaicDlg *ui;
    bool m_canceled;
    QString m_database;
    QString m_image;
    QString m_outputImage;

};

#endif // CREATEMOSAICDLG_H
