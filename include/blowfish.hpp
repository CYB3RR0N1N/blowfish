#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <cstdint>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "../include/pi_mantiss.hpp"

class BlowfishEncrypter
{
public:
    BlowfishEncrypter();
    BlowfishEncrypter(uint32_t *key, int len); //Creates object and expand given key
    void encrypt_block(uint32_t &left, uint32_t &right); //Encrypting two blocks with own key
    void decrypt_block(uint32_t &left, uint32_t &right); //Decrypting two blocks with own key
    void key_expand(uint32_t *key, int len);
    void encrypt_file(std::string in_filepath,std::string out_filepath = "");
    void decrypt_file(std::string in_filepath,std::string out_filepath = "");

private:
    uint32_t key[18];
    uint32_t sbox[4][256];

    uint32_t F(uint32_t x);
};

#endif