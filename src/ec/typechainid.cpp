#include "typechainid.h"

#include <string.h>

TypeChainId::TypeChainId()
{
    memset(id, 0, sizeof(id));
}

void *TypeChainId::getBytes()
{
    return (void*)id;
}

const unsigned char *TypeChainId::chainId() const
{
    return id;
}
