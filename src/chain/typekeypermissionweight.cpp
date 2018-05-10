#include "typekeypermissionweight.h"
#include "eosbytewriter.h"

TypeKeyPermissionWeight::TypeKeyPermissionWeight()
{
    weight = 0;
}

TypeKeyPermissionWeight::TypeKeyPermissionWeight(const std::string &pubKey, short weight)
{
    this->pubKey = pubKey;
    this->weight = weight;
}

void TypeKeyPermissionWeight::serialize(EOSByteWriter *writer) const
{
    if (writer) {
        writer->putVariableUInt(0); // pubKey.isCurvePramK1 ? 0 : 1

        std::vector<unsigned char> pub = eos_key::get_public_key_char(pubKey);
        if (!pub.empty()) {
            writer->putBytes(pub.data(), pub.size());
        }
        //writer->putBytes((const unsigned char*)pubKey.c_str(), pubKey.size());

        writer->putShortLE(weight);
    }
}
