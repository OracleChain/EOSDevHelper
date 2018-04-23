#ifndef TRANSACTIONHEADER_H
#define TRANSACTIONHEADER_H

#include "chainbase.h"
#include <string>

class TransactionHeader : public ISerializer, public IFormater
{
public:
    TransactionHeader();

    virtual void serialize(EOSByteWriter* writer) const;
    virtual QJsonValue toJson() const;
    virtual void fromJson(const QJsonValue& value);

    void setExpiration(const std::string& expiration);
    void setReferenceBlock(const std::string& ref);
    void setNetUsageWords(LONG net_usage);
    void setKcpuUsage(LONG kcpu);

private:
    std::string expiration;
    int region;
    int ref_block_num;
    LONG ref_block_prefix;
    LONG net_usage_words;
    LONG kcpu_usage;
    LONG delay_seconds;

private:
    const static int EXPIRATION_SEC = 30;
};

#endif // TRANSACTIONHEADER_H
