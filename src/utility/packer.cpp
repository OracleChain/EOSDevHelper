#include "packer.h"

#include <string.h>
#include <vector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariant>

#include "wallet/eoswallet.h"

Packer::Packer()
{

}

std::vector<char> Packer::pack(const plain_keys &data)
{
    std::vector<char> vec;

    QJsonObject obj;
    QJsonArray hash;
    for (int i = 0; i < (int)data.checksum.size(); ++i) {
        // It seems that QJsonValue take a ulonglong QVariant number Scientific notation,
        // e.g, 8153218187713844392 to 8.15321818771e+18
        // and this is NOT what we want.
        // So just use as a string.

        QString strHash = QString::number(data.checksum._hash[i]);
        hash.insert(i, QJsonValue(strHash));
    }

    obj.insert("checksum", hash);

    QJsonArray keys;
    QStringList list = data.keys.keys();
    for (int j = 0; j < list.size(); ++j) {
        QJsonObject key;
        key.insert(list.at(j), data.keys.value(list.at(j)));
        keys.append(key);
    }

    obj.insert("keys", keys);

    QJsonDocument doc(obj);
    QByteArray ba = doc.toJson();
    char *rawData = ba.data();
    while (*rawData) {
        vec.push_back(*rawData);
        rawData++;
    }

    return vec;
}

void Packer::unpack(const std::vector<char> &arr, plain_keys &pk)
{
    QByteArray ba;
    for (const auto& c : arr) {
        if (c == '\0') {
            break;
        }

        ba.append(c);
    }

    QJsonDocument doc = QJsonDocument::fromJson(ba);
    QJsonObject obj = doc.object();

    QJsonArray hash = obj.value("checksum").toArray();
    for (int j = 0; j < hash.size(); ++j) {
        pk.checksum._hash[j] = hash.at(j).toString().toULongLong();
    }

    QJsonArray keys = obj.value("keys").toArray();
    for (int j = 0; j < keys.size(); ++j) {
        QJsonObject objKey = keys.at(j).toObject();
        QStringList list = objKey.keys();
        if (!list.size()) {
            // keys are empty.
            continue;
        }
        QString key = list.at(0);
        pk.keys.insert(key, objKey.value(key).toString());
    }
}
