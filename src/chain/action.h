#ifndef ACTION_H
#define ACTION_H

#include "typename.h"
#include "typepermissionlevel.h"
#include <vector>

class Action : public ISerializer, public IFormater
{
public:
    Action();
    Action(const TypeName& account, const TypeName& action, const TypePermissionLevel& auth, const std::string& data);
    Action(const TypeName& account, const TypeName& action);

    virtual void serialize(EOSByteWriter* writer) const;
    virtual QJsonValue toJson() const;
    virtual void fromJson(const QJsonValue& value);

    void setAccount(const TypeName& account);
    void setAction(const TypeName& action);
    void addAuthorization(const TypePermissionLevel& auth);
    void setAuthorization(const std::vector<TypePermissionLevel>& authorization);
    void setAuthorization(const std::vector<std::string>& accountWithPermissionLvl);
    void setData(const std::string& data);

private:
    TypeName account;
    TypeName action;
    std::vector<TypePermissionLevel> authorization;
    std::string data;
};

#endif // ACTION_H
