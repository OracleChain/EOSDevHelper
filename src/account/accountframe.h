#ifndef ACCOUNTFRAME_H
#define ACCOUNTFRAME_H

#include <QFrame>
#include <QMap>
#include <memory>

#include "codebase/utility/httpclient.h"

namespace Ui {
class AccountFrame;
}

class QJsonDocument;
class QTextEdit;

class GetAccountDialog;
class CreateAccountFrame;
class GetTransactionDialog;
class GetControlledAccountsDialog;
class OutputFrame;

class AccountFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AccountFrame(QWidget *parent = nullptr);
    ~AccountFrame();

    void refreshCreateAccountFrame();
    void printCreateAccountInfo(int index, bool request, const QByteArray& data, const QString& title = "");
    void clearOutput();

private:
    void formatPrint(const QJsonDocument& doc, QTextEdit *edit);
    void initHttpClients();
    void initUI();

private slots:
    void on_pushButtonGetAccount_clicked();

    void on_pushButtonGetTransactions_clicked();

    void on_pushButtonGetServants_clicked();

private:
    Ui::AccountFrame    *ui;
    CreateAccountFrame  *createAccountFrame;
    OutputFrame         *createOutPutFrame;

    QMap<FunctionID, std::shared_ptr<HttpClient>> httpcs;
};

#endif // ACCOUNTFRAME_H
