#include "typeauthority.h"
#include "eosbytewriter.h"

TypeAuthority::TypeAuthority()
{

}

TypeAuthority::TypeAuthority(int threshold, const TypeKeyPermissionWeight &key)
{
    this->threshold = threshold;
    this->keys.push_back(key);
}

TypeAuthority::TypeAuthority(int threshold, const TypeAccountPermissionWeight &permission)
{
    this->threshold = threshold;
    this->accounts.push_back(permission);
}

TypeAuthority::TypeAuthority(int threshold, const TypeKeyPermissionWeight& key, const TypeAccountPermissionWeight& permission)
{
    this->threshold = threshold;
    this->keys.push_back(key);
    this->accounts.push_back(permission);
}

TypeAuthority::TypeAuthority(int threshold, const std::string &pubKey, const std::string &permission)
{
    this->threshold = threshold;
    if (!pubKey.empty()) {
        this->keys.push_back(TypeKeyPermissionWeight(pubKey, 1));

    }
    if (!permission.empty()) {
        this->accounts.push_back(TypeAccountPermissionWeight(permission));
    }
}

void TypeAuthority::serialize(EOSByteWriter *writer) const
{
    if (writer) {
        writer->putIntLE(threshold);
        SerializeCollection<TypeKeyPermissionWeight>(keys, writer);
        SerializeCollection<TypeAccountPermissionWeight>(accounts, writer);
        //writer->putCollection(keys);
        //writer->putCollection(accounts);
    }
}
