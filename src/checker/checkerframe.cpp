#include "checkerframe.h"
#include "ui_checkerframe.h"

#include "codebase/chain/chainmanager.h"
#include "codebase/utility/httpclient.h"

#include <QMessageBox>
#include <QEventLoop>

CheckerFrame::CheckerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CheckerFrame)
{
    ui->setupUi(this);
}

CheckerFrame::~CheckerFrame()
{
    delete ui;
}

void CheckerFrame::on_pushButtonCheck_clicked()
{
    ui->textEditOutput->clear();


    QString rawAction = ui->textEditAction->toPlainText();
    QByteArray ba = QByteArray::fromStdString(rawAction.toStdString());
    QJsonDocument doc = QJsonDocument::fromJson(ba);
    if (doc.isNull()) {
        QMessageBox::warning(nullptr, "Error", "Wrong json format for raw action!");
        return;
    }

    QString signature = ui->textEditSignature->toPlainText();
    QString pubKey = ui->textEditPublicKey->toPlainText();

    std::string result;

    HttpClient httpc;
    QEventLoop loop;
    connect(&httpc, &HttpClient::responseData, [&](const QByteArray& d){
        QJsonObject obj = QJsonDocument::fromJson(d).object();
        if (!obj.isEmpty()) {
            ChainManager::ValidateSignature(rawAction.toStdString(), signature.toStdString(), pubKey.toStdString(), obj.value("chain_id").toString().toStdString(), result);
            loop.quit();
        }

    });
    httpc.request(FunctionID::get_info);

    loop.exec();

    ui->textEditOutput->setText(QString::fromStdString(result));
}
