#include <iostream>
#include <string>


#include "blowfish.hpp"

using namespace std;

union block
{
    uint32_t dword;
    struct 
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    };
};

int main(int argc, char* argv[])
{
    BlowfishEncrypter encrypter;
    uint32_t a = 0xf0, b= 0x0f;
    uint32_t key[4] = {0xff, 0x012, 0x321, 0xfed2};
    encrypter.key_expand(key, 4);
    encrypter.encrypt_block(a,b);
    encrypter.decrypt_block(a,b);
    if ( a == 0xf0 && b ==0x0f)
        cout << "Success\n";
    cout << "Done\n";
    return 0;
}