#ifndef HLED_H
#define HLED_H

#include <QTimer>
#include <QWidget>

class QColor;

class HLed : public QWidget    {
    Q_OBJECT

private:
    struct Private;
    Private * const m_d;
    QTimer blinkTimer;
    quint64 counter;


private slots:
    void blinkOnce();

protected:
    void paintEvent(QPaintEvent *);
    int ledWidth() const;

public:
    HLed(QWidget *parent = 0);
    ~HLed();

    QColor color() const;
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void blink(qint64 count);
    void setColor(const QColor &color);
    void toggle();
    void turnOn();
    void turnOff();
};

#endif // HLED_H
