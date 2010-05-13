#include "processpage.h"

#include <QGridLayout>
#include <QVariant>
#include <QMessageBox>
#include <QCloseEvent>

#include "debug.h"

#define log(text) Debug::log(text)

ProcessPage::ProcessPage()
{
    newDatabase = 0;
    this->infoLabel =
            new QLabel(tr("Please stand by while the files are processed. This may take a long time."));
    this->processProgress = new QProgressBar;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(infoLabel, 0, 0);
    layout->addWidget(processProgress, 1, 0);
    this->setLayout(layout);
}

void ProcessPage::initializePage()
{
    newDatabase = new PictureDatabase;
/*    newDatabase->addDirectory(field("directoryEdit").toString(),
                              field("includeSubdirs").toBool());*/
    newDatabase->processFiles();
}

void ProcessPage::closeEvent(QCloseEvent *e)
{
    Q_ASSERT (newDatabase != 0);
    log("ProcessPage::closeEvent called");
    if (newDatabase->isProcessingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel?"),
                                  tr("Image processing is in progress. Do you really wont to cancel?"),
                                  QMessageBox::No | QMessageBox::Yes,
                                  QMessageBox::No) != QMessageBox::Yes) {
            e->ignore();
            return;
        }
    }
    e->accept();
}
