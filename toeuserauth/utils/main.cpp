#include "hashlibpp/hashlibpp.h"
#include <iostream>
#include <string>

using std::string;

int main() {
	hashwrapper *mywrapper = new sha256wrapper();

string name = "Ramesh Chopra";
string address = "F-24 \nBlock F, Kirti Nagar \nNew Delhi, Delhi 110015";
string country_id = "434731547311"; 

	try {
		std::string hash1 = mywrapper->getHashFromString(name + address);
		std::string hash2 = mywrapper->getHashFromString(country_id);
		std::cout << hash1 << "\n";
		std::cout << hash2 << "\n";
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	delete mywrapper;

	return 0;
}