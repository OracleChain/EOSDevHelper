#include "checkerframe.h"
#include "ui_checkerframe.h"
#include "chain/chainmanager.h"

#include <QMessageBox>

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
    ChainManager::ValidateSignature(rawAction.toStdString(), signature.toStdString(), pubKey.toStdString(), result);

    ui->textEditOutput->setText(QString::fromStdString(result));
}
