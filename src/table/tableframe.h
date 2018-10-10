#ifndef TABLEFRAME_H
#define TABLEFRAME_H

#include <QFrame>
#include <QMap>
#include <memory>

#include "codebase/utility/httpclient.h"

namespace Ui {
class TableFrame;
}

class QTextEdit;

class TableFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TableFrame(QWidget *parent = nullptr);
    ~TableFrame();

private:
    void formatPrint(const QByteArray& data, QTextEdit *edit);

    void updateTableList();
    void initHttpClients();

private slots:
    void on_pushButtonGetTable_clicked();

    void on_pushButtonGetStruct_clicked();

private:
    Ui::TableFrame              *ui;
    QMap<FunctionID, std::shared_ptr<HttpClient>> httpcs;
    QByteArray contractAbi;
};

#endif // TABLEFRAME_H
