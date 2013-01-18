#include "consolewidget.h"

#include <QBuffer>

#include <QDebug>
ConsoleWidget::ConsoleWidget(QWidget *parent) : QTextEdit(parent) {
    setGeometry(5, 5, parent->width() - 10, parent->height() - 10);
    setStyleSheet("ConsoleWidget { background-color:#000000; color: #ffffff}");
}

void ConsoleWidget::keyPressEvent(QKeyEvent *event) {
    setTextColor(QColor(Qt::green));
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        QByteArray data = toPlainText().toAscii(), line;
        QBuffer buffer(&data);
        buffer.open(QIODevice::ReadWrite);
        while(!buffer.atEnd()) {
            line = buffer.readLine();
        }
        emit newCmdSent(line);
    }
    QTextEdit::keyPressEvent(event);
}

void ConsoleWidget::printBanner(QString banner) {
    clear();
    setTextColor(QColor(Qt::white));
    append(banner);
}

void ConsoleWidget::printIncomingMessage(QByteArray msg) {
    setTextColor(QColor(Qt::yellow));
    append(QString(msg)+QString("\n"));
}
