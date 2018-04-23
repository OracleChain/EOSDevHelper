#ifndef TYPEAUTHORITY_H
#define TYPEAUTHORITY_H

#include "typeaccountpermissionweight.h"
#include "typekeypermissionweight.h"
#include <vector>

class TypeAuthority : public ISerializer
{
public:
    TypeAuthority();
    TypeAuthority(int threshold, const TypeKeyPermissionWeight& key);
    TypeAuthority(int threshold, const TypeAccountPermissionWeight& permission);
    TypeAuthority(int threshold, const TypeKeyPermissionWeight& key, const TypeAccountPermissionWeight& permission);
    TypeAuthority(int threshold, const std::string& pubKey, const std::string& permission);

    virtual void serialize(EOSByteWriter *writer) const;

private:
    int threshold;
    std::vector<TypeKeyPermissionWeight> keys;     // TypeKeyPermissionWeight
    std::vector<TypeAccountPermissionWeight> accounts; // TypeAccountPermissionWeight
};

#endif // TYPEAUTHORITY_H
