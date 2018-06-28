#ifndef PUSHFRAME_H
#define PUSHFRAME_H

#include <QFrame>

#include "codebase/chain/signedtransaction.h"

namespace Ui {
class PushFrame;
}

class HttpClient;

class PushFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PushFrame(QWidget *parent = 0);
    ~PushFrame();

private:
    QByteArray packAbiJsonToBinParam();
    QByteArray packGetRequiredKeysParam();
    QByteArray packPushTransactionParam();

    void UpdateActionList();

private slots:
    void on_pushButtonImportFile_clicked();

    void on_pushButtonSend_clicked();

    void abi_json_to_bin_returned(const QByteArray& data);
    void get_info_returned(const QByteArray& data);
    void get_required_keys_returned(const QByteArray& data);

    void on_pushButtonGetAbi_clicked();

    void on_pushButtonFormInput_clicked();

private:
    Ui::PushFrame *ui;

    HttpClient *httpc;

    QByteArray abiJsonToBinData;
    QByteArray getInfoData;
    QByteArray getRequiredKeysData;

    QByteArray contractAbi;

    SignedTransaction signedTxn;
};

#endif // PUSHFRAME_H
