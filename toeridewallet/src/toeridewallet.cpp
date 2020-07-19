#include "../include/toeridewallet.hpp"
#include "../../toeridetaxi/include/toeridetaxi.hpp"


// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::addpay_mode_status( const name& commuter_ac ) {
	toeridetaxi::addpay_action apay("toe1111rtaxi"_n, {get_self(), "active"_n} );
	apay.send(commuter_ac);
}

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

	// @TODO: check whether the `commuter_ac` is a verified commuter by reading the `auth` table

	/*
		@TODO
		- create the `toetoken` contract
	*/
	// check for negative amount value
	check( quantity.amount > 0, "Your quantity has negative amount value.");
	// Check for token symbol to be 'TOE'
	check( quantity.symbol == ride_token_symbol, "The token transferred is different");

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

	// add payment related status
	addpay_mode_status(commuter_ac);

	// instantiate the `ride` table to read data
/*	ridetaxi_index ridetaxi_table("toe1111rtaxi"_n, "toe1111rtaxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);
*/
	// update(add/modify) the & `pay_status`
/*	if(ride_it == ridetaxi_table.end()) {
		// @TODO: emplace data in the ridetaxi table
		ridetaxi_table.emplace(commuter_ac, [&] (auto& row) {
			row.commuter_ac = commuter_ac;
			row.pay_mode = "crypto";
			row.pay_status = "paidbycom";
		});
	} else {
		// @TODO: modify data in the ridetaxi table
		ridetaxi_table.modify(ride_it, commuter_ac, [&] (auto& row) {
			row.pay_mode = "crypto";
			row.pay_status = "paidbycom";
		});
	}
*/
	
	
	// On successful execution, an alert is sent
	send_receipt(
		commuter_ac, 
		name{commuter_ac}.to_string() + " transfers an amount of " 
		+ quantity.to_string() + " to the contract -> " + name(contract_ac).to_string() );

}


// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::withdraw( const name& commuter_ac, 
					const asset& quantity) {
	require_auth(commuter_ac);

	// instantiate the `ridewallet` table
	ridewallet_index ridewallet_table(get_self(), commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// Make sure that the commuter is present in the table
	check( wallet_it != ridewallet_table.end(), "Sorry! There is no such commuter in the ride wallet.");

	// if the quantity is less than or equal to ride wallet balance
	if(quantity <= wallet_it->balance ) {
		action(
			permission_level{get_self(), "active"_n},
			"toe1111token"_n,
			"transfer"_n,
			std::make_tuple(get_self(), commuter_ac, quantity, "commuter withdraws " + quantity.to_string() + " money.")
		).send();

		// update the previous balance with new balance after substraction
		ridewallet_table.modify(wallet_it, get_self(), [&](auto& row) {
			row.balance -= quantity;
		});

		// check if zero balance, then delete the data
		// @TODO: test whether 0 or 0.0000
		if( (wallet_it->balance).amount == 0 ) {
			ridewallet_table.erase(wallet_it);
		}

		// On execution, a receipt is sent
		send_receipt( commuter_ac, name{commuter_ac}.to_string() + " withdraws " + quantity.to_string() + " amount." );
	} else {
		// On failure, a receipt is sent
		send_receipt( commuter_ac, quantity.to_string() + " asked is higher than the balance amount in the ride wallet.");
		return;
	}

}


// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::withdrawfull( const name& commuter_ac ) {
	require_auth(commuter_ac);

	// instantiate the `ridewallet` table
	ridewallet_index ridewallet_table(get_self(), commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// Make sure that the commuter is present in the table
	check( wallet_it != ridewallet_table.end(), "Sorry! There is no such commuter in the ride wallet.");


	action(
		permission_level{get_self(), "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::make_tuple(get_self(), commuter_ac, wallet_it->balance, "commuter withdraws " + (wallet_it->balance).to_string() + " money.")
	).send();

	// erase after transferring the entire amount in ride wallet balance
	ridewallet_table.erase(wallet_it);
}


// --------------------------------------------------------------------------------------------------------------------
void toeridewallet::sendreceipt(
	const name& user,
	const string& message
	) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridewallet::send_receipt(const name& user, const string& message) {
	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendreceipt"_n,
		std::make_tuple(user, message)
		).send();
}
