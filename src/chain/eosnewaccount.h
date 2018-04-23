#ifndef EOSNEWACCOUNT_H
#define EOSNEWACCOUNT_H

#include "typename.h"
#include "typeauthority.h"

class EOSNewAccount : public ISerializer
{
public:
    EOSNewAccount();
    EOSNewAccount(const std::string& creator, const std::string& newName, const TypeAuthority& owner, const TypeAuthority& active, const TypeAuthority& recovery);
    EOSNewAccount(const std::string& creator, const std::string& newName, const std::string& owner, const std::string& active, const std::string& recovery);

    virtual void serialize(EOSByteWriter *writer) const;

    std::string getCreatorName();
    std::string getActionName() const;

    std::vector<unsigned char> data();
    std::vector<unsigned char> dataAsHex();

private:
    TypeName creator;
    TypeName newName;
    TypeAuthority owner;
    TypeAuthority active;
    TypeAuthority recovery;
};

#endif // EOSNEWACCOUNT_H
