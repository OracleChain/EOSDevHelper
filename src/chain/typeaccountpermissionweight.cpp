#include "typeaccountpermissionweight.h"
#include "eosbytewriter.h"

TypeAccountPermissionWeight::TypeAccountPermissionWeight()
{

}

TypeAccountPermissionWeight::TypeAccountPermissionWeight(const std::string &name, short weight)
{
    this->permission = TypePermissionLevel(name, "active");
    this->weight = weight;
}

void TypeAccountPermissionWeight::serialize(EOSByteWriter *writer) const
{
    if (writer) {
        permission.serialize(writer);
        writer->putShortLE(weight);
    }
}
