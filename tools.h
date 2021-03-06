#ifndef tools_h
#define tools_h

#include <iomanip>
#include <limits>
#include <vector>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace rom {

//this function will tell us if a given literal type is able to hold negative values
template <class typ>
constexpr bool is_signed(void) {
return (std::numeric_limits<typ>::lowest() < typ{});
}

void assert_plain_char_is_unsigned(void) {
static_assert(is_signed<std::string::value_type>()==false,"You cannot use this software because your system iplements plain char as signed char.");
static_assert(is_signed<char>()==false,			"You cannot use this software because your system iplements plain char as signed char.");
}

//this function will tell us if char on this system is unsigned char or signed char
//unqualified int will always be signed int, but char is not exactly defined by the c++ standard
bool char_is_signed(void){return is_signed<char>();}

//get the value of one bit out of any unsigned integer
template <class uint>  //should be a unsigned integer type
uint8_t getbit(uint in,uint16_t nthbit) {
static_assert(std::is_integral<uint>::value, "getbit must be instatiated with an INTEGER type");
static_assert(is_signed<uint>()==0, "getbit can only operate in UNSIGNED integers");
uint16_t bit_size{std::numeric_limits<uint>::digits}; //will tell us how many bits out uinteger has
if (nthbit>=bit_size) {return 0;}
uint mask(uint(1) << uint16_t(nthbit));
return (mask & in)?1:0;
}

//get the value of the nth bit out of one uint64_t
inline uint8_t getbyte(uint32_t in, uint8_t nthbyte) {
if (nthbyte>=4) {return 0;}
uint32_t mask{uint32_t(255) << (nthbyte*8)};
return uint8_t((mask&in) >> (nthbyte*8));
}


}//namespace rom
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace debug {	//functions for debugging purpose

//get an binary string from any container
template <class cont> 				//any container of bool with begin() and end()
std::string to_string(const cont& _d){		//convert to binary std::string
std::string ret{};
for (auto it{_d.begin()};it!=_d.end();++it) {ret += (*it)?'1':'0';}
ret += " "+std::to_string(_d.size())+"bits";
return ret;
}

std::vector<bool> conv_bool(uint32_t in) {	//convert uint32_t to vector<bool>
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

}//namespace debug
////////////////////////////////////////////////////////////////////////////////////////////////////

//print any std::array<uint8_t,size> in hexadezimal to std::ostream;
//use this for printing results from hash functions
template <size_t array_sz>
std::ostream& operator << (std::ostream& os,const  std::array<uint8_t,array_sz>& v) {
for (auto a:v) {os << std::hex << std::setw(2) << std::setfill('0') << uint32_t(a);}
return os;
}

template <size_t array_sz>
std::string to_string(const  std::array<uint8_t,array_sz>& v) {
std::stringstream os;
for (auto a:v) {os << std::hex << std::setw(2) << std::setfill('0') << uint32_t(a);}
return os.str();
}

#endif //tools_h
