#ifndef TYPECHAINID_H
#define TYPECHAINID_H


class TypeChainId
{
public:
    TypeChainId();

    void *getBytes();
    const unsigned char *chainId() const;

    int size () const { return sizeof(id); }

private:
    unsigned char id[32];
};

#endif // TYPECHAINID_H
