#ifndef PUSHFRAME_H
#define PUSHFRAME_H

#include <QFrame>
#include <QMap>
#include <memory>

#include "codebase/chain/signedtransaction.h"
#include "codebase/utility/httpclient.h"

namespace Ui {
class PushFrame;
}

class PushFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PushFrame(QWidget *parent = nullptr);
    ~PushFrame();

private:
    QByteArray packAbiJsonToBinParam();
    QByteArray packGetRequiredKeysParam();
    QByteArray packPushTransactionParam();

    void updateActionList();
    void initHttpClients();

private slots:
    void on_pushButtonImportFile_clicked();
    void on_pushButtonSend_clicked();
    void on_pushButtonGetAbi_clicked();
    void on_pushButtonFormInput_clicked();

    void abi_json_to_bin_returned(const QByteArray& data);
    void get_info_returned(const QByteArray& data);
    void get_required_keys_returned(const QByteArray& data);

private:
    Ui::PushFrame *ui;

    QMap<FunctionID, std::shared_ptr<HttpClient>> httpcs;
    
    QByteArray abiJsonToBinData;
    QByteArray getInfoData;
    QByteArray getRequiredKeysData;
    QByteArray contractAbi;
    
    SignedTransaction signedTxn;
};

#endif // PUSHFRAME_H
