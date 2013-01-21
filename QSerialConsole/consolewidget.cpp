#include "consolewidget.h"

#include <QBuffer>

#include <QDebug>
ConsoleWidget::ConsoleWidget(QWidget *parent) : QTextEdit(parent) {
    setGeometry(5, 5, parent->width() - 10, parent->height() - 10);
    setStyleSheet("ConsoleWidget { background-color:#000000; color: #ffffff}");
    _lastLine = textCursor();
}

ConsoleWidget::~ConsoleWidget() {

}

void ConsoleWidget::keyPressEvent(QKeyEvent *event) {
    setTextCursor(_lastLine);
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
    if (event->key() != Qt::Key_Up)
        QTextEdit::keyPressEvent(event);
    _lastLine = textCursor();
}

void ConsoleWidget::printBanner(QString banner) {
    setTextCursor(_lastLine);
    clear();
    setTextColor(QColor(Qt::white));
    append(banner);
    _lastLine = textCursor();
}

void ConsoleWidget::printIncomingMessage(QByteArray msg) {
    setTextCursor(_lastLine);
    setTextColor(QColor(Qt::yellow));
    insertPlainText(QString(msg)+QString("\n"));
    _lastLine = textCursor();
}
