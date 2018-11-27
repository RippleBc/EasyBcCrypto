#include "./common.h"

/* origin */
unsigned char source[16];

int main()
{
    source[0] = 0x6d; 
    source[1] = 0x69; 
    source[2] = 0x64; 
    source[3] = 0x6e; 
    source[4] = 0x69; 
    source[5] = 0x67; 
    source[6] = 0x68; 
    source[7] = 0x74;
    source[8] = 0x6d; 
    source[9] = 0x69; 
    source[10] = 0x64; 
    source[11] = 0x6e; 
    source[12] = 0x69; 
    source[13] = 0x67; 
    source[14] = 0x68; 
    source[15] = 0x74;

    aes_encrypt(source);
    aes_decrypt(source);
}