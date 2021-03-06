#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

#include "../include/blowfish.hpp"
#include "../include/simple_parser.hpp"

using namespace std;

uint32_t* parse_key(const char *arg, int &len)
{
    uint32_t* key;
    int dif;
    char buf[8];
    int uintcheck = 0, keynum = 0;
    if (arg[0] == '0' && arg[1] == 'x')
      arg += 2; 
    int lenght = strlen(arg);
    if (lenght % 8 == 0)
        len = lenght / 8;
    else
        len = lenght / 8 + 1;
    key = new uint32_t[len];

    for (int i = 0; i < lenght; i++)
    {
        buf[uintcheck] = arg[i];
        uintcheck++;
        if (uintcheck == 8)
        {
            key[keynum] = strtoul(buf, NULL, 16);
            uintcheck = 0;
            keynum++;
        }
        if ((i == lenght - 1) && (uintcheck != 8))
        {
            dif = 8 - uintcheck;
            for (int i = 7; i >= dif; i--)
                buf[i] = buf[i - dif];
            for (int i = 0; i < dif; i++)
                buf[i] = '0';
            key[keynum] = strtoul(buf, NULL, 16);
        }
    }
    return key;
}

void print_key(uint32_t *key, int key_len)
{
  cout << "0x";
  cout << std::hex << std::uppercase;
  for (int i = 0 ; i < key_len; i++)
    cout << setw(8) << setfill('0') <<key[i] ;
  cout << std::dec;
}

enum state {
  ENCRYPT,
  DECRYPT
};

int main(int argc, char* argv[])
{
    uint32_t *user_key = NULL;
    int user_key_len = 0;
    string in_filepath = "";
    string out_filepath = "";
    int state = ENCRYPT;

    options opt[] = {
      {'c',NO_ARGUMENT},
      {'d',NO_ARGUMENT},
      {'k',REQUIRED},
      {'f',REQUIRED},
      {'o',REQUIRED},
      {'h',NO_ARGUMENT},
      {0}
    };

    int c;
    while ((c = parse(argc,argv,opt)) != -1)
    {
      switch (c)
      {
          case 'c':
            state = ENCRYPT;
            break;

          case 'd':
            state = DECRYPT;
            break;

          case 'k':
            user_key = parse_key(cmd_arg,user_key_len);
            break;

          case 'f':
            in_filepath = cmd_arg;
            break;

          case 'o':
            out_filepath = cmd_arg;
            break;

          case '?':
            cout << "Error in input\n";
            abort();
            break;
      }
    }

    BlowfishEncrypter encrypter = BlowfishEncrypter();
    uint32_t default_key[4] = {0xffffffff, 0x012, 0x321, 0xfed2};

    if (user_key != NULL && user_key_len > 0)
    {
      cout << "Using user key : ";
      print_key(user_key, user_key_len);
      cout << endl;
      encrypter.key_expand(user_key, user_key_len);
    }
    else 
    {
      cout << "No key detected.\nUsing built in key: ";
      print_key(default_key,4);
      cout << endl;
      encrypter.key_expand(default_key, 4);
    }

    
    
    if (!in_filepath.empty())
    {
      if (state == ENCRYPT)
        encrypter.encrypt_file(in_filepath);
      else if (state == DECRYPT)
        encrypter.decrypt_file(in_filepath);
    }
    else
    {
      cout << "Error. Please set input filepath" << endl;
    }

    if (user_key != NULL)
        delete user_key;

    return 0;
}