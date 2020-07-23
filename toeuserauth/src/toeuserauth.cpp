#include "../include/toeuserauth.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::adduser( const name& user,
							const name& type,
							const checksum256& profile_hash) {
	require_auth(user);

	user_index user_table(get_self(), user.value);
	auto user_it = user_table.find(user.value);

	check(user_it == user_table.end(), "this eosio user account already exists.");

	user_table.emplace( user, [&](auto& row){
		row.user = user;
		row.type = type;
		row.profile_hash = profile_hash;
		row.user_status = "added"_n;
		row.add_timestamp = now();
	});


	// send_receipt() to user for getting added
	send_receipt(user, "the user details is added.");
}

// --------------------------------------------------------------------------------------------------------------------
void toeuserauth::addusrstatus( const name& user,
								const name& user_status,
								const string& memo ) {
	// this action is done by the contract. So, its permission.
	require_auth(get_self());

	check(is_account(user), "invalid user account name");

	// instantiate the `users` table
	user_index user_table(get_self(), user.value);
	auto user_it = user_table.find(user.value);

	// ensure that the user is found in the table
	check(user_it != user_table.end(), "this user account is not a user of the DApp.");
	
	// add the status & timestamp for corresponding @param user_status
	user_table.modify( user_it, same_payer, [&](auto& row){
		row.user_status = user_status;

		if(user_status == "added"_n) { 
			row.add_timestamp = now();
		} 

		if(user_status == "verified"_n) { 
			row.auth_timestamp = now();
		}

		if(user_status == "blacklisted"_n) { 
			row.blist_timestamp = now();
		}

	});


	//send_alert() to user for getting blacklisted
	send_alert(user, "the user status is updated to " + user_status.to_string() + ". Note for this: " + memo);


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
