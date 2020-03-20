#ifndef md2_h
#define md2_h

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <array>
#include "tools.h"

namespace rom {

class md2 {
	struct MDCTX {
	std::array <uint8_t,48>  D;	//buffer for forming digest in; At the end, D.at(0...15) form the message digest
	std::array <uint8_t,16>  C;	//checksum register
	uint8_t  i,L;	//number of bytes handled, modulo 16 //L: last checksum char saved
	MDCTX(void):D{},C{},i{0},L{0} {for (uint8_t j{0};j!=16;++j) {D.at(j) = C.at(j) = 0;}}
	} mdctx;

static const std::array<uint8_t,256> S;

void MDUPDATE(uint8_t c) {
uint8_t i{mdctx.i}, t{0};		//Add new character to buffer
mdctx.D.at(16+i) = c;
mdctx.D.at(32+i) = c ^ mdctx.D.at(i);
mdctx.L = (mdctx.C.at(i) ^= S.at(0xFF & (c ^ mdctx.L)));	// Update checksum register C and value L
i = mdctx.i = (i + 1) & 15;	// Increment mdctx->i by one modulo 16
if (i) {return;}		// Transform D if i=0
for (uint8_t j{0}; j!=18; t+=j++) {for (i=0;i!=48;++i) {t = mdctx.D.at(i) ^= S.at(t);}}
}

void MDFINAL() {
auto padlen {uint8_t(16 - mdctx.i)};	//pad out to multiple of 16 //lets hope we do not get a negative number
for (uint8_t i{0};i<padlen;++i) MDUPDATE(padlen);
for (uint8_t i{0};i<16;++i) 	MDUPDATE(mdctx.C.at(i));
}

public:
md2(void):mdctx{} {}

std::array<uint8_t,16> operator()(const std::string& strin) {
rom::assert_plain_char_is_unsigned();   //make sure char runs from 0 to 255
mdctx = MDCTX{};	//reinitialize mdctx to initial state
for (auto ch:strin) {MDUPDATE(ch);}
MDFINAL();
std::array<uint8_t,16> ret{};
for (uint8_t i{0};i!=16;++i) {ret.at(i)=(mdctx.D.at(i));}
return std::move(ret);
}

};	//class md2

const std::array<uint8_t,256> md2::S {
	 41, 46, 67,201,162,216,124,  1, 61, 54, 84,161,236,240,  6, 19, 98,167,  5,243,192,199,115,140,152,147, 43,217,188, 76,130,202,
	 30,155, 87, 60,253,212,224, 22,103, 66,111, 24,138, 23,229, 18,190, 78,196,214,218,158,222, 73,160,251,245,142,187, 47,238,122,
	169,104,121,145, 21,178,  7, 63,148,194, 16,137, 11, 34, 95, 33,128,127, 93,154, 90,144, 50, 39, 53, 62,204,231,191,247,151,  3,
	255, 25, 48,179, 72,165,181,209,215, 94,146, 42,172, 86,170,198, 79,184, 56,210,150,164,125,182,118,252,107,226,156,116,  4,241,
	 69,157,112, 89,100,113,135, 32,134, 91,207,101,230, 45,168,  2, 27, 96, 37,173,174,176,185,246, 28, 70, 97,105, 52, 64,126, 15,
	 85, 71,163, 35,221, 81,175, 58,195, 92,249,206,186,197,234, 38, 44, 83, 13,110,133, 40,132,  9,211,223,205,244, 65,129, 77, 82,
	106,220, 55,200,108,193,171,250, 36,225,123,  8, 12,189,177, 74,120,136,149,139,227, 99,232,109,233,203,213,254, 59,  0, 29, 57,
	242,239,183, 14,102, 88,208,228,166,119,114,248,235,117, 75, 10, 49, 68, 80,180,143,237, 31, 26,219,153,141, 51,159, 17,131, 20	};

}	//namespace rom

#endif //md2_h
