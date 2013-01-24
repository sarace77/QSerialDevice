#ifndef QCONSOLEWIDGET_H
#define QCONSOLEWIDGET_H

#include <QComboBox>
#include <QKeyEvent>
#include <QTextCursor>
#include <QTextEdit>

class QConsoleWidget : public QTextEdit
{
    Q_OBJECT

private:
    bool _newTagsList;
    QTextCursor _promptTextCursor;
    QComboBox *_tagsInsertComboBox;
    QStringList _tagsList;


protected:
    void keyPressEvent(QKeyEvent *event);

protected slots:
    void insertProtocolTag(QString tag);

public:
    explicit QConsoleWidget(QWidget *parent = 0);
    virtual ~QConsoleWidget();
    
public slots:
    void refreshProtocolTags(QStringList tags);
    void showBanner(QString bannerMsg = QString("Console succesfully initialized\n\n"));
    void showIncomingMessage(QByteArray msg);

signals:
    void newOutgoingMessage(QByteArray msg);
};

#endif // QCONSOLEWIDGET_H
