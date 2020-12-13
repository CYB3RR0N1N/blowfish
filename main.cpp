#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
#include <getopt.h>
#include "blowfish.hpp"
#include "simple_parser.hpp"

using namespace std;

uint32_t *parse_key(char *key_str,int &key_len)
{
    int str_len = strlen(key_str);
    uint32_t *key;
    key_len = str_len / 8 + 1;


    key = new uint32_t[2];
    key[0] = 0xFF;
    key[1] = 0xBB;
    key_len = 2;
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
    string filepath = "";
    int state = ENCRYPT;

    options opt[] = {
      {'c',NO_ARGUMENT},
      {'d',NO_ARGUMENT},
      {'k',REQUIRED},
      {'f',REQUIRED},
      {'h',NO_ARGUMENT},
    };

    int c;
    while ((c = parse(argc,argv,opt)) != -1)
    {
      switch (c)
      {
      case 'c':
        cout << "kek c\n";
        break;
      case 'd':
        cout << "kek d\n";
        break;
      case 'k':
        cout << "kek k \n";
        break;
      case 'f':
        cout << cmd_arg << "\n";
        break;
      case '?':
        cout << "Read manual\n";
        break;
      }
    }

    BlowfishEncrypter encrypter = BlowfishEncrypter();
    uint32_t default_key[4] = {0xffffffff, 0x012, 0x321, 0xfed2};

    if (user_key != NULL && user_key_len > 0)
    {
      cout << " Using user key : ";
      print_key(user_key, user_key_len);
      cout << endl;
      encrypter.key_expand(user_key, user_key_len);
    }
    else 
    {
      cout << "No key detected . Using built in key: ";
      print_key(default_key,4);
      cout << endl;
      encrypter.key_expand(default_key, 4);
    }

    
    
    if (!filepath.empty())
    {
      if (state == ENCRYPT)
        encrypter.encrypt_file(filepath);
      else if (state == DECRYPT)
        encrypter.decrypt_file(filepath);
    }
    else
    {
      cout << "Error please set filepath" << endl;
    }

    if (user_key != NULL)
        delete user_key;

    return 0;
}