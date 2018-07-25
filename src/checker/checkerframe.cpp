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

    auto rawAction  = ui->textEditAction->toPlainText();
    auto doc        = QJsonDocument::fromJson(QByteArray::fromStdString(rawAction.toStdString()));
    if (doc.isNull()) {
        QMessageBox::warning(nullptr, "Error", "Wrong json format for raw action!");
        return;
    }

    auto signature  = ui->textEditSignature->toPlainText();
    auto pubKey     = ui->textEditPublicKey->toPlainText();

    std::string result;

    HttpClient httpc;
    QEventLoop loop;
    connect(&httpc, &HttpClient::responseData, [&](const QByteArray& d){
        auto obj = QJsonDocument::fromJson(d).object();
        if (!obj.isEmpty()) {
            ChainManager::ValidateSignature(rawAction.toStdString(), signature.toStdString(),
                                            pubKey.toStdString(), obj.value("chain_id").toString().toStdString(), result);
            loop.quit();
        }

    });
    httpc.request(FunctionID::get_info);

    loop.exec();

    ui->textEditOutput->setText(QString::fromStdString(result));
}
