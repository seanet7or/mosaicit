#ifndef PROCESSPAGE_H
#define PROCESSPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QProgressBar>

class ProcessPage : public QWizardPage
{
public:
    ProcessPage();
    void initializePage();
private:
    QLabel *infoLabel;
    QProgressBar *processProgress;
};

#endif // PROCESSPAGE_H
