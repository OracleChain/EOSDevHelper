#include "eosnewaccount.h"
#include "eosbytewriter.h"

EOSNewAccount::EOSNewAccount()
{

}

EOSNewAccount::EOSNewAccount(const std::string &creator, const std::string &newName, const TypeAuthority &owner, const TypeAuthority &active, const TypeAuthority &recovery)
{
    this->creator = TypeName(creator);
    this->newName = TypeName(newName);
    this->owner = owner;
    this->active = active;
    this->recovery = recovery;
}

EOSNewAccount::EOSNewAccount(const std::string &creator, const std::string &newName, const std::string &owner, const std::string &active, const std::string &recovery)
{
    this->creator = TypeName(creator);
    this->newName = TypeName(newName);
    this->owner = TypeAuthority(1, TypeKeyPermissionWeight(owner));
    this->active = TypeAuthority(1, TypeKeyPermissionWeight(active));
    this->recovery = TypeAuthority(1, TypeAccountPermissionWeight(recovery));
}

void EOSNewAccount::serialize(EOSByteWriter *writer) const
{
    if (writer) {
        creator.serialize(writer);
        newName.serialize(writer);
        owner.serialize(writer);
        active.serialize(writer);
        recovery.serialize(writer);
    }
}

std::string EOSNewAccount::getCreatorName()
{
    return creator.to_string();
}

std::string EOSNewAccount::getActionName() const
{
    return "newaccount";
}

std::vector<unsigned char> EOSNewAccount::data()
{
    EOSByteWriter writer;
    serialize(&writer);

    return writer.toBytes();
}

std::vector<unsigned char> EOSNewAccount::dataAsHex()
{
    return convertBytesToHexStr(data());;
}
