// #include <string>
#include <iostream>  //for "cerr"
#include "gen_otp.hpp"

using std::string;

int main() {
	string timestamp = "1592401869";	// cab booking timestamp
	string driver_ac_name = "cabeos1111d1";		// driver EOS account name
	string commuter_ac_name = "cabeos1111c1";	// commuter EOS account name

	std::cout << gen_otp(timestamp, driver_ac_name, commuter_ac_name) << "\n";

	return 0;	
}
