#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QKeyEvent>
#include <QTextCursor>
#include <QTextEdit>


class ConsoleWidget : public QTextEdit
{
    Q_OBJECT

private:
    QTextCursor _lastLine;

protected:
    void keyPressEvent(QKeyEvent *event);

public:
    explicit ConsoleWidget(QWidget *parent = 0);   
    ~ConsoleWidget();

public slots:
    void printBanner(QString banner);
    void printIncomingMessage(QByteArray msg);
signals:
    void newCmdSent(QByteArray cmd);
};

#endif // CONSOLEWIDGET_H
