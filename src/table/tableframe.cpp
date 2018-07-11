#include "tableframe.h"
#include "ui_tableframe.h"

#include "codebase/utility/httpclient.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

TableFrame::TableFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TableFrame),
    httpc(new HttpClient)
{
    ui->setupUi(this);
    ui->lineEditScope->setValidator(new QRegExpValidator(QRegExp(eos_account_regex), this));
}

TableFrame::~TableFrame()
{
    delete ui;
    delete httpc;
}

void TableFrame::formatPrint(const QByteArray &data, QTextEdit *edit)
{
    if (!edit) {
        return;
    }

    auto doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        return;
    }

    edit->setText(QString::fromStdString(doc.toJson(QJsonDocument::Indented).toStdString()));
}

void TableFrame::on_pushButtonGetTable_clicked()
{
    QJsonObject obj;
    obj.insert("json", QJsonValue(true));
    obj.insert("table_key", QJsonValue(QString()));
    obj.insert("lower_bound", QJsonValue(QString()));
    obj.insert("upper_bound", QJsonValue(QString()));
    obj.insert("limit", QJsonValue(10));
    obj.insert("code", QJsonValue(ui->lineEditCode->text()));
    obj.insert("scope", QJsonValue(ui->lineEditScope->text()));
    obj.insert("table", QJsonValue(ui->lineEditTableName->text()));

    QJsonDocument   doc(obj);
    auto            data = doc.toJson();
    formatPrint(data, ui->textEditInput);

    if (httpc) {
        connect(httpc, &HttpClient::responseData, [=](const QByteArray& d){
            formatPrint(d, ui->textEditOutput);
        });
        httpc->request(FunctionID::get_table, data);
    }
}
