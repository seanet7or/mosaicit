/***************************************************************************************************
*
* FILE:     builddatabasedlg.h
*
* CREATED:  01-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  Dialog that shows the progress when building a databas
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

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
    void readSettings();
    void writeSettings();

    Ui::BuildDatabaseDlg *ui;
    QString m_name;
    QString m_directory;
    bool m_includeSubdirectories;
    PictureDatabase *m_newDatabase;
    bool m_canceled;
};

#endif // BUILDDATABASEDLG_H
