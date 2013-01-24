#include "qconsolewidget.h"

#include <QTextBlock>

#ifdef _DEBUG_QCONSOLEWIDGET
#include <QDebug>
#define _MODULE_NAME "[QCONSOLEWIDGET] - "
#endif //_DEBUG_QCONSOLEWIDGET

QConsoleWidget::QConsoleWidget(QWidget *parent) : QTextEdit(parent) {
    _newTagsList = false;
    _promptTextCursor = textCursor();
    _tagsInsertComboBox = new QComboBox(this);
    _tagsList.clear();

    _tagsInsertComboBox->hide();
    _tagsInsertComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    setGeometry(5, 5, parent->width() - 10, parent->height() - 10);
    setStyleSheet("QConsoleWidget { background-color:#0a0a0a; color: #ffffff}");

    connect(_tagsInsertComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(insertProtocolTag(QString)));
    //connect(_tagsInsertComboBox, SIGNAL(highlighted(QString)), this, SLOT(insertProtocolTag(QString)));
}

QConsoleWidget::~QConsoleWidget() {
    delete _tagsInsertComboBox;
}

void QConsoleWidget::keyPressEvent(QKeyEvent *event) {
    setTextCursor(_promptTextCursor);
    setTextColor(QColor(Qt::green));
    switch(event->key()) {
    case Qt::Key_Up:
#ifdef _DEBUG_QCONSOLEWIDGET
        qDebug() << _MODULE_NAME << "keyPressEvent() - Up Arrow Key pressed, handler disabled!";
#endif //_DEBUG_QCONSOLEWIDGET
        break;
    case Qt::Key_Tab:
        if (_tagsList.isEmpty()) {
#ifdef _DEBUG_QCONSOLEWIDGET
            qDebug() << _MODULE_NAME << "keyPressEvent() - Tab Key pressed, handler disabled!";
#endif //_DEBUG_QCONSOLEWIDGET
            break;
        }
        _tagsInsertComboBox->move(QPoint(width() -_tagsInsertComboBox->width() - 5, 5));
        _tagsInsertComboBox->show();
        break;
    case Qt::Key_Backspace:
        if (!textCursor().atBlockStart())
            QTextEdit::keyPressEvent(event);
#ifdef _DEBUG_QCONSOLEWIDGET
        else
            qDebug() << _MODULE_NAME << "keyPressEvent() - Cursor reaches line start, Backspace handler disabled!";
#endif //_DEBUG_QCONSOLEWIDGET
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
#ifdef _DEBUG_QCONSOLEWIDGET
        qDebug() << _MODULE_NAME << "keyPressEvent() - Enter/Return Key pressed, sending line to event handler!";
#endif //_DEBUG_QCONSOLEWIDGET
//        setTextColor(QColor(Qt::white));
//        insertPlainText("$ ");
        emit newOutgoingMessage(textCursor().block().text().toAscii());
//        break;
    default:
        QTextEdit::keyPressEvent(event);
        break;
    }
    _promptTextCursor = textCursor();

}

void QConsoleWidget::insertProtocolTag(QString tag) {
    _tagsInsertComboBox->hide();
    setFocus();
    setTextColor(QColor(Qt::red));
    if(!_newTagsList) {
        insertPlainText(tag);
    }
    _newTagsList = false;
}

void QConsoleWidget::refreshProtocolTags(QStringList tags) {
#ifdef _DEBUG_QCONSOLEWIDGET
        qDebug() << _MODULE_NAME << "refreshProtocolTags() - Adding new protocol tags";
#endif //_DEBUG_QCONSOLEWIDGET
    _newTagsList = true;
    _tagsList = tags;
    _tagsInsertComboBox->clear();
    _tagsInsertComboBox->addItems(_tagsList);
}

void QConsoleWidget::showBanner(QString bannerMsg) {
    setTextCursor(_promptTextCursor);
    clear();
    setTextColor(QColor(Qt::white));
    append(bannerMsg);
    _promptTextCursor = textCursor();
}

void QConsoleWidget::showIncomingMessage(QByteArray msg) {
    setTextCursor(_promptTextCursor);
    setTextColor(QColor(Qt::yellow));
    insertPlainText(QString(msg)+QString("\n"));
    _promptTextCursor = textCursor();
}
