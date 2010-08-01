#ifndef RENDERMOSAICDLG_H
#define RENDERMOSAICDLG_H

#include <QDialog>
#include <QString>

#include "picturedatabase.h"
#include "rendermosaicthread.h"

namespace Ui {
    class RenderMosaicDlg;
}

class RenderMosaicDlg : public QDialog {
    Q_OBJECT
public:
    RenderMosaicDlg(QWidget *parent,
                    PictureDatabase *database,
                    const QString &imageFile,
                    int tileWidth,
                    int tileHeight,
                    int tileCount,
                    bool cutEdges,
                    int alphaChannel);
    ~RenderMosaicDlg();

public slots:
    void logText(const QString &text);

private slots:
    void renderComplete(float percent);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RenderMosaicDlg *ui;
    RenderMosaicThread *m_renderThread;
};

#endif // RENDERMOSAICDLG_H
