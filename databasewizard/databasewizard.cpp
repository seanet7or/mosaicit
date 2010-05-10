#include "databasewizard.h"

#include "createpage.h"
#include "processpage.h"
#include "resultpage.h"

DatabaseWizard::DatabaseWizard()
{
    setPage(Page_Create, new CreatePage);
    setPage(Page_Process, new ProcessPage);
    setPage(Page_Finish, new ResultPage);
    setStartId(Page_Create);
    setOption(HaveHelpButton, false);
    setWindowTitle(tr("Create new database"));
}
