#ifndef PROCESSPAGE_H
#define PROCESSPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QProgressBar>

#include "../picturedatabase.h"

class ProcessPage : public QWizardPage
{
    Q_OBJECT
public:
    ProcessPage();
    void initializePage();
public slots:
    void closeEvent(QCloseEvent *e);
private:
    QLabel *infoLabel;
    QProgressBar *processProgress;
    PictureDatabase *newDatabase;
};

#endif // PROCESSPAGE_H
