#include <iostream>
#include <string>


#include "blowfish.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    BlowfishEncrypter encrypter;
    uint32_t key[4] = {0xffffffff, 0x012, 0x321, 0xfed2};
    encrypter.key_expand(key, 4);
    
    encrypter.encrypt_file("files/test.txt");
    encrypter.decrypt_file("files/crypted.dat");
    return 0;
}