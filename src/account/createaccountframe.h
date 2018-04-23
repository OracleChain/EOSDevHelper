#ifndef CREATEACCOUNTFRAME_H
#define CREATEACCOUNTFRAME_H

#include <QFrame>
#include <QPair>

#include "chain/signedtransaction.h"

namespace Ui {
class CreateAccountFrame;
}

class eos_key;
class HttpClient;

class CreateAccountFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CreateAccountFrame(QWidget *parent = 0);
    ~CreateAccountFrame();

    void initWallets();

private slots:
    void on_pushButtonOk_clicked();

    void get_info_returned(const QByteArray& data);
    void get_required_keys_returned(const QByteArray& data);
    void push_transaction_returned(const QByteArray& data);

private:
    void geneate_keys();

    QByteArray packGetRequiredKeysParam();
    QByteArray packPushTransactionParam();

private:
    Ui::CreateAccountFrame *ui;

    QVector<eos_key> keys;
    HttpClient *httpc;

    QByteArray getInfoData;
    QByteArray getRequiredKeysData;

    SignedTransaction signedTxn;
};

#endif // CREATEACCOUNTFRAME_H
