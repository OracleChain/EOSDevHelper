#include "transactionheader.h"
#include "eosbytewriter.h"
#include "utility/utils.h"

#include <QDateTime>
#include <QVariant>

TransactionHeader::TransactionHeader()
{
    region = 0;
    ref_block_num = 0;
    ref_block_prefix = 0;
    net_usage_words = 0;
    kcpu_usage = 0;
    delay_seconds = 0;
}

void TransactionHeader::serialize(EOSByteWriter *writer) const
{
   if (writer) {
       QDateTime date = QDateTime::fromString(QString::fromStdString(expiration), Qt::ISODate);
       writer->putIntLE((int)(date.toMSecsSinceEpoch() / 1000 + date.offsetFromUtc() + EXPIRATION_SEC));
       writer->putShortLE((short)(region & 0xFFFF));
       writer->putShortLE((short)ref_block_num & 0xFFFF);
       writer->putIntLE((int)(ref_block_prefix & 0xFFFFFFFF));
       writer->putVariableUInt(net_usage_words);
       writer->putVariableUInt(kcpu_usage);
       writer->putVariableUInt(delay_seconds);
   }
}

QJsonValue TransactionHeader::toJson() const
{
    QJsonObject obj;
    obj.insert("expiration", QJsonValue(expiration.c_str()));
    obj.insert("region", QJsonValue((int)region));
    obj.insert("ref_block_num", QJsonValue((int)ref_block_num));
    obj.insert("ref_block_prefix", QJsonValue::fromVariant(QVariant((uint)ref_block_prefix)));
    obj.insert("net_usage_words", QJsonValue((int)net_usage_words));
    obj.insert("kcpu_usage", QJsonValue((int)kcpu_usage));
    obj.insert("delay_sec", QJsonValue((int)delay_seconds));

    return QJsonValue(obj);
}

void TransactionHeader::fromJson(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    if (obj.isEmpty()) {
        return;
    }

    expiration = obj.value("expiration").toString().toStdString();
    region = obj.value("region").toInt();
    ref_block_num = obj.value("ref_block_num").toInt();
    ref_block_prefix = obj.value("ref_block_prefix").toVariant().toUInt();
    net_usage_words = obj.value("net_usage_words").toInt();
    kcpu_usage = obj.value("kcpu_usage").toInt();
    delay_seconds = obj.value("delay_sec").toInt();
}

void TransactionHeader::setExpiration(const std::string &expiration)
{
    this->expiration = expiration;
}

void TransactionHeader::setReferenceBlock(const std::string &ref)
{
    QString ref_block = QString::fromStdString(ref);
    ref_block_num = ref_block.left(8).toInt(nullptr, 16);
    ref_block_prefix = Utils::convertEndian(ref_block.mid(16, 8).toUInt(nullptr, 16));
}

void TransactionHeader::setNetUsageWords(long long net_usage)
{
    net_usage_words = net_usage;
}

void TransactionHeader::setKcpuUsage(long long kcpu)
{
    kcpu_usage = kcpu;
}
