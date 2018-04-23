#ifndef TYPEPERMISSIONLEVEL_H
#define TYPEPERMISSIONLEVEL_H

#include "typename.h"
#include <string>

class TypePermissionLevel : public ISerializer, public IFormater
{
public:
    TypePermissionLevel();
    TypePermissionLevel(const std::string& accountname, const std::string& permissionName);

    virtual void serialize(EOSByteWriter* writer) const;
    virtual QJsonValue toJson() const;
    virtual void fromJson(const QJsonValue& value);

    void setAccount(const std::string& accountName);
    void setPermission(const std::string& permissionName);


private:
    TypeName actor;
    TypeName permission;
};

#endif // TYPEPERMISSIONLEVEL_H
