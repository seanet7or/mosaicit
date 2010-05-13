#ifndef BUILDDATABASEDLG_H
#define BUILDDATABASEDLG_H

#include <QDialog>

namespace Ui {
    class BuildDatabaseDlg;
}

class BuildDatabaseDlg : public QDialog {
    Q_OBJECT
public:
    BuildDatabaseDlg(QWidget *parent = 0);
    ~BuildDatabaseDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BuildDatabaseDlg *ui;
};

#endif // BUILDDATABASEDLG_H
