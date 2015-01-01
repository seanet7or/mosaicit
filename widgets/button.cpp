#include "button.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "Animator.h"
#include "widgets.h"

#define GUI_TIME_ANIMATION_DURATION 250
#define GUI_TIME_FADEIN_DURATION    (GUI_TIME_ANIMATION_DURATION * 2)

#define BUTTON_INTERNALPADDING_HOR  8
#define BUTTON_EXTERNALPADDING_HOR  4
#define BUTTON_MINIMUMWIDTH         88

#define BUTTON_HEIGHT               48
#define BUTTON_EXTERNALPADDING_VER  6
#define BUTTON_INTERNALHEIGHT       (BUTTON_HEIGHT - (2 * BUTTON_EXTERNALPADDING_VER))
#define BUTTON_INTERNALPADDING_VER  ((BUTTON_INTERNALHEIGHT - BUTTON_FONTSIZE) / 2)

#define BUTTON_FONTSIZE             14

#define BUTTON_BACKGROUNDCOLOR      QColor(0x21, 0x96, 0xF3, 0xFF)
#define BUTTON_HOVERCOLOR           QColor(0x1E, 0x88, 0xE5)
#define BUTTON_PRESSEDCOLOR         QColor(0x19, 0x76, 0xD2)

#define BUTTON_FONTCOLOR            QColor::fromRgbF(1.f, 1.f, 1.f, 0.87f)

QColor blendColors(QColor sourceColor, QColor newColor)
{
    qreal alpha = newColor.alphaF();
    QColor out((newColor.red() * alpha)
               + ((1 - alpha) * sourceColor.red()),
               (newColor.green() * alpha)
               + ((1 - alpha) * sourceColor.green()),
               (newColor.blue() * alpha)
               + ((1 - alpha) * sourceColor.blue()),
               std::min(255, newColor.alpha() + sourceColor.alpha()));
    return out;
}

Button::Button(QWidget *parent) : QWidget(parent)
{
    m_horizontalAlignment = Qt::AlignLeft;

    m_font.setCapitalization(QFont::AllUppercase);
    m_font.setPixelSize(BUTTON_FONTSIZE);

    m_touchFeedbackTimeline.setUpdateInterval(GUI_TIME_FRAMETIME);
    m_touchFeedbackTimeline.setCurveShape(QTimeLine::EaseOutCurve);
    m_touchFeedbackTimeline.setDuration(GUI_TIME_ANIMATION_DURATION);
    connect(&m_touchFeedbackTimeline,
            &QTimeLine::valueChanged,
            this,
            &Button::onAnimTimelineChanged);

    m_fadeInTimeline.setUpdateInterval(GUI_TIME_FRAMETIME);
    m_fadeInTimeline.setDuration(GUI_TIME_FADEIN_DURATION);
    connect(&m_fadeInTimeline,
            &QTimeLine::valueChanged,
            this,
            &Button::onAnimTimelineChanged);

#ifndef Q_OS_ANDROID
    m_hoverTimeLine.setUpdateInterval(GUI_TIME_FRAMETIME);
    m_hoverTimeLine.setDuration(GUI_TIME_ANIMATION_DURATION);
    connect(&m_hoverTimeLine,
            &QTimeLine::valueChanged,
            this,
            &Button::onAnimTimelineChanged);
#endif
}

void Button::onAnimTimelineChanged(qreal /*value*/)
{
    Animator::updateNextFrame(this);
}

Button::~Button()
{

}

void Button::setText(const QString &text)
{
    m_text = text;
    QFontMetrics fontMetrics(m_font);
    m_textWidth = fontMetrics.width(m_text);
}

void Button::paintEvent(QPaintEvent *)
{
    QColor bgColor(BUTTON_BACKGROUNDCOLOR);

#ifndef Q_OS_ANDROID
    qreal hoverAlpha = m_hoverTimeLine.currentValue();
    if (!qFuzzyIsNull(hoverAlpha))
    {
        QColor hoverFeedbackColor(BUTTON_HOVERCOLOR);
        hoverFeedbackColor.setAlphaF(hoverAlpha * hoverFeedbackColor.alphaF());
        bgColor = blendColors(bgColor, hoverFeedbackColor);
    }
#endif

    qreal pressedAlpha = m_touchFeedbackTimeline.currentValue();
    if (!qFuzzyIsNull(pressedAlpha))
    {
        QColor touchFeedbackColor(BUTTON_PRESSEDCOLOR);
        touchFeedbackColor.setAlphaF(pressedAlpha * touchFeedbackColor.alphaF());
        bgColor = blendColors(bgColor, touchFeedbackColor);
    }

    QRect innerRect(BUTTON_EXTERNALPADDING_HOR,
                    BUTTON_EXTERNALPADDING_VER,
                    width() - (2 * BUTTON_EXTERNALPADDING_HOR),
                    BUTTON_HEIGHT - (2 * BUTTON_EXTERNALPADDING_VER));
    QRect textRect(BUTTON_EXTERNALPADDING_HOR + BUTTON_INTERNALPADDING_HOR,
                   BUTTON_EXTERNALPADDING_VER,
                   width() - (2 * BUTTON_EXTERNALPADDING_HOR) - (2 * BUTTON_INTERNALPADDING_HOR),
                   BUTTON_HEIGHT - (2 * BUTTON_EXTERNALPADDING_VER));

    QPainter painter(this);
    painter.setOpacity(m_fadeInTimeline.currentValue());
    painter.fillRect(innerRect,
                     bgColor);
    painter.setPen(BUTTON_FONTCOLOR);
    painter.setFont(m_font);
    painter.drawText(textRect,
                     Qt::AlignVCenter | m_horizontalAlignment,
                     m_text);
}

void Button::showEvent(QShowEvent *)
{
    m_fadeInTimeline.setDirection(QTimeLine::Forward);
    if (m_fadeInTimeline.state() != QTimeLine::Running)
    {
        m_fadeInTimeline.start();
    }
}

void Button::setHorizontalAlignment(Qt::AlignmentFlag alignment)
{
    m_horizontalAlignment = alignment;
}

QSize Button::sizeHint() const
{
    int width = BUTTON_EXTERNALPADDING_HOR + BUTTON_INTERNALPADDING_HOR
            + m_textWidth
            + BUTTON_INTERNALPADDING_HOR + BUTTON_EXTERNALPADDING_HOR;
    int minWidth = std::max(BUTTON_MINIMUMWIDTH, width);
    return QSize(minWidth,
                 BUTTON_HEIGHT);
}

QSizePolicy Button::sizePolicy() const
{
    return QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

QSize Button::minimumSizeHint() const
{
    return sizeHint();
}

#ifndef Q_OS_ANDROID
void Button::enterEvent(QEvent *)
{
    m_hoverTimeLine.setDirection(QTimeLine::Forward);
    if (m_hoverTimeLine.state() != QTimeLine::Running)
    {
        m_hoverTimeLine.start();
    }
}

void Button::leaveEvent(QEvent *)
{
    //qDebug() << "Button" << m_text << "leaveEvent";
    m_hoverTimeLine.setDirection(QTimeLine::Backward);
    if (m_hoverTimeLine.state() != QTimeLine::Running)
    {
        m_hoverTimeLine.start();
    }
}
#endif

void Button::mouseMoveEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()) && (e->buttons() != Qt::NoButton))
    {
        if (m_touchFeedbackTimeline.direction() != QTimeLine::Forward)
        {
            m_touchFeedbackTimeline.setDirection(QTimeLine::Forward);
            if (m_touchFeedbackTimeline.state() != QTimeLine::Running)
            {
                m_touchFeedbackTimeline.start();
                Animator::updateNextFrame(this);
            }
        }
    }
    else
    {
        if (m_touchFeedbackTimeline.direction() != QTimeLine::Backward)
        {
            m_touchFeedbackTimeline.setDirection(QTimeLine::Backward);
            if (m_touchFeedbackTimeline.state() != QTimeLine::Running)
            {
                m_touchFeedbackTimeline.start();
                Animator::updateNextFrame(this);
            }
        }
    }
}

void Button::mousePressEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()))
    {
        m_touchFeedbackTimeline.setDirection(QTimeLine::Forward);
        if (m_touchFeedbackTimeline.state() != QTimeLine::Running)
        {
            m_touchFeedbackTimeline.start();
            Animator::updateNextFrame(this);
        }
    }
}

void Button::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_touchFeedbackTimeline.direction() != QTimeLine::Backward)
    {
        m_touchFeedbackTimeline.setDirection(QTimeLine::Backward);
        if (m_touchFeedbackTimeline.state() != QTimeLine::Running)
        {
            m_touchFeedbackTimeline.start();
        }
    }

    if ((e != NULL) && rect().contains(e->pos()))
    {
        //qDebug() << "Button was clicked.";
        emit pressed();
    }
}
