//this File is a c++ fork of
//code written by Marek Ulwański

#ifndef md5_h
#define md5_h

#include <cstring>
#include <type_traits>
#include <vector>
#include <array>
#include <algorithm>

namespace rom {

using u64= uint64_t;
using u32= uint32_t;
using u8 = uint8_t;

class md5 {
private:
static u32 F(u32 x,u32 y,u32 z) {return z ^ (x & (y^z));}
static u32 G(u32 x,u32 y,u32 z) {return y ^ (z & (x^y));}
static u32 H(u32 x,u32 y,u32 z) {return x ^ y ^ z;	}
static u32 I(u32 x,u32 y,u32 z) {return y ^ (x | (~z));}

template <class fun>
void STEP(fun f,u32& a,u32 b,u32 c,u32 d,u32 x,u32 t,u8 s) {
a += f(b,c,d) + x + t;
a = (a<<s) | ((a & 0xffffffff) >> (32-s));	//rotate shift
a += b;
}

struct MD5_CTX{
u32 lo,hi;
u32 a,b,c,d;
std::array<u8,64> buffer;
std::array<u32,16> block;
MD5_CTX(void):lo{0},hi{0},a{0x67452301},b{0xefcdab89},c{0x98badcfe},d{0x10325476},buffer{},block{} {}
};

template <class iter>	//some pointer or iterator to u8
u32 SET(iter ptr,MD5_CTX& ctx,u32 n) {
u64 m{n*4};
return ctx.block.at(n) = *(ptr+m) | (*(ptr+m+1) << 8) | (*(ptr+m+2) << 16) | (*(ptr+m+3) << 24);
}


u32 GET(const MD5_CTX& ctx,u32 n) {return ctx.block.at(n);}

template <class iter>	//some pointer or iterator to u8
iter body(MD5_CTX& ctx,iter ptr, u64 size){
u32 a = {ctx.a};
u32 b = {ctx.b};
u32 c = {ctx.c};
u32 d = {ctx.d};
do {
	u32 saved_a {a};
	u32 saved_b {b};
	u32 saved_c {c};
	u32 saved_d {d};
	STEP(F, a, b, c, d, SET(ptr,ctx,0),	0xd76aa478, 7);
	STEP(F, d, a, b, c, SET(ptr,ctx,1),	0xe8c7b756, 12);
	STEP(F, c, d, a, b, SET(ptr,ctx,2),	0x242070db, 17);
	STEP(F, b, c, d, a, SET(ptr,ctx,3),	0xc1bdceee, 22);
	STEP(F, a, b, c, d, SET(ptr,ctx,4),	0xf57c0faf, 7);
	STEP(F, d, a, b, c, SET(ptr,ctx,5),	0x4787c62a, 12);
	STEP(F, c, d, a, b, SET(ptr,ctx,6),	0xa8304613, 17);
	STEP(F, b, c, d, a, SET(ptr,ctx,7),	0xfd469501, 22);
	STEP(F, a, b, c, d, SET(ptr,ctx,8),	0x698098d8, 7);
	STEP(F, d, a, b, c, SET(ptr,ctx,9),	0x8b44f7af, 12);
	STEP(F, c, d, a, b, SET(ptr,ctx,10),	0xffff5bb1, 17);
	STEP(F, b, c, d, a, SET(ptr,ctx,11),	0x895cd7be, 22);
	STEP(F, a, b, c, d, SET(ptr,ctx,12),	0x6b901122, 7);
	STEP(F, d, a, b, c, SET(ptr,ctx,13),	0xfd987193, 12);
	STEP(F, c, d, a, b, SET(ptr,ctx,14),	0xa679438e, 17);
	STEP(F, b, c, d, a, SET(ptr,ctx,15),	0x49b40821, 22);
	STEP(G, a, b, c, d, GET(ctx,1),		0xf61e2562, 5);
	STEP(G, d, a, b, c, GET(ctx,6),		0xc040b340, 9);
	STEP(G, c, d, a, b, GET(ctx,11),	0x265e5a51, 14);
	STEP(G, b, c, d, a, GET(ctx,0),		0xe9b6c7aa, 20);
	STEP(G, a, b, c, d, GET(ctx,5),		0xd62f105d, 5);
	STEP(G, d, a, b, c, GET(ctx,10),	0x02441453, 9);
	STEP(G, c, d, a, b, GET(ctx,15),	0xd8a1e681, 14);
	STEP(G, b, c, d, a, GET(ctx,4),		0xe7d3fbc8, 20);
	STEP(G, a, b, c, d, GET(ctx,9),		0x21e1cde6, 5);
	STEP(G, d, a, b, c, GET(ctx,14),	0xc33707d6, 9);
	STEP(G, c, d, a, b, GET(ctx,3),		0xf4d50d87, 14);
	STEP(G, b, c, d, a, GET(ctx,8),		0x455a14ed, 20);
	STEP(G, a, b, c, d, GET(ctx,13),	0xa9e3e905, 5);
	STEP(G, d, a, b, c, GET(ctx,2),		0xfcefa3f8, 9);
	STEP(G, c, d, a, b, GET(ctx,7),		0x676f02d9, 14);
	STEP(G, b, c, d, a, GET(ctx,12),	0x8d2a4c8a, 20);
	STEP(H, a, b, c, d, GET(ctx,5),		0xfffa3942, 4);
	STEP(H, d, a, b, c, GET(ctx,8),		0x8771f681, 11);
	STEP(H, c, d, a, b, GET(ctx,11),	0x6d9d6122, 16);
	STEP(H, b, c, d, a, GET(ctx,14),	0xfde5380c, 23);
	STEP(H, a, b, c, d, GET(ctx,1),		0xa4beea44, 4);
	STEP(H, d, a, b, c, GET(ctx,4),		0x4bdecfa9, 11);
	STEP(H, c, d, a, b, GET(ctx,7),		0xf6bb4b60, 16);
	STEP(H, b, c, d, a, GET(ctx,10),	0xbebfbc70, 23);
	STEP(H, a, b, c, d, GET(ctx,13),	0x289b7ec6, 4);
	STEP(H, d, a, b, c, GET(ctx,0),		0xeaa127fa, 11);
	STEP(H, c, d, a, b, GET(ctx,3),		0xd4ef3085, 16);
	STEP(H, b, c, d, a, GET(ctx,6),		0x04881d05, 23);
	STEP(H, a, b, c, d, GET(ctx,9),		0xd9d4d039, 4);
	STEP(H, d, a, b, c, GET(ctx,12),	0xe6db99e5, 11);
	STEP(H, c, d, a, b, GET(ctx,15),	0x1fa27cf8, 16);
	STEP(H, b, c, d, a, GET(ctx,2),		0xc4ac5665, 23);
	STEP(I, a, b, c, d, GET(ctx,0),		0xf4292244, 6);
	STEP(I, d, a, b, c, GET(ctx,7),		0x432aff97, 10);
	STEP(I, c, d, a, b, GET(ctx,14),	0xab9423a7, 15);
	STEP(I, b, c, d, a, GET(ctx,5),		0xfc93a039, 21);
	STEP(I, a, b, c, d, GET(ctx,12),	0x655b59c3, 6);
	STEP(I, d, a, b, c, GET(ctx,3),		0x8f0ccc92, 10);
	STEP(I, c, d, a, b, GET(ctx,10),	0xffeff47d, 15);
	STEP(I, b, c, d, a, GET(ctx,1),		0x85845dd1, 21);
	STEP(I, a, b, c, d, GET(ctx,8),		0x6fa87e4f, 6);
	STEP(I, d, a, b, c, GET(ctx,15),	0xfe2ce6e0, 10);
	STEP(I, c, d, a, b, GET(ctx,6),		0xa3014314, 15);
	STEP(I, b, c, d, a, GET(ctx,13),	0x4e0811a1, 21);
	STEP(I, a, b, c, d, GET(ctx,4), 	0xf7537e82, 6);
	STEP(I, d, a, b, c, GET(ctx,11), 	0xbd3af235, 10);
	STEP(I, c, d, a, b, GET(ctx,2), 	0x2ad7d2bb, 15);
	STEP(I, b, c, d, a, GET(ctx,9), 	0xeb86d391, 21);
	a += saved_a;
	b += saved_b;
	c += saved_c;
	d += saved_d;
	ptr += 64;
	} while (size -= 64);
ctx.a = a;
ctx.b = b;
ctx.c = c;
ctx.d = d;
return ptr;
}

void MD5_Update(MD5_CTX& ctx,const std::vector<u8>& inp){
auto it {inp.begin()};
auto size{inp.size()};
auto lo_tmp{ctx.lo};
ctx.lo = (lo_tmp + size) & 0x1fffffff;
if (ctx.lo < lo_tmp)	{ctx.hi++;}
ctx.hi += size >> 29;
u64 used{lo_tmp & u32(63)};
if (used){
	u64 free{64-used};
	if (size < free) {
		std::copy_n(inp.begin(),inp.size(),ctx.buffer.begin()+used);
		return;
		}
	std::copy_n(inp.begin(),free,ctx.buffer.begin()+used);
	it   += free;
	size -= free;
	body(ctx,ctx.buffer.begin(),64);
	}
if (size >= 64) {
	it = body(ctx, it, size & ~u64(63));
	size &= u64(63);
	}
std::copy_n(it,size,ctx.buffer.begin());
}

void MD5_Final(std::array<u8,16>& result, MD5_CTX& ctx){
u64 used {ctx.lo & 0x3f};
u64 free {64 - used};
ctx.buffer.at(used++) = 0x80;
if (free < 8) {
	std::fill_n(ctx.buffer.begin()+used,free,0);
	body(ctx,ctx.buffer.begin(),64);
	used = 0;
	free = 64;
	}
std::fill_n(ctx.buffer.begin()+used,free-8,0);
ctx.lo <<= 3;
ctx.buffer.at(56) = ctx.lo;
ctx.buffer.at(57) = ctx.lo >> 8;
ctx.buffer.at(58) = ctx.lo >> 16;
ctx.buffer.at(59) = ctx.lo >> 24;
ctx.buffer.at(60) = ctx.hi;
ctx.buffer.at(61) = ctx.hi >> 8;
ctx.buffer.at(62) = ctx.hi >> 16;
ctx.buffer.at(63) = ctx.hi >> 24;
body(ctx,ctx.buffer.begin(),64);
result.at(0) = ctx.a;
result.at(1) = ctx.a >> 8;
result.at(2) = ctx.a >> 16;
result.at(3) = ctx.a >> 24;
result.at(4) = ctx.b;
result.at(5) = ctx.b >> 8;
result.at(6) = ctx.b >> 16;
result.at(7) = ctx.b >> 24;
result.at(8) = ctx.c;
result.at(9) = ctx.c >> 8;
result.at(10) = ctx.c >> 16;
result.at(11) = ctx.c >> 24;
result.at(12) = ctx.d;
result.at(13) = ctx.d >> 8;
result.at(14) = ctx.d >> 16;
result.at(15) = ctx.d >> 24;
}

/* Return Calculated raw result(always little-endian), the size is always 16 */
void md5bin(const std::vector<u8>& dat,std::array<u8,16>& out) {
MD5_CTX c;
MD5_Update(c,dat);
MD5_Final(out, c);
}

public:
std::array<uint8_t,16> operator()(const std::vector<u8>& dat){
std::array<uint8_t,16> res;
md5bin(dat,res);
return res;
}

std::array<uint8_t,16> operator()(const std::string& dat){
rom::assert_plain_char_is_unsigned();   //make sure char runs from 0 to 255
std::vector<u8> datu;
for (auto c:dat) {datu.push_back(u8(c));}
return this->operator()(datu);
}

};//class md5
////////////////////////////////////////////////////////////////////////////////////////////////////

}//namespace rom

#endif //md5_h

//this File is a c++ fork of
//code written by Marek Ulwański

