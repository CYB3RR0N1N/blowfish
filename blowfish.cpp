#include "blowfish.hpp"


void BlowfishEncrypter::encrypt_block(uint32_t &left, uint32_t &right)
{
    for (int i = 0 ; i < 16 ; i++)
    {
        left ^= key[i];
        right = F(left) ^ right;
        std::swap(left,right);
    }
    std::swap(left,right);

    left ^= key[17];
    right ^= key[16];
}
void BlowfishEncrypter::decrypt_block(uint32_t &left, uint32_t &right)
{

    left ^=  key[17];
    right ^= key[16];


    for (int i = 15 ; i >= 0 ; i--)
    {
        right = F(left) ^ right;
        left ^= key[i];
        std::swap(left,right);
    }

    std::swap(left,right);
}
void BlowfishEncrypter::key_expand(uint32_t *key, int len)
{
    for (int i = 0 ; i < 18 ; i++)
    {
        this->key[i] = key[i % len] ^ INIT_P[i]; //Copy and concatenate initial key
    }   

    uint32_t l = 0,r = 0;

    for(int i = 0; i < 18; i++)
    {
        encrypt_block(l, r);
        this->key[i] = l;
        this->key[++i] = r;
    }

    for(int i = 0 ; i < 4 ; i++)
    {
        for(int j = 0 ; j < 256; j++)
        {
            encrypt_block(l, r);
            this->sbox[i][j] = l;
            this->sbox[i][++j] = r;
        }
    }
}
uint32_t BlowfishEncrypter::F(uint32_t x)
{
    uint32_t res = ((sbox[0][(x >> 24) & 0xFF] + sbox[1][(x >> 16) & 0xFF]) ^ sbox[2][(x >> 8) & 0xFF]) + sbox[3][(x) & 0xFF];
    return res;
}


typedef union block
{
    uint32_t dword;
    uint8_t byte[4];
} block;

void BlowfishEncrypter::encrypt_file(std::string filepath)
{
    std::filesystem::path path = filepath;

    //Determine file size
    std::uintmax_t size = std::filesystem::file_size(filepath);
    if (size % 4 != 0)
        size = size / 4 + 1;
    int buffer_size = size / 4;
    //Prepare input buffer
    block *buffer = new block[buffer_size];
    //Read file
    std::ifstream fstream;
    fstream.open(path, std::ios::binary | std::ios::in);
    fstream.read((char *)buffer->byte,size);
    //Encrypt file
    for (int i = 0; i < buffer_size; i+=2)
    {
        encrypt_block(buffer[i].dword,buffer[i+1].dword);
    }
    //Write crypted
    std::string out_filename = path.filename();
    out_filename = "crypted_" + out_filename;
    path.replace_filename(out_filename);
    path.replace_extension(".dat");
    std::ofstream ofstream;
    ofstream.open(path, std::ios::binary | std::ios::out);
    ofstream.write((char *)buffer->byte, size);

    delete buffer;
}

void BlowfishEncrypter::decrypt_file(std::string filepath)
{
    std::filesystem::path path = filepath;
    //Determine file size
    std::uintmax_t size = std::filesystem::file_size(filepath);
    if (size % 4 != 0)
        size = size / 4 + 1;
    int buffer_size = size / 4;
    //Prepare input buffer
    block *buffer = new block[buffer_size];
    //Read file
    std::ifstream fstream;
    fstream.open(path, std::ios::binary | std::ios::in);
    fstream.read((char *)buffer->byte,size);
    //Encrypt file
    for (int i = 0; i < buffer_size; i+=2)
    {
        decrypt_block(buffer[i].dword,buffer[i+1].dword);
    }
    //Write crypted
    std::string out_filename = path.filename();
    out_filename = "decrypted_" + out_filename;
    path.replace_filename(out_filename);
    path.replace_extension(".dat");
    std::ofstream ofstream;
    ofstream.open(path, std::ios::binary | std::ios::out);
    ofstream.write((char *)buffer->byte, size);

    delete buffer;
}