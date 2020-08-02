#include "hashlibpp/hashlibpp.h"
#include <iostream>
#include <string>

using std::string;

int main() {
	hashwrapper *mywrapper = new sha256wrapper();

string name = "Dharmender Singh Dhillon";
string address = "K-52 \nBlock K, Kirti Nagar \nNew Delhi, Delhi 110015";
string country_id = "257425328532"; 

	try {
		std::string hash1 = mywrapper->getHashFromString(name + address + country_id);
		std::cout << hash1 << "\n";
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	delete mywrapper;

	return 0;
}