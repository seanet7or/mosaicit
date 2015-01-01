#include "Animator.h"
#include <QWidget>
#include "widgets.h"

Animator::Animator() : QObject(), m_frameTimer(new QTimer(this))
{
    connect(m_frameTimer, SIGNAL(timeout()), this, SLOT(onFrameTimerTimeout()));
}

Animator::~Animator()
{
    m_frameTimer->blockSignals(true);
    m_frameTimer->disconnect();
    delete m_frameTimer;
}

Animator *Animator::instance()
{
    static Animator inst;
    return &inst;
}

void Animator::onObjectDestroyed(QObject *o)
{
    QWidget *w = qobject_cast<QWidget*>(o);
    if (w)
    {
        m_dirtyWidgets.remove(w);
    }
}

void Animator::onFrameTimerTimeout()
{
    if (m_dirtyWidgets.isEmpty())
    {
        m_frameTimer->stop();
    }
    else
    {
        foreach (QWidget *w, m_dirtyWidgets)
        {
            w->update();
        }
        m_dirtyWidgets.clear();
    }
}

void Animator::updateNextFrame(QWidget *w)
{
    Animator *inst = Animator::instance();
    if (!inst->m_frameTimer->isActive())
    {
        inst->m_frameTimer->start(GUI_TIME_FRAMETIME);
    }
    inst->m_dirtyWidgets.insert(w);
}
