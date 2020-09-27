#include "../include/toeridewallet.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::sendfare(
	const name& commuter_ac, 
	const name& contract_ac, 
	const asset& quantity, 
	const string& memo
	) {

	// check for conditions if either or both of these are true for `from` & `to`
	if(contract_ac != get_self() ||  commuter_ac == get_self()) {		// atleast one of the condition is true
		print("Either money is not sent to the contract or contract itself is the commuter.");
		return;
	}

	// check whether the `commuter_ac` is a verified commuter by reading the `auth` table
	user_index user_table(auth_contract_ac, "commuter"_n.value);
	auto user_it = user_table.find(commuter_ac.value);

	check( user_it != user_table.end(), "The commuter is not added in the Auth Table.");
	check( user_it->user_status == "verified"_n, "The commuter is not verified yet.");

	// Although this is checked in "toetoken::transfer" action, but ride_token_symbol check is pending. 
	// So, in addition the entire asset check is done using static func defined in "toeridewallet.hpp" file.
	// check quantity is valid for all conditions as 'asset'
	check_quantity(quantity);

	// instantiate the `ridewallet` table
	ridewallet_index ridewallet_table(get_self(), commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// update (add/modify) the balance
	if(wallet_it == ridewallet_table.end()) {
		ridewallet_table.emplace(get_self(), [&](auto& row) {
			row.balance = quantity;
		});
	} else {
		ridewallet_table.modify(wallet_it, get_self(), [&](auto& row) {
			row.balance += quantity;
		});
	}
	
	// On successful execution, an alert is sent
	send_receipt(
		commuter_ac, 
		commuter_ac.to_string() + " transfers an amount of " 
		+ quantity.to_string() + " to the contract -> " + contract_ac.to_string() );

}


// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::withdraw( const name& commuter_ac, 
					const asset& quantity) {
	require_auth(commuter_ac);

	// check quantity is valid for all conditions as 'asset'
	check_quantity(quantity);

	// instantiate the `ridewallet` table
	ridewallet_index ridewallet_table(get_self(), commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// Make sure that the commuter is present in the table
	check( wallet_it != ridewallet_table.end(), "Sorry! There is no amount transferred by " + commuter_ac.to_string() + "in the ride wallet.");

	check( wallet_it->balance >= quantity, "The user is trying to overdraw from the ride wallet's balance." );

	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), commuter_ac, quantity, "commuter withdraws " + quantity.to_string() + " money.")
	).send();

	// update the previous balance with new balance after substraction
	ridewallet_table.modify(wallet_it, get_self(), [&](auto& row) {
		row.balance -= quantity;
	});

	// check if zero balance, then delete the data
	if( (wallet_it->balance).amount == 0 ) {
		ridewallet_table.erase(wallet_it);
	}

	// On execution, a receipt is sent
	send_receipt( commuter_ac, name{commuter_ac}.to_string() + " withdraws " + quantity.to_string() + " amount." );

}


// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::withdrawfull( const name& commuter_ac ) {
	require_auth(commuter_ac);

	// instantiate the `ridewallet` table
	ridewallet_index ridewallet_table(get_self(), commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// Make sure that the commuter is present in the table
	check( wallet_it != ridewallet_table.end(), "Sorry! There is no amount transferred by " + commuter_ac.to_string() + "in the ride wallet.");

	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), commuter_ac, wallet_it->balance, "commuter withdraws " + (wallet_it->balance).to_string() + " money.")
	).send();

	// erase after transferring the entire amount in ride wallet balance
	ridewallet_table.erase(wallet_it);
}

// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::disburse(const name& receiver_ac,
								const name& wallet_holder,
								const asset& quantity,
								const string& memo )
{
	require_auth(ridetaxi_contract_ac);		// for single ride contract account - taxi (for now)
	// has_auth()		// for multiple ride contract accounts -taxi, bus, metro, ...

	check(is_account(receiver_ac), "receiver account doesn't exist");
	check(is_account(wallet_holder), "wallet holder's account doesn't exist");
	check(receiver_ac != wallet_holder, "As wallet holder & receiver accounts are same, so use \'withdraw\' action for this purpose.");
	check( receiver_ac != get_self(), "amount can't be sent to contract itself");

	// check quantity is valid for all conditions as 'asset'
	check_quantity(quantity);

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `ridewallet` table
	ridewallet_index ridewallet_table(get_self(), wallet_holder.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// Make sure that the commuter is present in the table
	check( wallet_it != ridewallet_table.end(), "Sorry! There is no amount transferred by " + wallet_holder.to_string() + "in the ride wallet.");

	// if the quantity is less than or equal to ride wallet balance
	check(wallet_it->balance >= quantity, "amount to be disbursed is higher than the ridewallet balance of wallet holder: " + wallet_holder.to_string());
	
	// initialized the ride_fees
	auto ride_fees = asset(0.0000, ride_token_symbol);
	ride_fees.amount = ride_commission_percent * (quantity.amount);

	// instantiate the ridetaxi table for reading the 'ridex_usagestatus_dri' field
	ridetaxi_index ridetaxi_table(ridetaxi_contract_ac, ridetaxi_contract_ac.value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(receiver_ac.value);

	if(ride_it->ridex_usagestatus_dri == "y"_n) {
		// transfer entire quantity from contract to the receiver account
		action(
			permission_level{get_self(), "active"_n},
			token_contract_ac,
			"transfer"_n,
			std::make_tuple(get_self(), receiver_ac, quantity, "ridewallet disburses " + quantity.to_string() + " to \'" + receiver_ac.to_string() + "\'.")
		).send();

		send_alert( receiver_ac, receiver_ac.to_string() + " recieves " + (quantity-ride_fees).to_string() + " amount for purpose: " + memo );

	} else if(ride_it->ridex_usagestatus_dri == "n"_n) {
		// transfer remaining quantity from contract to the receiver account
		action(
			permission_level{get_self(), "active"_n},
			token_contract_ac,
			"transfer"_n,
			std::make_tuple(get_self(), receiver_ac, quantity-ride_fees, "ridewallet disburses " + (quantity-ride_fees).to_string() + " to \'" + receiver_ac.to_string() + "\'.")
		).send();

		// transfer commission-fee quantity from contract to the toeridesfee account
		action(
			permission_level{get_self(), "active"_n},
			token_contract_ac,
			"transfer"_n,
			std::make_tuple(get_self(), rides_fees_ac, ride_fees, "ridewallet disburses " + ride_fees.to_string() + " to \'" + rides_fees_ac.to_string() + "\'.")
		).send();

		send_alert( receiver_ac, receiver_ac.to_string() + " recieves " + (quantity-ride_fees).to_string() + " amount for purpose: " + memo );
		send_alert( rides_fees_ac, rides_fees_ac.to_string() + " recieves " + ride_fees.to_string() + " amount for purpose: " + memo );
	}



	// update the previous balance with new balance after substraction
	ridewallet_table.modify(wallet_it, get_self(), [&](auto& row) {
		row.balance -= quantity;
	});

	// check if zero balance, then delete the data
	if( wallet_it->balance.amount == 0 ) {
		ridewallet_table.erase(wallet_it);
	}

	// On execution, a receipt is sent
	send_alert( wallet_holder, "ridewallet of \'" + wallet_holder.to_string() + "\' is deducted by " + quantity.to_string() + " amount." );

}
// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::sendalert(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridewallet::send_alert(const name& user, 
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
void toeridewallet::sendreceipt(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridewallet::send_receipt(const name& user, 
								const string& message) {
	check(message.size() <= 256, "message has more than 256 bytes");

	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendreceipt"_n,
		std::make_tuple(user, message)
		).send();
}
