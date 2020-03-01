#include "md2.h"
#include "md4.h"
#include "tools.h"
#include <iostream>

template <class hash_fun>
void test(std::string in, std::string known_result="", hash_fun hf=rom::md2()) {
std::cout << in << std::endl;
std::cout << "should be:   \t" << known_result << std::endl;
std::cout << "calculation: \t" << hf(in) << std::endl;
}

int main(void) {


//test("Test","1178f3ddf625018179a93c1a0298dfa9",rom::md2());
//test("","8350e5a3e24c153df2275c9f80692773",rom::md2());

std::string a{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
//a+=char(10);

std::cout <<"md4: " << rom::md4{}(a) << std::endl;
std::cout <<"md4: " << rom::md4{}(a) << std::endl;
std::cout <<"md2: " << rom::md2{}(a) << std::endl;
std::cout <<"md2: " << rom::md2{}(a) << std::endl;

}
