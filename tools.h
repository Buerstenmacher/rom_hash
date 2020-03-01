#ifndef tools_h
#define tools_h

#include <iomanip>
#include <limits>
#include <vector>

namespace rom {
//todo: make one template function out of 4 overloadet getbit()
//std::numeric_limits<T>::digits  will tell us how many bits out uinteger has

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t getbit(uint64_t in, uint8_t nthbit) {//get the value of the nth bit out of one uint64_t
if (nthbit>=64) {return 0;}
uint64_t mask{uint64_t(1) << nthbit};
return (mask & in)?1:0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t getbit(uint32_t in, uint8_t nthbit) {//get the value of the nth bit out of one uint64_t
if (nthbit>=32) {return 0;}
uint32_t mask{uint32_t(1) << nthbit};
return (mask&in)?1:0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t getbit(uint8_t in, uint8_t nthbit) {//get the value of the nth bit out of one uint8_t
if (nthbit>=8) {return 0;}
uint8_t mask(uint8_t{1} << nthbit);
return (mask&in)?1:0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t getbyte(uint32_t in, uint8_t nthbyte) {//get the value of the nth bit out of one uint64_t
if (nthbyte>=4) {return 0;}
uint32_t mask{uint32_t(255) << (nthbyte*8)};
return uint8_t((mask&in) >> (nthbyte*8));
}

//this function will tell us if a given literal type is able to hold negative values
template <class typ>
bool is_signed(void) {
return (std::numeric_limits<typ>::lowest() < typ{});
}

//this function will tell us if char on this system is unsigned char or signed char
//unqualified int will always be signed int, but char is not exactly defined by the c++ standard
bool char_is_signed(void){return is_signed<char>();}

}//namespace rom

//print any std::array<uint8_t,size> in hexadezimal to std::ostream;
//use this for results from hash functions
template <size_t array_sz>
std::ostream& operator << (std::ostream& os,const  std::array<uint8_t,array_sz>& v) {
for (auto a:v) {os << std::hex << std::setw(2) << std::setfill('0') << uint32_t(a);}
return os;
}

//get an binary string from any container
template <class cont> 				//any container of bool with begin() and end()
std::string to_string(const cont& _d){		//convert to binary std::string
std::string ret{};
for (auto it{_d.begin()};it!=_d.end();++it) {ret += (*it)?'1':'0';}
ret += " "+std::to_string(_d.size())+"bits";
return ret;
}

std::vector<bool> conv_bool(uint32_t in) {
std::vector<bool> ret{};
for (int8_t i{31};i>=0;--i) {ret.push_back(rom::getbit(in,i));}
return ret;
}

std::string decToHex(uint32_t value){
std::string hex{},endian_fix{};
while(value != 0){
        if ((value % 16) < 10) {hex += (value % 16) + 48;}
        if ((value % 16) > 9 ) {hex += (value % 16) + 55;}
        value >>=  4;
        }
for(uint32_t i{1};i<=hex.length();++i)  {endian_fix += hex[hex.length() - i];}
return endian_fix;
}


#endif //tools_h
