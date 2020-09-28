#include "hashlibpp/hashlibpp.h"
#include <iostream>
#include <string>

using std::string;

int main() {
	hashwrapper *mywrapper = new sha256wrapper();

string name = "Arun K P";
string address = "#890, Sector 78, Mohali, Punjab - 160078";
string country_id = "543254265354"; 

	try {
		std::string hash1 = mywrapper->getHashFromString(country_id);
		std::string hash2 = mywrapper->getHashFromString(name + address);
		std::cout << hash1 << "\n";
		std::cout << hash2 << "\n";
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	delete mywrapper;

	return 0;
}