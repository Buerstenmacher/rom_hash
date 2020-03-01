#include <iostream>
#include <string>
#include <array>
#include <stdlib.h>
#include <sys/stat.h>
#include <fstream>
#include "tools.h"

constexpr uint32_t IV_A{0x67452301};
constexpr uint32_t IV_B{0xefcdab89};
constexpr uint32_t IV_C{0x98badcfe};
constexpr uint32_t IV_D{0x10325476};
constexpr uint32_t K_0 {0x00000000};
constexpr uint32_t K_1 {0x5A827999};
constexpr uint32_t K_2 {0x6ED9EBA1};
constexpr uint8_t NUM_ROUNDS 		{3};
constexpr uint8_t NUM_OPERATIONS	{16};
constexpr uint16_t BLOCKSIZE 		{512};
constexpr uint16_t DIGESTSIZE 		{128};

template <class T>	T F(T X,T Y,T Z)	{return (X&Y) | ((~X)&Z);}
template <class T>	T G(T X,T Y,T Z) 	{return (X&Y) | (X&Z) | (Y&Z);}
template <class T>	T H(T X,T Y,T Z) 	{return (X ^ Y ^ Z);}
template <class T,class U>  T RLL32(T X,U C)	{return (X<<C) | (X >> (32-C));}

uint32_t endiannessFix(uint32_t v){return (((v & 0xff) << 24) | ((v & 0xff00) << 8) | ((v & 0xff0000) >> 8) | ((v & 0xff000000) >> 24));}

struct md4Digest {
uint32_t A,B,C,D;
md4Digest(void):A{IV_A},B{IV_B},C{IV_C},D{IV_D} {}
};

int main(int argc, char *argv[]) {

if (argc < 2) {
	argc++;
	argv[1] = new char[40];
	argv[1][0] = 'a';
	argv[1][1] = 'b';
	argv[1][2] = 'c';
	argv[1][3] = '\0';
	}
std::cout << "Char is signed " << rom::is_signed<char>() << std::endl;
uint64_t filesize{}; 	//64 bit because MD4 spec
struct stat results{};	//blindly following instructions for file size
uint64_t numBlocks{};
bool paddingBlock{false};
md4Digest digest{};
std::array<uint32_t,3> key{K_0, K_1, K_2};
if (stat(argv[1], &results) == 0)	{filesize = results.st_size * 8;}
else {std::cout << "error opening file"; return 1;}
numBlocks = filesize >> 9;		//set numBlocks to the filesize divided by blocksize, and increment the block size if the remainder is enough to force an additional block
if ((filesize % 512) > 447) {   	//need to iterate through the empty block if
	paddingBlock = true;        	//there exists a padding block
	numBlocks++;
	}
std::ifstream fileToBeHashed (argv[1], std::ios::in | std::ios::binary);
bool paddedBlock	{false}; //used for program flow control
bool lastBlock		{false};
for (uint64_t i{0}; i < numBlocks+1; i++){ 	//iterate through hashing function loop
        std::array<char,64> buffer{};		//holds file input
        std::array<uint32_t,16> messageBlock{}; //blocks of message data that are used in hashing
        fileToBeHashed.seekg(i * 64);
        fileToBeHashed.read(&*buffer.begin(),64);
        if (!fileToBeHashed){ //note to self, data in buffer not in messageBlock
            buffer[fileToBeHashed.gcount()] = DIGESTSIZE;
            if(!paddingBlock){//if there isn't a padding block pad with all 0's
                for (uint16_t j = fileToBeHashed.gcount()+1; j < (512 - 64) / 8; j++)	{buffer[j] = 0;}
                lastBlock = true;
            }
            if(paddingBlock && !lastBlock){//if there is a padding block need to first determine if we're in the padding block or not and act accordingly
                if(paddedBlock) { //if we'd in the padded block it's all zero's except for the last 64 bits
                    for (uint16_t j = 0; j < (64); j++)	{buffer[j] = 0;}
                    lastBlock = true;
                }
                if(!paddedBlock){//if we're not we append a 1 then all 0's
                    buffer[fileToBeHashed.gcount()] = DIGESTSIZE; //magic constant for padding, needs fixed
                    for (uint16_t j = fileToBeHashed.gcount()+1; j < 64; j++)	{buffer[j] = 0;}
                    paddedBlock = true;
                }
            }
        }
        //64 8-bit ints in buffer; so we fix the ordering to little endian and shove them into message blocks
        for(int j = 0; j < 64; j++) { //tested, seems to work, since it operates on 64 8-bit registers (I was tired.  Please don't judge)
            messageBlock[j/4] = (messageBlock[j/4] << 8) | uint8_t(buffer[j]);  //convert 8 bit int to 32 bit int by shifting left by
        }
        if(lastBlock){
            messageBlock[15] = endiannessFix(uint32_t(((filesize) & 0xffffffff00000000) >> 32));
            messageBlock[14] = endiannessFix(uint32_t((filesize) & 0x00000000ffffffff));
        }
        md4Digest previousIter = digest; //backup the previous values of A,B,C,D by spec

	for (uint8_t i{0};i<16;++i) {
	std::cout << to_string(conv_bool(messageBlock[i])) << std::endl;
	}

        for(int j = 0; j < NUM_ROUNDS; j++) { //add j<3 to command arguments; 3 replace with NUM_ROUNDS, 16 with NUM_OPERATIONS
            int messageOrder [16];
            int rollAmount [4];
            if (j == 0) {
                int messageOrder_0 [16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15};
                int rollAmount_0 [4] = {3, 7, 11, 19};
                std::copy(messageOrder_0, messageOrder_0+16, messageOrder);
                std::copy(rollAmount_0, rollAmount_0+4, rollAmount);
                //rollAmount = rollAmount_0;
            }
            if (j == 1) {
                int messageOrder_1 [16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6 ,10, 14, 3, 7, 11, 15};
                int rollAmount_1 [4] = {3, 5, 9, 13};
                std::copy(messageOrder_1, messageOrder_1+16, messageOrder);
                std::copy(rollAmount_1, rollAmount_1+4, rollAmount);
            }
            if (j == 2) {
                int messageOrder_2 [16] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
                int rollAmount_2 [4] = {3, 9, 11, 15};
                std::copy(messageOrder_2, messageOrder_2+16, messageOrder);
                std::copy(rollAmount_2, rollAmount_2+4, rollAmount);
            }
            for(int k = 0; k < NUM_OPERATIONS; k++) { //and now, to hash
                uint32_t temp_B = 0;
                if (j == 0) {temp_B = ( (digest.A + (F(digest.B, digest.C, digest.D) + key[j] + endiannessFix(messageBlock[messageOrder[k]]))));}
                if (j == 1) {temp_B = ( (digest.A + (G(digest.B, digest.C, digest.D) + key[j] + endiannessFix(messageBlock[messageOrder[k]]))));}
                if (j == 2) {temp_B = ( (digest.A + (H(digest.B, digest.C, digest.D) + key[j] + endiannessFix(messageBlock[messageOrder[k]]))));}
                temp_B = RLL32(temp_B, rollAmount[(k % 4)]);
                digest.A = digest.D;
                digest.D = digest.C;
                digest.C = digest.B;
                digest.B = temp_B;
            }
        }
        digest.A = previousIter.A + digest.A;
        digest.B = previousIter.B + digest.B;
        digest.C = previousIter.C + digest.C;
        digest.D = previousIter.D + digest.D;
    }
    fileToBeHashed.close();
    std::cout << digest.A << " ";
    std::cout << digest.B << " ";
    std::cout << digest.C << " ";
    std::cout << digest.D << std::endl;
    std::cout << decToHex(endiannessFix(digest.A)) << decToHex(endiannessFix(digest.B)) << decToHex(endiannessFix(digest.C)) << decToHex(endiannessFix(digest.D))<< std::endl;
    return 0;
}
