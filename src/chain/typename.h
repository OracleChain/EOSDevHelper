#ifndef TYPENAME_H
#define TYPENAME_H

#include "chainbase.h"
#include <string>


class TypeName : public ISerializer, public IFormater
{
public:
    TypeName();
    TypeName(const std::string& name);

    virtual void serialize(EOSByteWriter *writer) const;
    virtual QJsonValue toJson() const;
    virtual void fromJson(const QJsonValue& value);

    std::string to_string() const;

    static LONG name_string_to_long(const std::string& name);
    static std::string long_name_to_string(LONG name);

private:
    static unsigned char char_to_symbol(char c);

private:
    LONG value;

private:
    enum {
      MAX_NAME_IDX = 12,
    };
};

#endif // TYPENAME_H
