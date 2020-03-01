#ifndef md4_h
#define md4_h

#include <vector>
#include "tools.h"

namespace rom {

using u8 = uint8_t;
using u32 =uint32_t;

inline u32 r_left(u32 in,u8 left) {
left %= 32;
return (in<<left) | (in>>(32-left));
}

u32 fuckendian(u32 v){return ((v&0xff)<<24 | (v&0xff00)<<8 | (v&0xff0000)>>8 | (v&0xff000000) >> 24);}

////////////////////////////////////////////////////////////////////////////////////////////////////
class md4 {
static constexpr u32 W_A{0x67452301};	//this magic numbers are defined by ronald rivest in 1990
static constexpr u32 W_B{0xefcdab89};
static constexpr u32 W_C{0x98badcfe};
static constexpr u32 W_D{0x10325476};

	struct md4Digest {
	u32 A,B,C,D;
	md4Digest(void):A{W_A},B{W_B},C{W_C},D{W_D} {}
	u32& at(u8 pos) {
		pos %= 4;
		if (pos==0) {return A;}
		if (pos==1) {return B;}
		if (pos==2) {return C;}
		else /*pos==3*/ {return D;}
		}
	};

static constexpr u32 R1C{0x00000000};
static constexpr u32 R2C{0x5a827999};
static constexpr u32 R3C{0x6ed9eba1};

static constexpr u8 N_ROUNDS            {3};
static constexpr u8 N_OPERATIONS        {16};

static u32 F(u32 X,u32 Y,u32 Z)	{return (X&Y) | ((~X)&Z);}
static u32 G(u32 X,u32 Y,u32 Z)	{return (X&Y) | (X&Z) | (Y&Z);}
static u32 H(u32 X,u32 Y,u32 Z)	{return (X ^ Y ^ Z);}

std::vector<bool> padding(const std::vector<bool>& inp) {
auto in{inp};
uint64_t sz_bef_pad{in.size()};
in.push_back(true);	//padding is always performed; a single "1" bit is appended
//and then enough zero bits are appendet so that length becomes congruent to 448 mod 512
while ((in.size() % 512) != 448) 	{in.push_back(false);}
for (int8_t byte{0};byte!=8;++byte) {	//add filesize in bits as 64 bits at the end
	for (int8_t bit{7};bit>=0;--bit) {in.push_back(getbit(sz_bef_pad,byte*8+bit));}
	}
return in;	//return std::vector<bool> size() mod 512 == 0
}

std::array<u8,16> hashing(const std::vector<bool>& in) {
md4Digest dig{},prev{};
constexpr std::array<u32,3> key{R1C,R2C,R3C};
constexpr std::array<std::array<u8,4>,3> rollAmount    {{ 	{3,7,11,19}, {3,5,9,13}, {3,9,11,15} }};
constexpr std::array<std::array<u8,16>,3> messageOrder {{	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
								{0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15},
								{0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15} }};
for(size_t blocknr{0};blocknr!=in.size()/512;blocknr++) {
	std::array<u32,16> mblock{}; //blocks of message data that are used in hashing
	for(int16_t j{0}; j<512; j++) {mblock.at(j/32) = (mblock.at(j/32) << 1) | u8(in.at(j+512*blocknr));}
//	for (u8 i{0};i<16;++i) {std::cout << to_string(conv_bool(mblock.at(i))) << std::endl;}
	prev = dig;
	for(u8 j{0}; j!=N_ROUNDS;++j) {
		for(u8 k{0}; k!=N_OPERATIONS;++k) { //and now, to hash
			u32 temp_B{0};
			if (j==0) {temp_B = dig.A + F(dig.B, dig.C, dig.D) + key.at(j) + fuckendian(mblock.at(messageOrder.at(j).at(k) ));}
			if (j==1) {temp_B = dig.A + G(dig.B, dig.C, dig.D) + key.at(j) + fuckendian(mblock.at(messageOrder.at(j).at(k) ));}
			if (j==2) {temp_B = dig.A + H(dig.B, dig.C, dig.D) + key.at(j) + fuckendian(mblock.at(messageOrder.at(j).at(k) ));}
			temp_B = r_left(temp_B, rollAmount.at(j).at(k%4) );
			dig.A = dig.D;
			dig.D = dig.C;
			dig.C = dig.B;
			dig.B = temp_B;
			}
        	}
	for (u8 t{0}; t!=4; ++t) {dig.at(t) = prev.at(t) + dig.at(t);}
 	}
std::array<u8,16> ret{};
for (u8 t{0}; t!=4; ++t) {for (u8 by{0};by!=4;by++) {ret.at(by+4*t) =  getbyte(dig.at(t),by);}}
return ret;
}

public:
md4(void) {}

std::array<u8,16> operator()(const std::string& in) {
std::vector<bool> tmp{};	//convert to std::vector<bool> and run calculation
for (auto ch:in) {for (int8_t bit{7};bit>=0;--bit) {tmp.push_back(getbit(u8(ch),bit));}}
return this->operator()(tmp);
}

std::array<u8,16> operator()(const std::vector<bool>& in) {
auto copy{padding(in)};
return hashing(copy);
}

}; //class md4
////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace rom
#endif //md4_h
