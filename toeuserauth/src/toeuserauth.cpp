#include "../include/toeuserauth.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::creatifyuser( const name& user,
								const name& type,
								const checksum256& profile_hash,
								const string& memo) {
	require_auth(user);

	check( (type == "driver"_n) 
		|| (type == "commuter"_n)
		|| (type == "validator"_n), "invalid user type");
	check(memo.size() <= 256, "memo has more than 256 bytes");

	user_index user_table(get_self(), user.value);
	auto user_it = user_table.find(user.value);

	if (user_it == user_table.end()) {		// not found
		user_table.emplace( user, [&](auto& row){
			row.user = user;
			row.type = type;
			row.profile_hash = profile_hash;
			row.user_status = "added"_n;
			row.add_timestamp = now();
		});
	} else {								// found
		check( user_it->type == type, "user's type doesn't match with the stored one.");

		user_table.modify( user_it, user, [&](auto& row) {
			row.profile_hash = profile_hash;
			row.user_status = "updated"_n;
			row.update_timestamp = now();
		});
	}



	// send_receipt() to user for getting added
	send_receipt(user, "the user details is added.");
}

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::vbdricom( const name& validator_user,
								const name& dricom_user,
								const name& dricom_user_status,
								const string& memo ) {
	// this action is done by validator => So, it's authority.
	require_auth(validator_user);

	// check if dricom_user account name is valid
	check(is_account(dricom_user), "invalid user account name");
	check( validator_user != dricom_user, "validator can't self-validate.");
	check( (dricom_user_status == "verified"_n) || (dricom_user_status == "blacklisted"_n), "user status has to be either verified or blacklisted" );
	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `users` table for validator_user
	user_index user_validator_table(get_self(), validator_user.value);
	auto user_validator_it = user_validator_table.find(validator_user.value);

	check( user_validator_it != user_validator_table.end(), "validator doesn't exist in the table."); 
	check( user_validator_it->type == "validator"_n, "the given validator account name is not a validator");
	check( user_validator_it->user_status == "verified"_n, "validator is not verified");

	// instantiate the `users` table for driver/commuter
	user_index user_dricom_table(get_self(), dricom_user.value);
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
			row.auth_timestamp = now();
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
	// authority by the company itself @TODO: can be replaced by a company a/c name: "bhubtoeindia"
	require_auth(get_self());
	// require_auth("bhubtoeindia"_n);

	check(is_account(validator_user), "invalid validator user account name");
	check( (validator_user_status == "verified"_n) 
		|| (validator_user_status == "blacklisted"_n), "user status has to be either verified or blacklisted" );
	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `users` table for validator_user
	user_index user_validator_table(get_self(), validator_user.value);
	auto user_validator_it = user_validator_table.find(validator_user.value);

	check( user_validator_it != user_validator_table.end(), "validator doesn't exist in the table."); 
	check( user_validator_it->type == "validator"_n, "the given validator account name is not a validator");
	check( (user_validator_it->user_status == "added"_n) 
		|| (user_validator_it->user_status == "updated"_n), "validator is either already verified or blacklisted");

	// add the status & timestamp for corresponding @param user_status
	user_validator_table.modify( user_validator_it, same_payer, [&](auto& row){
		row.user_status = validator_user_status;

		if(validator_user_status == "verified"_n) {
			row.validator_verify = get_self();		// contract or company name
			row.auth_timestamp = now();
		}

		if(validator_user_status == "blacklisted"_n) { 
			row.validator_blacklist = get_self();	// contract or company name
			row.blist_timestamp = now();
		}

	});

	//send_alert() to validator_user for getting verified/blacklisted
	send_alert(validator_user, "the user status is updated to " + validator_user_status.to_string() + " by " 
											+ get_self().to_string() + ". Note for this: " + memo);

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
