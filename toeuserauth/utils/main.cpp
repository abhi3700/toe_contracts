#include "hashlibpp/hashlibpp.h"
#include <iostream>
#include <string>

using std::string;

int main() {
	hashwrapper *mywrapper = new sha256wrapper();

	string name = "Ramesh Chakravorty";
	string address = "5 Garfa 8th Lane, Jadavpur, Kolkata - 700045";
	string country_id = "653278229043"; 

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