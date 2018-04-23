#ifndef TABLEFRAME_H
#define TABLEFRAME_H

#include <QFrame>

namespace Ui {
class TableFrame;
}

class HttpClient;
class QTextEdit;

class TableFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TableFrame(QWidget *parent = 0);
    ~TableFrame();

private:
    void formatPrint(const QByteArray& data, QTextEdit *edit);

private slots:
    void on_pushButtonGetTable_clicked();

private:
    Ui::TableFrame *ui;

    HttpClient *httpc;
};

#endif // TABLEFRAME_H
