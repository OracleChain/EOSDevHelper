#ifndef EOSBYTEWRITER_H
#define EOSBYTEWRITER_H

#include <string>
#include <vector>
#include <string.h>

#include "chainbase.h"

class TypeChainId;
class QJsonObject;
class QJsonArray;

class EOSByteWriter
{
public:
    EOSByteWriter();
    EOSByteWriter(int capacity);
    EOSByteWriter(const std::vector<unsigned char>& buf);
    ~EOSByteWriter();

    void put(unsigned char b);
    void putShortLE(short value);
    void putIntLE(int value);
    void putLongLE(LONG value);
    void putBytes(const unsigned char *value, int len);
    void putBytes(const unsigned char *value, int offset, int len);
    void putString(const std::string& str);
    void putVariableUInt(LONG val);

    std::vector<unsigned char> toBytes();
    int length();

private:
    std::vector<unsigned char> buf;
};

// gcc do not support template which is a member, use this.
template<typename T>
void SerializeCollection(const std::vector<T>& list, EOSByteWriter *writer)
{
    if (writer) {
        writer->putVariableUInt(list.size());
        for (auto item : list) {
            item.serialize(writer);
        }
    }
}

#endif // EOSBYTEWRITER_H
