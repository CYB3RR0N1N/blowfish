#include <iostream>
#include <string>
#include <cstring>

#include "blowfish.hpp"

using namespace std;

void parse_key(char *key)
{

}

int main(int argc, char* argv[])
{   
    string filepath;
    uint32_t *key = NULL;
    int key_len = 0;
    int state = 0;
    uint32_t default_key[4] = {0xffffffff, 0x012, 0x321, 0xfed2};

    char *tmp;
    for (int i = 1; i < argc; i++)
    {
        tmp = argv[i];
        if (tmp[0] == '-')
        {
            int str_len;
            switch (tmp[1])
            {
            case 'k':
                if (i != argc - 1)
                {
                    str_len = strlen(argv[++i]);

                    if (key_len % 8 == 0)
                        key_len = key_len / 4 + 1;
                    
                    key = new uint32_t[key_len];
                }
                else
                {
                    cout << endl << "No key detected" << endl;
                }
                break;
            case 'c':
                state = 1;
                break;
            case 'd':
                state = 2;
                break;
            default:
                cout << endl << "Unknown command arguments" << endl;
                break;
            }
        } 
        else
        {
            filepath = tmp;
        }
        
    }

    BlowfishEncrypter encrypter;

    if (key == NULL || key_len < 1)
        encrypter.key_expand(default_key, 4);
    else
        encrypter.key_expand(key,key_len);
    
    if (!filepath.empty())
    {
        switch (state)
        {
            case 1:
                encrypter.encrypt_file(filepath);
                break;
            case 2:
                encrypter.decrypt_file(filepath);
                break;
            default:
                break;
        }
    }
    else
    {
        cout << endl << "No filepath detected" << endl;
    }
    
    return 0;
}