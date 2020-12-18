#include "blowfish.hpp"

void BlowfishEncrypter::encrypt_block(uint32_t &left, uint32_t &right)
{
    for (int i = 0 ; i < 16 ; i++)
    {
        left ^= key[i];
        right ^= F(left);
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

    for(int i = 0; i < 18; i++)  // Ошибка тут , каждый раз при запуске получаются разные клчи
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
} block ;

void BlowfishEncrypter::encrypt_file(std::string in_filepath, std::string out_filepath)
{
    std::cout << "Ecrypting file " << in_filepath << std::endl;
    //Determine file size

#ifdef __linux__
    std::uintmax_t size = std::filesystem::file_size(in_filepath);
#elif _WIN32
	struct stat stat_buf;
	int rc = stat(in_filepath.c_str(), &stat_buf);
	int size = stat_buf.st_size;
#endif
    uint8_t zero_cnt = 0;
    zero_cnt = (8 - size % 8) % 8;
    size += zero_cnt;
        
    int buffer_size = size / 4;
    //Prepare input buffer
    block *buffer = new block[buffer_size];
    for (int i = 0 ; i < buffer_size; i++)
        buffer[i].dword = 0;
    //Read file
    std::ifstream fstream;
    fstream.open(in_filepath, std::ios::binary | std::ios::in);
    fstream.read((char *)buffer->byte,size);
    fstream.close();
    //Encrypt file
    for (int i = 0; i < buffer_size; i+=2)
    {
        encrypt_block(buffer[i].dword,buffer[i+1].dword);
    }
    //Write crypted
    std::ofstream ofstream;
    std::filesystem::path path = in_filepath;
    std::string old_ext = path.extension().string();
    path.replace_extension("dat");
    uint8_t ext_size = (uint8_t)old_ext.size();
    ofstream.open(path, std::ios::binary | std::ios::out);
    ofstream.write((char *)&zero_cnt,1);
    ofstream.write((char *)&ext_size,1);
    ofstream.write(old_ext.c_str(),ext_size);
    ofstream.write((char *)buffer->byte, size);
    ofstream.close();
    delete buffer;
    std::cout << "Eccryption complete\nOutput in file " << path.filename().string() << std::endl;
}

void BlowfishEncrypter::decrypt_file(std::string in_filepath, std::string out_filepath)
{
    std::cout << "Decrypting file " << in_filepath << std::endl;
    //Determine file size
	std::ifstream fstream;
	fstream.open(in_filepath, std::ios::binary | std::ios::in);
#ifdef __linux__
    std::uintmax_t size = std::filesystem::file_size(in_filepath);
#elif _WIN32
	struct stat stat_buf;
	int rc = stat(in_filepath.c_str(), &stat_buf);
	int size = stat_buf.st_size;
#endif
    uint8_t zeros_size = 0;
    uint8_t ext_size;
    fstream.read((char *)&zeros_size,1);
    fstream.read((char *)&ext_size, 1);
    char *ext = new char[ext_size + 1];
    fstream.read(ext,ext_size);
	ext[ext_size] = 0;

    size -= 2 + ext_size;
    if (size % 8 != 0)
    {
        std::cout << "Error in file\n";
        return;
    }

    int buffer_size = size / 4;
    //Prepare input buffer
    block *buffer = new block[buffer_size];
    for (int i = 0 ; i < buffer_size; i++)
        buffer[i].dword = 0;
    //Read data
    fstream.read((char *)buffer->byte,size);
    fstream.close();
    //Encrypt file
    for (int i = 0; i < buffer_size; i+=2)
    {
        decrypt_block(buffer[i].dword,buffer[i+1].dword);
    }
    std::filesystem::path path = in_filepath;
    path.replace_filename("decrypted");
    path.replace_extension(ext);
    //Write crypted
    std::ofstream ofstream;
    ofstream.open(path, std::ios::binary | std::ios::out);
    ofstream.write((char *)buffer->byte, size - zeros_size);
    ofstream.close();
    delete ext;
    delete buffer;
    std::cout << "Decryption complete\nOutput in file " << path.string() << std::endl;
}

BlowfishEncrypter::BlowfishEncrypter()
{
    for (int i = 0 ; i < 18 ; i++)
        key[i] = 0;

    for (int i = 0 ; i < 4; i++)
        for (int j = 0; j < 256; j++)
            sbox[i][j] = INIT_SBOX[i][j];
}