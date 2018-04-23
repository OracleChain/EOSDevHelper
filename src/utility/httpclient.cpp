#include "httpclient.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

QString base_url = "159.89.197.162";
QString url_port = "8888";
bool skip_signature = true;

QString make_url(const QString& api_url)
{
    QString url = base_url;
    if (!url.contains("http://", Qt::CaseInsensitive)
       && !url.contains("https://", Qt::CaseInsensitive)) {
        url = "http://" + base_url;
    }

    return url + (url_port.isEmpty() ? "" : QString(":" +url_port)) + api_url;
}

HttpClient::HttpClient(QObject *parent) : QObject(parent), reply(nullptr), qnam(new QNetworkAccessManager)
{
    Q_ASSERT(qnam);
}

HttpClient::~HttpClient()
{
    if (qnam) {
        delete qnam;
    }
}

void HttpClient::get_info()
{
    MakeRequest(make_url(get_info_func));
}

void HttpClient::push_transaction(const QString &content)
{
    MakeRequest(make_url(push_txn_func), content_type_application_json, content);
}

void HttpClient::get_account(const QString &content)
{
    MakeRequest(make_url(get_account_func), content_type_application_json, content);
}

void HttpClient::get_transactions(const QString &content)
{
    MakeRequest(make_url(get_transactions_func), content_type_application_json, content);
}

void HttpClient::get_controlled_accounts(const QString &content)
{
    MakeRequest(make_url(get_controlled_accounts_func), content_type_application_json, content);
}

void HttpClient::abi_json_to_bin(const QString &content)
{
    MakeRequest(make_url(json_to_bin_func), content_type_application_json, content);
}

void HttpClient::get_required_keys(const QString &content)
{
    MakeRequest(make_url(get_required_keys_func), content_type_application_json, content);
}

void HttpClient::get_table(const QString &content)
{
    MakeRequest(make_url(get_table_func), content_type_application_json, content);
}

void HttpClient::MakeRequest(const QString &url, const QString &contentType, const QString &param)
{
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
    request.setHeader(QNetworkRequest::ContentLengthHeader, param.length());
    request.setUrl(QUrl(url));
    if (!contentType.isEmpty()) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    }

    if (qnam) {
        reply = qnam->post(request, param.toUtf8());
        Q_ASSERT(reply);

        connect(reply, &QNetworkReply::finished, this, &HttpClient::slotReadAll);
    }
}

void HttpClient::slotReadAll()
{
    if (reply) {
        QByteArray data = reply->readAll();
        emit responseData(data);
    }
}
