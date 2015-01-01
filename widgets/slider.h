#ifndef SLIDER_H
#define SLIDER_H
#include <QSlider>

class Slider : public QSlider
{
public:
    Slider(QWidget* parent);
    ~Slider();

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    qreal relativeValue() const;
};

#endif // SLIDER_H
