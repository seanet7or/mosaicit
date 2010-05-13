#include "processpage.h"

#include <QGridLayout>
#include <QVariant>

#include "../picturedatabase.h"

ProcessPage::ProcessPage()
{
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
    PictureDatabase *newDatabase = new PictureDatabase;
    newDatabase->addDirectory(field("directoryEdit").toString(),
                              field("includeSubdirs").toBool());
    newDatabase->processFiles();
}
