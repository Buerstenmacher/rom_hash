#include "md2.h"
#include "md4.h"
#include "tools.h"
#include <iostream>

int main(void) {
std::string a{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
//a+=char(10);

std::cout <<"md4: " << rom::md4{}(a) << std::endl;
std::cout <<"md4: " << rom::md4{}(a) << std::endl;
std::cout <<"md2: " << rom::md2{}(a) << std::endl;
std::cout <<"md2: " << rom::md2{}(a) << std::endl;
}	//main
