#ifndef BUILDDATABASEDLG_H
#define BUILDDATABASEDLG_H

#include <QDialog>
#include <QString>

#include "picturedatabase.h"

namespace Ui {
    class BuildDatabaseDlg;
}

class BuildDatabaseDlg : public QDialog {
    Q_OBJECT
public:
    BuildDatabaseDlg(QWidget *parent,
                     const QString &name,
                     const QString &directory,
                     bool includeSubdirectories);
    ~BuildDatabaseDlg();

public slots:
    void processProgress(float percent);
    void closeEvent(QCloseEvent *e);
    void reject();

protected:
    void changeEvent(QEvent *e);

private slots:
    void indexingFinished();
    void processingFinished(bool wasCanceled);
    void closeButtonPressed();

private:
    Ui::BuildDatabaseDlg *ui;
    QString m_name;
    QString m_directory;
    bool m_includeSubdirectories;
    PictureDatabase *m_newDatabase;
    bool m_canceled;
};

#endif // BUILDDATABASEDLG_H
