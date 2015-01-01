#include "slider.h"
#include <QPainter>
#include <QDebug>

#define SLIDER_LINEWIDTH        2

#define SLIDER_LINECOLOR        QColor::fromRgbF(0.f, 0.f, 0.f, 0.3f)
#define SLIDER_VALUELINECOLOR   QColor(0x21, 0x96, 0xF3, 0xFF)
#define SLIDER_BUTTONRADIUS     6

Slider::Slider(QWidget *parent) : QSlider(parent)
{

}

Slider::~Slider()
{

}

qreal Slider::relativeValue() const
{
    int valueRelToMin = value() - minimum();
    if (valueRelToMin == 0) {
        return 0;
    } else {
        qreal range = (qreal)(maximum() - minimum());
        qreal res = ((qreal)valueRelToMin) / range;
        return res;
    }
}

void Slider::paintEvent(QPaintEvent *)
{


    QRect lineRect(SLIDER_BUTTONRADIUS,
                   (height() - SLIDER_LINEWIDTH) / 2,
                   width() - (2 * SLIDER_BUTTONRADIUS),
                   SLIDER_LINEWIDTH);


    QRect valueLineRect(SLIDER_BUTTONRADIUS,
                        (height() - SLIDER_LINEWIDTH) / 2,
                        ((qreal)(width() - (2 * SLIDER_BUTTONRADIUS)) * relativeValue()),
                        SLIDER_LINEWIDTH);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(lineRect, SLIDER_LINECOLOR);
    painter.fillRect(valueLineRect, SLIDER_VALUELINECOLOR);

    painter.setPen(SLIDER_VALUELINECOLOR);
    painter.setBrush(SLIDER_VALUELINECOLOR);
    painter.drawPie(((qreal)(width() - (2 * SLIDER_BUTTONRADIUS)) * relativeValue()),
                    (height() / 2) - SLIDER_BUTTONRADIUS,
                    SLIDER_BUTTONRADIUS * 2,
                    SLIDER_BUTTONRADIUS * 2,
                    0,
                    5760);
}
