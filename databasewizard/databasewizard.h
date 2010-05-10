#ifndef DATABASEWIZARD_H
#define DATABASEWIZARD_H

#include <QWizard>

class DatabaseWizard : public QWizard
{
public:
    DatabaseWizard();
private:
    enum {Page_Create, Page_Process, Page_Finish};
};

#endif // DATABASEWIZARD_H
