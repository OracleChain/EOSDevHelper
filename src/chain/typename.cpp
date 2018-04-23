#include "typename.h"
#include "eosbytewriter.h"

#include <QRegularExpression>

static const char char_map[] = ".12345abcdefghijklmnopqrstuvwxyz";

TypeName::TypeName()
{
    value = 0;
}

TypeName::TypeName(const std::string &name)
{
    value = name_string_to_long(name);
}

void TypeName::serialize(EOSByteWriter *writer) const
{
    if (writer) {
        writer->putLongLE(value);
    }
}

QJsonValue TypeName::toJson() const
{
    return QJsonValue((const char *)to_string().c_str());
}

void TypeName::fromJson(const QJsonValue &value)
{
    this->value = name_string_to_long(value.toString().toStdString());
}

std::string TypeName::to_string() const
{
    return long_name_to_string(value);
}

LONG TypeName::name_string_to_long(const std::string &name)
{
    if (name.empty()) {
        return 0L;
    }

    int len = name.length();
    LONG value = 0;

    for (int i = 0; i <= MAX_NAME_IDX; ++i) {
        LONG c = 0;

        if (i < len && i <= MAX_NAME_IDX) {
            c = char_to_symbol(name.at(i));
        }

        if (i < MAX_NAME_IDX) {
            c &= 0x1f;
            c <<= 64 - 5 * (i + 1);
        } else {
            c &= 0x0f;
        }

        value |= c;
    }

    return value;
}

std::string TypeName::long_name_to_string(LONG val)
{
    std::vector<char> vec(MAX_NAME_IDX+1, '\0');
    LONG temp = val;
    for (int i = 0; i <= MAX_NAME_IDX; ++i) {
        char c = char_map[(int)(temp & (i == 0 ? 0x0f : 0x1f))];
        vec[MAX_NAME_IDX - i] = c;
        temp >>= (i == 0 ? 4 : 5);
    }

    QString strRaw = QString::fromStdString(std::string(vec.begin(), vec.end()));
    strRaw = strRaw.replace(QRegularExpression("[.]+$"), "");
    return strRaw.toStdString();
}

unsigned char TypeName::char_to_symbol(char c)
{
    if (c >= 'a' && c <= 'z') {
        return (unsigned char)((c - 'a') + 6);
    }

    if (c >= '1' && c <= '5') {
        return (unsigned char)((c - '1') + 1);
    }

    return (unsigned char)0;
}
