#ifndef CREATEACCOUNTFRAME_H
#define CREATEACCOUNTFRAME_H

#include <QFrame>
#include <QPair>
#include <QMap>
#include <memory>

#include "codebase/chain/signedtransaction.h"
#include "codebase/utility/httpclient.h"

namespace Ui {
class CreateAccountFrame;
}

class eos_key;

class CreateAccountFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CreateAccountFrame(QWidget *parent = nullptr);
    ~CreateAccountFrame();

    void initWallets();
    void initHttpClients();

private slots:
    void on_pushButtonOk_clicked();

    void get_info_returned(const QByteArray& data);
    void get_required_keys_returned(const QByteArray& data);
    void push_transaction_returned(const QByteArray& data);

    void on_checkBoxPractical_stateChanged(int arg1);

private:
    void enable_resource_ui(bool enable);
    void serilize_json();

    void geneate_keys();

    QByteArray packGetRequiredKeysParam();
    QByteArray packPushTransactionParam();

private:
    Ui::CreateAccountFrame *ui;

    QVector<eos_key>                                keys;
    QMap<FunctionID, std::shared_ptr<HttpClient>>   httpcs;
    std::shared_ptr<HttpClient>                     delegatebw_httpc;
    std::shared_ptr<HttpClient>                     buyram_httpc;
    std::unique_ptr<SignedTransaction>              signedTxn;
    QMap<std::string, std::string>                  binargs;

    QByteArray getInfoData;
    QByteArray getRequiredKeysData;

};

#endif // CREATEACCOUNTFRAME_H
