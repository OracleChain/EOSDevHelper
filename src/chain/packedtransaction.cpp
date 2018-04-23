#include "packedtransaction.h"
#include "eosbytewriter.h"

#include <QDebug>

PackedTransaction::PackedTransaction(const SignedTransaction &signedTxn, const std::string &compress)
{
    compression = compress;
    signatures = signedTxn.getSignatures();

    compress_type ct;
    if (compress.compare("none") == 0) {
        ct = compress_type::COMPRESS_NONE;
    } else {
        ct = compress_type::COMPRESS_ZLIB;
    }

    std::vector<unsigned char> hexPkedTxn = convertBytesToHexStr(packTxn(signedTxn, ct));

    packed_trx = std::string(hexPkedTxn.begin(), hexPkedTxn.end());

    std::vector<std::string> ctxFreeData = signedTxn.getCtxFreeData();
    std::vector<unsigned char> hexPkedCtx = convertBytesToHexStr(packCtxFreeData(ctxFreeData, ct));

    packed_context_free_data = std::string(hexPkedCtx.begin(), hexPkedCtx.end());
}

QJsonValue PackedTransaction::toJson() const
{
    QJsonObject obj;
    obj.insert("compression", QJsonValue(QString::fromStdString(compression)));
    obj.insert("packed_context_free_data", QJsonValue(QString::fromStdString(packed_context_free_data)));
    obj.insert("packed_trx", QJsonValue(QString::fromStdString(packed_trx)));

    QJsonArray signatureArr;
    for (int i = 0; i < (int)signatures.size(); ++i) {
        signatureArr.append(QJsonValue(QString::fromStdString(signatures.at(i))));
    }

    obj.insert("signatures", signatureArr);

    return QJsonValue(obj);
}

void PackedTransaction::fromJson(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    if (obj.isEmpty()) {
        return;
    }

    compression = obj.value("compression").toString().toStdString();
    packed_context_free_data = obj.value("packed_context_free_data").toString().toStdString();
    packed_trx = obj.value("packed_trx").toString().toStdString();

    QJsonArray signatureArr = obj.value("signatures").toArray();
    if (!signatureArr.isEmpty()) {
        for (int i = 0; i < signatureArr.size(); ++i)
        signatures.push_back(signatureArr.at(i).toString().toStdString());
    }
}

std::vector<unsigned char> PackedTransaction::packCtxFreeData(const std::vector<std::string> &ctxFreeData, PackedTransaction::compress_type ct)
{
    EOSByteWriter writer;

    int ctxCnt = ctxFreeData.size();
    writer.putVariableUInt(ctxCnt);
    if (!ctxCnt) {
        return writer.toBytes();
    }

    for (int i = 0; i < ctxCnt; ++i) {
        std::string hexStr = ctxFreeData.at(i);
        std::vector<unsigned char> bytes = convertHexStrToBytes(std::vector<unsigned char>(hexStr.begin(), hexStr.end()));

        writer.putBytes(bytes.data(), bytes.size());
    }

    return compress(writer.toBytes(), ct);
}

std::vector<unsigned char> PackedTransaction::packTxn(const SignedTransaction &txn, PackedTransaction::compress_type ct)
{
    EOSByteWriter writer;
    txn.serialize(&writer);

    return compress(writer.toBytes(), ct);
}

std::vector<unsigned char> PackedTransaction::compress(const std::vector<unsigned char> &src, compress_type cmp)
{
    if (cmp == compress_type::COMPRESS_NONE || cmp != compress_type::COMPRESS_ZLIB) {
        return src;
    }

    // use zip to compress.
    // FIX ME!

    std::vector<unsigned char> dst;
    return dst;
}
