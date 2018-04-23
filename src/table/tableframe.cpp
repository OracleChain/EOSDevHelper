#include "tableframe.h"
#include "ui_tableframe.h"
#include "utility/httpclient.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

TableFrame::TableFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TableFrame),
    httpc(new HttpClient)
{
    ui->setupUi(this);
    QRegExp accountRegex("([.1-5a-z]{3,13}$)");
    QRegExpValidator *accountVadt = new QRegExpValidator(accountRegex, this);
    ui->lineEditScope->setValidator(accountVadt);
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

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        return;
    }

    QByteArray formatData = doc.toJson(QJsonDocument::Indented);
    edit->setText(QString::fromStdString(formatData.toStdString()));
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

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    formatPrint(data, ui->textEditInput);

    if (httpc) {
        httpc->get_table(QString::fromStdString(data.toStdString()));
        connect(httpc, &HttpClient::responseData, [=](const QByteArray& data){
            formatPrint(data, ui->textEditOutput);
        });
    }
}
