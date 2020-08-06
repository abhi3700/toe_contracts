#include "hashlibpp/hashlibpp.h"
#include <iostream>
#include <string>

using std::string;

int main() {
	hashwrapper *mywrapper = new sha256wrapper();

string src_lat = "30.6715713";
string src_lon = "76.701094";
string des_lat = "30.703957";
string des_lon = "76.6999052";

	try {
		std::string src_lat_hash = mywrapper->getHashFromString(src_lat);
		std::string src_lon_hash = mywrapper->getHashFromString(src_lon);
		std::string des_lat_hash = mywrapper->getHashFromString(des_lat);
		std::string des_lon_hash = mywrapper->getHashFromString(des_lon);

		std::cout << src_lat_hash << "\n";
		std::cout << src_lon_hash << "\n";
		std::cout << des_lat_hash << "\n";
		std::cout << des_lon_hash << "\n";
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	delete mywrapper;

	return 0;
}