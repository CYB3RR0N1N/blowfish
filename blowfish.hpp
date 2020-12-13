#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <cstdint>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "pi_mantiss.hpp"

class BlowfishEncrypter
{
public:
    BlowfishEncrypter();
    void encrypt_block(uint32_t &left, uint32_t &right);
    void decrypt_block(uint32_t &left, uint32_t &right);
    void key_expand(uint32_t *key, int len);
    void encrypt_file(std::string filename);
    void decrypt_file(std::string filename);

private:
    uint32_t key[18];
    uint32_t sbox[4][256];

    uint32_t F(uint32_t x);
};

#endif