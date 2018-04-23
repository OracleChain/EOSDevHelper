#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

const QString chain_func_base = "/v1/chain";
const QString get_info_func = chain_func_base + "/get_info";
const QString push_txn_func = chain_func_base + "/push_transaction";
const QString push_txns_func = chain_func_base + "/push_transactions";
const QString json_to_bin_func = chain_func_base + "/abi_json_to_bin";
const QString get_block_func = chain_func_base + "/get_block";
const QString get_account_func = chain_func_base + "/get_account";
const QString get_table_func = chain_func_base + "/get_table_rows";
const QString get_code_func = chain_func_base + "/get_code";
const QString get_currency_balance_func = chain_func_base + "/get_currency_balance";
const QString get_currency_stats_func = chain_func_base + "/get_currency_stats";
const QString get_required_keys_func = chain_func_base + "/get_required_keys";

const QString account_history_func_base = "/v1/account_history";
const QString get_transaction_func = account_history_func_base + "/get_transaction";
const QString get_transactions_func = account_history_func_base + "/get_transactions";
const QString get_key_accounts_func = account_history_func_base + "/get_key_accounts";
const QString get_controlled_accounts_func = account_history_func_base + "/get_controlled_accounts";

const QString net_func_base = "/v1/net";
const QString net_connect = net_func_base + "/connect";
const QString net_disconnect = net_func_base + "/disconnect";
const QString net_status = net_func_base + "/status";
const QString net_connections = net_func_base + "/connections";

const QString wallet_func_base = "/v1/wallet";
const QString wallet_create = wallet_func_base + "/create";
const QString wallet_open = wallet_func_base + "/open";
const QString wallet_list = wallet_func_base + "/list_wallets";
const QString wallet_list_keys = wallet_func_base + "/list_keys";
const QString wallet_public_keys = wallet_func_base + "/get_public_keys";
const QString wallet_lock = wallet_func_base + "/lock";
const QString wallet_lock_all = wallet_func_base + "/lock_all";
const QString wallet_unlock = wallet_func_base + "/unlock";
const QString wallet_import_key = wallet_func_base + "/import_key";
const QString wallet_sign_trx = wallet_func_base + "/sign_transaction";

const QString content_type_application_json = "application/json";

QString make_url(const QString& api_url);

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = 0);
    ~HttpClient();

    void get_info();
    void push_transaction(const QString& content);
    void get_account(const QString& content);
    void get_transactions(const QString& content);
    void get_controlled_accounts(const QString& content);
    void abi_json_to_bin(const QString& content);
    void get_required_keys(const QString& content);
    void get_table(const QString& content);

private:
    void MakeRequest(const QString& url, const QString& contentType = "", const QString& param = "");

signals:
    void responseData(const QByteArray& );

private slots:
    void slotReadAll();

private:
    QNetworkAccessManager *qnam;
    QNetworkReply *reply;
};

#endif // HTTPCLIENT_H
