#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <cstdint>
#include <algorithm>
#include "pi_mantiss.hpp"

class BlowfishEncrypter
{
public:
    void encrypt_block(uint32_t &left, uint32_t &right);
    void decrypt_block(uint32_t &left, uint32_t &right);
    void key_expand(uint32_t *key, int len);

private:
    uint32_t key[18];
    uint32_t sbox[4][256];

    uint32_t F(uint32_t x);
};

#endif