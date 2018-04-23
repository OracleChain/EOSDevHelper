#ifndef ACCOUNTFRAME_H
#define ACCOUNTFRAME_H

#include <QFrame>

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
class HttpClient;

class AccountFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AccountFrame(QWidget *parent = 0);
    ~AccountFrame();

    void refreshCreateAccountFrame();
    void printCreateAccountInfo(int index, bool request, const QByteArray& data, const QString& title = "");

private:
    void formatPrint(const QJsonDocument& doc, QTextEdit *edit);

private slots:
    void on_pushButtonGetAccount_clicked();

    void on_pushButtonGetTransactions_clicked();

    void on_pushButtonGetServants_clicked();

private:
    Ui::AccountFrame *ui;
    CreateAccountFrame *createAccountFrame;
    OutputFrame *createOutPutFrame;

    HttpClient *httpcGetAccount;
    HttpClient *httpcGetTransaction;
    HttpClient *httpcGetServants;
};

#endif // ACCOUNTFRAME_H
