#include "md2.h"
#include "md4.h"
#include "md5.h"
#include "tools.h"
#include <iostream>

template <class hashfun>  //
bool hash_test(hashfun hf,const std::string& inp,const  std::string& known_result) {
static uint64_t n_ok{0};
auto calc {hf(inp)};
std::string calcstr{to_string(calc)};
if (known_result == calcstr) {
	std::cout << ++n_ok << " ok" << std::endl;
	return true;
	}
else 	{
	std::cout << calcstr	 << std::endl << known_result << std::endl;
	std::cout << "Wrong Result !!" << std::endl;
	}
return false;
}


int main(void) {

hash_test(rom::md2{},"","8350e5a3e24c153df2275c9f80692773");
hash_test(rom::md2{},"Test","1178f3ddf625018179a93c1a0298dfa9");
hash_test(rom::md2{},"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789","da33def2a42df13975352846c30338cd");

hash_test(rom::md4{},"","31d6cfe0d16ae931b73c59d7e0c089c0");
hash_test(rom::md4{},"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789","043f8582f241db351ce627e153e7f0e4");
hash_test(rom::md4{},"Test","5d9381b52cd41a87bc8fcc7726aec03a");
hash_test(rom::md4{},"a","bde52cb31de33e46245e05fbdbd6fb24");
hash_test(rom::md4{},"md4","4aa1e608aeebf95116f5ea35851a3b55");
hash_test(rom::md4{},"abc","a448017aaf21d8525fc10ae87aa6729d");
hash_test(rom::md4{},"message digest","d9130a8164549fe818874806e1c7014b");
hash_test(rom::md4{},"abcdefghijklmnopqrstuvwxyz","d79e1c308aa5bbcdeea8ed63df412da9");
hash_test(rom::md4{},"12345678901234567890123456789012345678901234567890123456789012345678901234567890","e33b4ddc9c38f2199c3e7b164fcc0536");

hash_test(rom::md5{},"a","0cc175b9c0f1b6a831c399e269772661");
hash_test(rom::md5{},"abc","900150983cd24fb0d6963f7d28e17f72");
hash_test(rom::md5{},"message digest","f96b697d7cb7938d525a2f31aaf161d0");
hash_test(rom::md5{},"abcdefghijklmnopqrstuvwxyz","c3fcd3d76192e4007dfb496cca67e13b");
hash_test(rom::md5{},"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789","d174ab98d277d9f5a5611c2c9f419d9f");
hash_test(rom::md5{},"12345678901234567890123456789012345678901234567890123456789012345678901234567890","57edf4a22be3c955ac49da2e2107b67a");
hash_test(rom::md5{},"","d41d8cd98f00b204e9800998ecf8427e");

hash_test(rom::md5{},"World's simplest hash calculator. Just paste your text in the form below, press Calculate Hashes button, and you get dozens of hashes. Press button, get hashes. No ads, nonsense or garbage.","e3931110d30cfb8a67fc076cd0a80429");


}	//main
