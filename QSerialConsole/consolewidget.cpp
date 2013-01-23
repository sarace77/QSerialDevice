#include "consolewidget.h"
#include "qserialdevice.h"

#include <QBuffer>

#include <QDebug>
ConsoleWidget::ConsoleWidget(QWidget *parent) : QTextEdit(parent) {
    setGeometry(5, 5, parent->width() - 10, parent->height() - 10);
    setStyleSheet("ConsoleWidget { background-color:#000000; color: #ffffff}");
    _lastLine = textCursor();

    _protocol = NULL;

    _tagComboBox = new QComboBox(this);
    _tagComboBox->hide();
    _tagComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    _fT = false;
    connect(_tagComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(addProtocolTag()));
}

ConsoleWidget::~ConsoleWidget() {

}

void ConsoleWidget::addProtocolTag() {
    _tagComboBox->hide();
    setTextColor(QColor(Qt::red));
    if (_fT) {
        insertPlainText(_tagComboBox->currentText());
    }
    _fT = true;
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

void ConsoleWidget::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton && _protocol) {
        setContextMenuPolicy(Qt::NoContextMenu);
        if (!_fT) {
            _tagComboBox->clear();
            _tagComboBox->addItems(_protocol->getTagList());
        }
        _tagComboBox->move(e->pos());
        _tagComboBox->show();
    } else {
        setContextMenuPolicy(Qt::DefaultContextMenu);
        QTextEdit::mousePressEvent(e);
    }
}

void ConsoleWidget::printBanner(QString banner) {
    setTextCursor(_lastLine);
    clear();
    setTextColor(QColor(Qt::white));
    append(banner);
    _lastLine = textCursor();
}

void ConsoleWidget::printIncomingMessage(QByteArray msg) {
    if (_protocol)
        msg = QSerialDevice::decodeData(_protocol, msg);
    setTextCursor(_lastLine);
    setTextColor(QColor(Qt::yellow));
    insertPlainText(QString(msg)+QString("\n"));
    _lastLine = textCursor();
}

void ConsoleWidget::protocolChanged(Protocol *proto) {
    qDebug() << " cazzo" << proto;
    _fT = false;
    _protocol = proto;
}
