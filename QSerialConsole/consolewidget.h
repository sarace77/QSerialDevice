#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QKeyEvent>
#include <QTextEdit>

class ConsoleWidget : public QTextEdit
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event);

public:
    explicit ConsoleWidget(QWidget *parent = 0);   
    
public slots:
    void printBanner(QString banner);
    void printIncomingMessage(QByteArray msg);
signals:
    void newCmdSent(QByteArray cmd);
};

#endif // CONSOLEWIDGET_H
