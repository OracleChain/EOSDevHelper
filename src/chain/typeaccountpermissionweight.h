#ifndef TYPEACCOUNTPERMISSIONWEIGHT_H
#define TYPEACCOUNTPERMISSIONWEIGHT_H

#include "typepermissionlevel.h"
#include <string>

class TypeAccountPermissionWeight : public ISerializer
{
public:
    TypeAccountPermissionWeight();
    TypeAccountPermissionWeight(const std::string& name, short weight = 1);

    virtual void serialize(EOSByteWriter* writer) const;

private:
    TypePermissionLevel permission;
    short weight;
};

#endif // TYPEACCOUNTPERMISSIONWEIGHT_H
