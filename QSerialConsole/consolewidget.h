#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include "protocol.h"

#include <QComboBox>
#include <QKeyEvent>
#include <QTextCursor>
#include <QTextEdit>

class ConsoleWidget : public QTextEdit
{
    Q_OBJECT

private:
    QTextCursor _lastLine;
    QComboBox *_tagComboBox;

    bool _fT;

    Protocol *_protocol;

private slots:
    void addProtocolTag();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *e);

public:
    explicit ConsoleWidget(QWidget *parent = 0);   
    ~ConsoleWidget();

public slots:
    void protocolChanged(Protocol *proto);
    void printBanner(QString banner);
    void printIncomingMessage(QByteArray msg);

signals:
    void newCmdSent(QByteArray cmd);
};

#endif // CONSOLEWIDGET_H
