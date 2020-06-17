#ifndef GEN_OTP_HPP
#define GEN_OTP_HPP

#include "hashlibpp/hashlibpp.h"

using std::string;

/**
 * @brief Generates OTP (4-letter)  
 * @details Generates OTP for ride verification
 * 
 * @param timestamp - time at which the cab booking was done/modified.
 * @param driver_ac_name - EOS account name of Driver
 * @param commuter_ac_name - EOS account name of Commuter
 * @return [description]
 */
string gen_otp(string timestamp, string driver_ac_name, string commuter_ac_name) {
	//creating a wrapper object 
	hashwrapper *myWrapper = new sha256wrapper();
	
	// concatenating all the fields in the fastest C++ way.
	string otp_str = timestamp;
	otp_str += driver_ac_name;
	otp_str += commuter_ac_name;

	string hash1 = "", otp = "";
	try{
		// create a hash from a string
		hash1 = myWrapper->getHashFromString(otp_str);
		// std::cout << hash1 << "\n";
		// std::cout << hash1.length() << "\n";

		// now, divide the hash into 4 different parts
		// & the OTP is [0], [16], [32], [63] i.e. 4 digit 
		otp = hash1[0];
		otp += hash1[16];
		otp += hash1[32];
		otp += hash1[63];

		// std::cout << "The final OTP is: " << otp << "\n";

	    
	}catch(hlException &e) {
	 	// std::cerr << "Error("  << e.error_number() << "): " << e.error_message() << std::endl;
	}

	delete myWrapper;
	return otp;
}

#endif // GEN_OTP_HPP