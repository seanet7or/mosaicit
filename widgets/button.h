#ifndef BUTTON_H
#define BUTTON_H
#include <QTimeLine>
#include <QWidget>
#include <QObject>

class Button : public QWidget
{
    Q_OBJECT
public:
    explicit Button(QWidget *parent = 0);
    ~Button();

    void setText(const QString &text);
    void setHorizontalAlignment(Qt::AlignmentFlag alignment);

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;
    virtual QSizePolicy sizePolicy() const;

signals:
    void pressed();

public slots:

protected:
#ifndef Q_OS_ANDROID
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
#endif
    virtual void paintEvent(QPaintEvent*);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void showEvent(QShowEvent*);

private slots:
    void onAnimTimelineChanged(qreal);

private:
    int m_textWidth;
    QString m_text;
    QFont m_font;
    QTimeLine m_touchFeedbackTimeline;
    QTimeLine m_fadeInTimeline;
#ifndef Q_OS_ANDROID
    QTimeLine m_hoverTimeLine;
#endif
    Qt::AlignmentFlag m_horizontalAlignment;
};

#endif // BUTTON_H
