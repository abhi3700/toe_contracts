#include "../include/toeuserauth.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::signup( const name& user,
							const name& type,
							const checksum256& national_id_hash,
							const checksum256& profile_hash,
							const string& memo) {
	require_auth(user);

	check( (type == "driver"_n) 
		|| (type == "commuter"_n)
		|| (type == "validator"_n), "invalid user type");
	check(memo.size() <= 256, "memo has more than 256 bytes");

	/*
		NOTE: For scope, 3 choices
				- M-1: contract [No, as the load will increase due to all users in single table.]
				- M-2: type [Yes]
				- M-3: user [No, as finding for accounts held by same person, will not be possible] 
		Here, the scope is chosen as the `type` i.e. "driver", "commuter", "validator", to reduce the load (in case of contract as scope.)
				So, if a validator registers as commuter, where in both cases they shall have a common `national_id_hash`,
				then, in order to check if a person has more than 1 account, it can be done by searching in all 3 tables,
				corresponding to a `national_id_hash` param.

	*/
	// Instantiate the user table
	user_index user_table(get_self(), type.value);
	auto user_it = user_table.find(user.value);

	if (user_it == user_table.end()) {		// not found
		user_table.emplace( user, [&](auto& row){
			row.user = user;
			row.national_id_hash = national_id_hash;
			row.profile_hash = profile_hash;
			row.user_status = "added"_n;
			row.add_timestamp = now();
		});

		// send_receipt() to user for adding
		send_receipt(user, "the user registers by adding as a " + type.to_string());

	} else {								// found
		user_table.modify( user_it, user, [&](auto& row) {
			row.national_id_hash = national_id_hash;
			row.profile_hash = profile_hash;
			row.user_status = "updated"_n;
			row.update_timestamp = now();
	
		// send_receipt() to user for updating
		send_receipt(user, "the user registers by updating as a " + type.to_string());
	
		});
	}

}


// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::vbdricom( const name& validator_user,
								const name& dricom_user,
								const name& dricom_user_type,
								const name& dricom_user_status,
								const string& memo ) {
	// this action is done by validator => So, it's authority.
	require_auth(validator_user);

	// check if dricom_user account name is valid
	check(is_account(dricom_user), "invalid user account name");
	check( validator_user != dricom_user, "validator can't self-validate.");
	check( (type == "driver"_n) 
		|| (type == "commuter"_n), "invalid user type for this action.");
	check( (dricom_user_status == "verified"_n) || (dricom_user_status == "blacklisted"_n), "user status has to be either verified or blacklisted" );
	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `users` table for validator_user
	user_index user_validator_table(get_self(), "validator".value);
	auto user_validator_it = user_validator_table.find(validator_user.value);

	check( user_validator_it != user_validator_table.end(), "validator doesn't exist in the table."); 
	check( user_validator_it->type == "validator"_n, "the given validator account name is not a validator");
	check( user_validator_it->user_status == "verified"_n, "validator is not verified");

	// instantiate the `users` table for driver/commuter
	user_index user_dricom_table(get_self(), dricom_user_type.value);
	auto user_dricom_it = user_dricom_table.find(dricom_user.value);

	// ensure that the dricom user is found in the table
	check(user_dricom_it != user_dricom_table.end(), "this user account is not a user of the DApp.");

	// check if the dricom_user details is either "added" or "updated".
	check((user_dricom_it->user_status == "added"_n) 
		|| (user_dricom_it->user_status == "updated"_n), "user is either already verified or blacklisted.");
	
	// add the status & timestamp for corresponding @param user_status
	user_dricom_table.modify( user_dricom_it, same_payer, [&](auto& row){
		row.user_status = dricom_user_status;

		if(dricom_user_status == "verified"_n) {
			row.validator_verify = validator_user;
			row.verify_timestamp = now();
		}

		if(dricom_user_status == "blacklisted"_n) { 
			row.validator_blacklist = validator_user;
			row.blist_timestamp = now();
		}

	});


	//send_alert() to dricom_user for getting verified/blacklisted
	send_alert(dricom_user, "the user status is updated to " + dricom_user_status.to_string() + " by " 
											+ validator_user.to_string() + ". Note for this: " + memo);


}

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::compvbvdator( const name& validator_user,
								const name& validator_user_status,
								const string& memo) {
	// authority by the company itself i.e. company a/c name: "bhubtoeinval"
	require_auth(company_validator_ac);

	check(is_account(company_validator_ac), "invalid company validator's account name");
	check(validator_user != company_validator_ac, "validator_user passed can't be company_validator_ac.");

	check(is_account(validator_user), "invalid validator user account name");
	check( (validator_user_status == "verified"_n) 
		|| (validator_user_status == "blacklisted"_n), "user status has to be either verified or blacklisted" );
	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `users` table for validator_user
	user_index user_validator_table(get_self(), "validator".value);
	auto user_validator_it = user_validator_table.find(validator_user.value);

	check( user_validator_it != user_validator_table.end(), "validator doesn't exist in the table."); 
	check( (user_validator_it->user_status == "added"_n) 
		|| (user_validator_it->user_status == "updated"_n), "validator is either already verified or blacklisted");

	// add the status & timestamp for corresponding @param user_status
	user_validator_table.modify( user_validator_it, same_payer, [&](auto& row){
		row.user_status = validator_user_status;

		if(validator_user_status == "verified"_n) {
			row.validator_verify = company_validator_ac;		// contract or company name
			row.verify_timestamp = now();
		}

		if(validator_user_status == "blacklisted"_n) { 
			row.validator_blacklist = company_validator_ac;	// contract or company name
			row.blist_timestamp = now();
		}

	});

	//send_alert() to validator_user for getting verified/blacklisted
	send_alert(validator_user, "the user status is updated to " + validator_user_status.to_string() + " by " 
											+ company_validator_ac.to_string() + ". Note for this: " + memo);

}

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::deluser( const name& user,
							const string& memo ) {
	// authority by either user or contract
	check( has_auth(user) || has_auth(get_self()), "missing required authority of " 
													+ user.to_string() + " or " + get_self().to_string() );

	check(is_account(user), "invalid user account name");

	// instantiate the `users` table
	user_index user_table(get_self(), user.value);
	auto user_it = user_table.find(user.value);

	// ensure that the user is found in the table
	check(user_it != user_table.end(), "this user account is not a user of the DApp.");

	// erase
	user_table.erase(user_it);

	// either receipt or alert
	send_alert( user, "The user is deleted due to: " + memo );
}

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::sendalert(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeuserauth::send_alert(const name& user, 
							const string& message) {
	check(message.size() <= 256, "message has more than 256 bytes");
	
	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendalert"_n,
		std::make_tuple(user, message)
		).send();
}

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::sendreceipt(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeuserauth::send_receipt(const name& user, 
								const string& message) {
	check(message.size() <= 256, "message has more than 256 bytes");

	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendreceipt"_n,
		std::make_tuple(user, message)
		).send();
}
