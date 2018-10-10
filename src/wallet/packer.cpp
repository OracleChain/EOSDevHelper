#include "packer.h"

#include <string.h>
#include <vector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariant>

#include "wallet/eoswallet.h"

namespace Packer {
std::vector<char> pack(const plain_keys &data)
{
    QJsonArray hash;
    for (auto i = 0; i < static_cast<int>(data.checksum.size()); ++i) {
        // It seems that QJsonValue take a ulonglong QVariant number Scientific notation,
        // e.g, 8153218187713844392 to 8.15321818771e+18
        // and this is NOT what we want.
        // So just use as a string.
        hash.insert(i, QJsonValue(QString::number(data.checksum._hash[i])));
    }

    QJsonArray keys;
    auto list = data.keys.keys();
    for (auto j = 0; j < list.size(); ++j) {
        QJsonObject key;
        key.insert(list.at(j), data.keys.value(list.at(j)));
        keys.append(key);
    }

    QJsonObject obj;
    obj.insert("checksum", hash);
    obj.insert("keys", keys);

    std::vector<char> vec;
    auto rawData = QJsonDocument(obj).toJson().data();
    while (*rawData) {
        vec.push_back(*rawData);
        rawData++;
    }

    return vec;
}

void unpack(const std::vector<char> &arr, plain_keys &pk)
{
    QByteArray ba;
    for (const auto& c : arr) {
        if (c == '\0') {
            break;
        }
        ba.append(c);
    }

    auto obj  = QJsonDocument::fromJson(ba).object();
    auto hash = obj.value("checksum").toArray();
    for (int j = 0; j < hash.size(); ++j) {
        pk.checksum._hash[j] = hash.at(j).toString().toULongLong();
    }

    auto keys = obj.value("keys").toArray();
    for (auto j = 0; j < keys.size(); ++j) {
        auto objKey = keys.at(j).toObject();
        auto list   = objKey.keys();
        if (!list.size()) {
            // keys are empty.
            continue;
        }
        auto key = list.at(0);
        pk.keys.insert(key, objKey.value(key).toString());
    }
}

}
