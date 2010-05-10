#ifndef CREATEPAGE_H
#define CREATEPAGE_H

#include <QWizardPage>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

class CreatePage : public QWizardPage
{
    Q_OBJECT
public:
    CreatePage();
    bool validatePage();
    void initializePage();
private:
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QLabel *directoryLabel;
    QLineEdit *directoryEdit;
    QPushButton *directoryButton;
    QCheckBox *includeSubdirsButton;
private slots:
    void selectDirBnClicked();
};

#endif // CREATEPAGE_H
