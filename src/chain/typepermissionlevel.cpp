#include "typepermissionlevel.h"
#include "eosbytewriter.h"

TypePermissionLevel::TypePermissionLevel()
{

}

TypePermissionLevel::TypePermissionLevel(const std::string &accountName, const std::string &permissionName)
{
    actor = TypeName(accountName);
    permission = TypeName(permissionName);
}

void TypePermissionLevel::serialize(EOSByteWriter *writer) const
{
    if (writer) {
        actor.serialize(writer);
        permission.serialize(writer);
    }
}

QJsonValue TypePermissionLevel::toJson() const
{
    QJsonObject obj;
    obj.insert("actor", actor.toJson());
    obj.insert("permission", permission.toJson());

    return QJsonValue(obj);
}

void TypePermissionLevel::fromJson(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    if (obj.isEmpty()) {
        return;
    }

    actor.fromJson(obj.value("actor"));
    permission.fromJson(obj.value("permission"));
}

void TypePermissionLevel::setAccount(const std::string &accountName)
{
    actor = TypeName(accountName);
}

void TypePermissionLevel::setPermission(const std::string &permissionName)
{
    permission = TypeName(permissionName);
}
