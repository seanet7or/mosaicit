#include "createpage.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

CreatePage::CreatePage()
{
    this->nameLabel = new QLabel(tr("Name of database:"));
    this->nameEdit = new QLineEdit;
    this->directoryLabel =
            new QLabel(tr("Select directory containing the pictures"));
    this->directoryEdit = new QLineEdit;
    this->directoryButton = new QPushButton(tr("..."));
    this->includeSubdirsButton = new QCheckBox(tr("include subdirectories"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameEdit, 0, 1);
    layout->addWidget(this->directoryLabel, 1, 0);
    layout->addWidget(this->directoryEdit, 1, 1);
    layout->addWidget(this->directoryButton, 1, 2);
    layout->addWidget(this->includeSubdirsButton, 2, 0);
    setLayout(layout);

    registerField("nameEdit", this->nameEdit);
    registerField("directoryEdit", this->directoryEdit);
    registerField("includeSubdirs", this->includeSubdirsButton);

    connect(this->directoryButton,
            SIGNAL(clicked()),
            this,
            SLOT(selectDirBnClicked()));
}

void CreatePage::initializePage()
{
    this->directoryEdit->setText(QDir::homePath());
    this->includeSubdirsButton->setChecked(true);
    this->nameEdit->setText(tr("<New Database>"));
}

bool CreatePage::validatePage()
{
    QDir rootDir(this->directoryEdit->text());
    if ("" == this->nameEdit->text()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("You have to specify a name for the new database"),
                             QMessageBox::Ok);
        return false;
    } else if (!rootDir.exists()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("You have to specify a valid directory"),
                             QMessageBox::Ok);
        return false;
    }
    return true;
}

void CreatePage::selectDirBnClicked()
{
    this->directoryEdit->setText(QFileDialog::getExistingDirectory(
            this,
            tr("Select picture folder"),
            this->directoryEdit->text(),
            QFileDialog::ShowDirsOnly));
}
