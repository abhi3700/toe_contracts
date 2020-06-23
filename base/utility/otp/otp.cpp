// #include <string>
#include "otp.hpp"

using std::string;

// int main() {
// 	string timestamp = "1592401869";	// cab booking timestamp
// 	string driver_ac_name = "cabeos1111d1";		// driver EOS account name
// 	string commuter_ac_name = "cabeos1111c1";	// commuter EOS account name

// 	std::cout << gen_otp(timestamp, driver_ac_name, commuter_ac_name) << "\n";

// 	return 0;	
// }

class [[eosio::contract]] otp : public contract
{
public:
	using contract::contract;

	// generate otp using required data
	[[eosio::action("genotp")]] 
	void gen_otp(string timestamp, name driver_ac_name, name commuter_ac_name) {
		auto otp = gen_otp(timestmap, drive_ac_name, commuter_ac_name);
		eosio::print("The generated OTP is, ", otp);
	}

	[[eosio::action("checkotp")]]
};
