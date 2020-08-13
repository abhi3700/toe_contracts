#include "../include/toeridex.hpp"

// --------------------------------------------------------------------------------
void toeridex::sendridex(
						const name& sender,
						const name& contract_ac,
						const asset& quantity,
						const string& memo) 
{
	// check for conditions if either or both of these are true for `from` & `to`
	if(contract_ac != get_self() ||  sender == get_self()) {		// atleast one of the condition is true
		print("Either money is not sent to the contract or contract itself is the sender.");
		return;
	}

	// instantiate the stat table
	stats_index stats_table(token_contract_ac, ride_token_symbol.code().raw());
	auto stats_it = stats_table.find(ride_token_symbol.code().raw());

	// Already done in "toetoken::transfer"
	// check fareamount is valid for all conditions as 'asset'
	// check_quantity(quantity);

	// check the initial_ride_qty is non-zero
	check(initial_ride_qty != 0, "Ride quantity can't be zero");


	auto ride_type = ""_n;
	// Assuming these
/*    
    string memo = "init ridex for driver ride type";
    string memo = "init ridex for commuter ride type";
    string memo = "buy 1 ride";
*/    

	size_t found_1 = memo.find("driver");
	size_t found_2 = memo.find("commuter");
	size_t found_3 = memo.find("buy");

	// 3 cases
	auto case_1 = (found_1 != string::npos) && (found_2 == string::npos) && (found_3 == string::npos);		// contain only "driver"
	auto case_2 = (found_2 != string::npos) && (found_1 == string::npos) && (found_3 == string::npos);		// contain only "commuter"
	auto case_3	= (found_3 != string::npos) && (found_1 == string::npos) && (found_2 == string::npos);		// contain only "buy"

	check( 
		case_1 ||
		case_2 ||
		case_3
		, "invalid memo type for RIDEX contract");

	/*
		Set respective table data for different found(s)
	*/
	if( case_1 ) {
		ride_type = "driver"_n;

		// check if sender is the token_issuer from token contract's stat table.
		check(stats_it != stats_table.end(), "the token symbol doesn't exist");
		check(stats_it->issuer == sender, "The sender doesn't match with stats table's issuer.");

		// instantiate the ridex table for driver
		ridex_index ridex_table(get_self(), get_self().value);
		auto ridex_it = ridex_table.find(ride_type.value);

		check(ridex_it == ridex_table.end(), "The values for \'" + ride_type.to_string() + "\' ride_type is already initialized.");

		// add ridex_table for driver
		ridex_table.emplace(get_self(), [&](auto& row) {
			row.ride_type = ride_type;
			row.ride_quota = initial_ride_qty;
			row.toe_balance = quantity;
		});

		// On successful execution, a receipt is sent
		send_receipt( sender, "initialized RIDEX with " + quantity.to_string() + " & " 
			+ std::to_string(initial_ride_qty) + " RIDE for \'" + ride_type.to_string() + "\' ride type.");

	} else if( case_2 ) {
		ride_type = "commuter"_n;

		// check if sender is the token_issuer from token contract's stat table.
		check(stats_it != stats_table.end(), "the token symbol doesn't exist");
		check(stats_it->issuer == sender, "The sender doesn't match with stats table's issuer.");

		// instantiate the ridex table for driver
		ridex_index ridex_table(get_self(), get_self().value);
		auto ridex_it = ridex_table.find(ride_type.value);

		check(ridex_it == ridex_table.end(), "The values for \'" + ride_type.to_string() + "\' ride_type is already initialized.");

		// add ridex_table for driver
		ridex_table.emplace(get_self(), [&](auto& row) {
			row.ride_type = ride_type;
			row.ride_quota = initial_ride_qty;
			row.toe_balance = quantity;
		});

		// On successful execution, a receipt is sent
		send_receipt( sender, "initialized RIDEX with " + quantity.to_string() + " & " 
			+ std::to_string(initial_ride_qty) + " RIDE for \'" + ride_type.to_string() + "\' ride type.");

	} else if( case_3 ) {
		// ride_type = "buy"_n;
	
		// instantiate the rexusrwallet table
		rexusrwallet_index rexusrwallet_table(get_self(), sender.value);
		auto rexusrwallet_it = rexusrwallet_table.find(quantity.symbol.raw());	// finding the ("TOE", 4)

		if(rexusrwallet_it == rexusrwallet_table.end()) {
			rexusrwallet_table.emplace(get_self(), [&](auto& row){
				row.balance = quantity;
			});
		} else {
			rexusrwallet_table.modify(rexusrwallet_it, get_self(), [&](auto& row){
				row.balance += quantity;
			});
		}

		// On successful execution, a receipt is sent
		send_receipt( sender, sender.to_string() + " sent \'" + quantity.to_string() + "\' money for purpose: " + memo);

	}

}


// --------------------------------------------------------------------------------------------------------------------
void toeridex::buyride( const name& buyer,
				const name& ride_type,
				uint64_t ride_qty,
				const string& memo )
{
	require_auth( buyer );

	// check the ride_type is "driver" or "commuter"
	check( (ride_type == "driver"_n) || (ride_type == "commuter"_n), "invalid ride type");

	// check if the buyer is verified & is eligible to buy asked 'ride_type' rides
	check_buyer_seller( buyer, ride_type );

	// check the ride_qty is non-zero
	check(ride_qty != 0, "Ride quantity can't be zero");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// check if the row exist for given ride_type
	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(ride_type.value);
	check(ridex_it != ridex_table.end(), "There is no data found in RIDEX for \'" + ride_type.to_string() + "\' ride_type.");

	// initialized the ride_expenditure
	auto ride_expenditure = asset(0.0000, ride_token_symbol);

	// calc ride_expenditure amount using the Bancor formula
	ride_expenditure.amount = (ride_qty * (ridex_it->toe_balance.amount))/( ride_qty + (ridex_it->ride_quota) );

	// initialized the ride_expend_fees
	auto ride_expend_fees = asset(0.0000, ride_token_symbol);
	ride_expend_fees.amount = fees_factor * (ride_expenditure.amount);

	// to get the ride_expend_supply, just substract: (expenditure - fees) 
	auto ride_expend_supply = ride_expenditure - ride_expend_fees;

	// check if the rexusrwallet table for buyer has `ride_expenditure` as min. balance
	rexusrwallet_index rexusrwallet_table(get_self(), buyer.value);
	auto rexusrwallet_it = rexusrwallet_table.find(ride_expenditure.symbol.raw());	// finding the ("TOE", 4)

	check(rexusrwallet_it != rexusrwallet_table.end(), "The RIDEX wallet for buyer has no balance. Please, transfer \'" + ride_expenditure.to_string() + "\'");
	check(rexusrwallet_it->balance >= ride_expenditure, "Low balance. So, transfer \'" + (ride_expenditure - rexusrwallet_it->balance).to_string() + "\' to RIDEX wallet");

	// send the fees from toeridex to toeridexsupp using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), ridex_fees_ac, ride_expend_fees, "transfer fees amount for buying " + std::to_string(ride_qty) + " ride(s)")
		).send();

	// update the ride_table with new `ride_quota` & `toe_balance`
	ridex_table.modify(ridex_it, get_self(), [&](auto& row){
		row.ride_quota -= ride_qty;
		row.toe_balance += ride_expend_supply;
	});

	// add ride_qty to the rexuseraccnt table
	rexuseraccnt_index rexuseraccnt_table(get_self(), buyer.value);
	auto rexuseraccnt_it = rexuseraccnt_table.find(ride_type.value);

	if(rexuseraccnt_it == rexuseraccnt_table.end()) {
		rexuseraccnt_table.emplace(buyer, [&](auto& row){
			row.ride_type = ride_type;
			row.rides_limit = ride_qty;
		});
	}
	else {
		rexuseraccnt_table.modify(rexuseraccnt_it, buyer, [&](auto& row){
			row.rides_limit += ride_qty;
		});
	}

	// substract the 'ride_expenditure' from 'buyer' rexusrwallet table
	rexusrwallet_table.modify(rexusrwallet_it, get_self(), [&](auto& row){
		row.balance -= ride_expenditure;
	});

	send_receipt(buyer, "You bought " + std::to_string(ride_qty) + " rides for \'" + ride_expenditure.to_string() + "\' amount");

}
// --------------------------------------------------------------------------------------------------------------------
void toeridex::sellride( const name& seller,
						const name& ride_type,
						uint64_t ride_qty,
						const string& memo)
{
	require_auth( seller );

	// check the ride_type is "driver" or "commuter"
	check( (ride_type == "driver"_n) || (ride_type == "commuter"_n), "invalid type");

	// check if the seller is verified & is eligible to sell asked 'ride_type' rides
	check_buyer_seller( seller, ride_type );

	check(ride_qty != 0, "Ride quantity can't be zero");

	// instantiate the rexuseraccnt table
	rexuseraccnt_index rexuseraccnt_table(get_self(), seller.value);
	auto rexuseraccnt_it = rexuseraccnt_table.find(ride_type.value);

	// check if the ride_qty < ride_limit of seller
	check(rexuseraccnt_it != rexuseraccnt_table.end(), "Sorry! There is no ride to sell.");
	check(ride_qty <= rexuseraccnt_it->rides_limit, "The ride no. asked, is more than the seller's limit");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// check if the row exist for given ride_type
	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(ride_type.value);
	check(ridex_it != ridex_table.end(), "There is no data found in RIDEX for \'" + ride_type.to_string() + "\' ride_type.");

	// initialized the ride_expenditure
	auto ride_expenditure = asset(0.0000, ride_token_symbol);

	// calc ride_expenditure amount using the Bancor formula
	ride_expenditure.amount = (ride_qty * (ridex_it->toe_balance.amount))/( ride_qty + (ridex_it->ride_quota) );

	// initialized the ride_expend_fees
	auto ride_expend_fees = asset(0.0000, ride_token_symbol);
	ride_expend_fees.amount = fees_factor * (ride_expenditure.amount);

	// to get the ride_expend_supply, just substract: (expenditure - fees) 
	auto ride_expend_supply = ride_expenditure - ride_expend_fees;

	// send the ride_expend_supply from contract_ac to seller using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), seller, ride_expend_supply, "sell " + std::to_string(ride_qty) + " ride(s)")
	).send();

	// send the ride_expend_fees from contract_ac to toeridexfees account using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), ridex_fees_ac, ride_expend_fees, "transfer fees amount for selling " + std::to_string(ride_qty) + " ride(s)")
	).send();

	// update the ridex_table with new `ride_quota` & `toe_balance`
	ridex_table.modify(ridex_it, get_self(), [&](auto& row){
		row.ride_quota += ride_qty;
		row.toe_balance -= ride_expend_supply;
	});

	// deduct ride_qty to the ridexaccount table	
	rexuseraccnt_table.modify(rexuseraccnt_it, seller, [&](auto& row){
		row.rides_limit -= ride_qty;
	});

	send_receipt(seller, "You sold " + std::to_string(ride_qty) + " rides for \'" + ride_expenditure.to_string() + "\' amount");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridex::consumeride( const name& user,
							const name& ride_type,
							uint64_t ride_qty )
{
	// Authority by toeridetaxi
	require_auth(ride_contract_ac);
	// has_auth()		// for accessing by other ride contracts - bus, metro,..

	// check user is a verified one
	user_index user_table(auth_contract_ac, user.value);
	auto user_it = user_table.find(user.value);

	check(user_it != user_table.end(), "Sorry! The user is not registered with us.");
	check(user_it->user_status == "verified"_n, "Sorry! The user is not yet verified.");

	// Only accessed by "driver" or "commuter"
	check((user_it->type == "driver"_n) || (user_it->type == "commuter"_n), "Other types are not allowed for this action");

	// check ride_type
	check((ride_type == "driver"_n) || (ride_type == "commuter"_n), "Other ride type is not eligible for this action." );

	// check ride quantity == 1
	check(ride_qty == 1, "Ride quantity must be one.");

	// instantiate the rexuseraccnt table
	rexuseraccnt_index rexuseraccnt_table(get_self(), user.value);
	auto rexuseraccnt_it = rexuseraccnt_table.find(ride_type.value);

	// check if the ride_qty exists
	check(rexuseraccnt_it != rexuseraccnt_table.end(), "Sorry! There is no ride to consume.");
	check(rexuseraccnt_it->rides_limit >= ride_qty, "The rides_limit must have min. value as ride_qty: " + std::to_string(ride_qty));

	// deduct ride_qty to the ridexaccount table	
	rexuseraccnt_table.modify(rexuseraccnt_it, user, [&](auto& row){
		row.rides_limit -= ride_qty;
	});

}

// --------------------------------------------------------------------------------------------------------------------
void toeridex::restoreride( const name& user,
							const name& ride_type,
							uint64_t ride_qty )
{
	// Authority by toeridetaxi
	require_auth(ride_contract_ac);
	// has_auth()		// for accessing by other ride contracts - bus, metro,..

	// check user is a verified one
	user_index user_table(auth_contract_ac, user.value);
	auto user_it = user_table.find(user.value);

	check(user_it != user_table.end(), "Sorry! The user is not registered with us.");
	check(user_it->user_status == "verified"_n, "Sorry! The user is not yet verified.");
	
	// Only accessed by "driver" or "commuter"
	check(user_it->type == "commuter"_n, "Other types are not allowed for this action");

	// check ride_type is commuter only, if accessed only by commuter
	check(ride_type == "commuter"_n, "Other ride type is not eligible for this action." );

	// check ride quantity == 1
	check(ride_qty == 1, "Ride quantity must be one.");

	// instantiate the rexuseraccnt table
	rexuseraccnt_index rexuseraccnt_table(get_self(), user.value);
	auto rexuseraccnt_it = rexuseraccnt_table.find(ride_type.value);

	// check if the ride_qty exists
	check(rexuseraccnt_it != rexuseraccnt_table.end(), "Sorry! There is no ride to consume.");

	// add ride_qty to the ridexaccount table	
	rexuseraccnt_table.modify(rexuseraccnt_it, user, [&](auto& row){
		row.rides_limit += ride_qty;
	});

}

// --------------------------------------------------------------------------------------------------------------------
// void toeridex::transferidex( const name& sender,
// 							const name& receiver,
// 							const name& ride_type,
// 							uint64_t ride_qty)
// {
// 	// Authority by sender of validator type
// 	require_auth(sender);

// 	// check sender != receiver
// 	check(sender != receiver, "sender & receiver must be different");

// 	// check sender & receiver is a verified one
// 	user_index sender_table(auth_contract_ac, sender.value);
// 	auto sender_it = sender_table.find(sender.value);

// 	check(sender_it != sender_table.end(), "Sorry! The sender is not registered with us.");
// 	check(sender_it->user_status == "verified"_n, "Sorry! The sender is not yet verified.");
	
// 	// Only accessed by "validator"
// 	check(sender_it->type == "validator"_n, "Sender must be a validator for this action");

// 	// check ride_type is commuter only, if accessed only by validator
// 	check(ride_type == "commuter"_n, "Other ride type is not eligible for this action." );

// 	// check ride quantity == 1
// 	check(ride_qty == 1, "Ride quantity must be one.");

// 	// instantiate the rexuseraccnt table
// 	rexuseraccnt_index rexsenderaccnt_table(get_self(), sender.value);
// 	auto rexsenderaccnt_it = rexsenderaccnt_table.find(ride_type.value);

// 	// check if the ride_qty exists
// 	check(rexsenderaccnt_it != rexsenderaccnt_table.end(), "Sorry! There is no ride to transfer.");

// 	// add ride_qty to the ridexaccount table	
// 	rexsenderaccnt_table.modify(rexsenderaccnt_it, sender, [&](auto& row){
// 		row.rides_limit -= ride_qty;
// 	});

// 	// instantiate the rexuseraccnt table
// 	rexuseraccnt_index rexuseraccnt_table(get_self(), seller.value);
// 	auto rexuseraccnt_it = rexuseraccnt_table.find(ride_type.value);

// 	// check if the ride_qty < ride_limit of seller
// 	check(rexuseraccnt_it != rexuseraccnt_table.end(), "Sorry! There is no ride to sell.");
// 	check(ride_qty <= rexuseraccnt_it->rides_limit, "The ride no. asked, is more than the seller's limit");

// }


// --------------------------------------------------------------------------------------------------------------------
void toeridex::addridequota(const name& ride_type,
							uint64_t ride_qty )
{
	// explicitly given permission to only ride contract ac for this action
	// for more linkage to other contracts - 'bus', 'metro', 'trains' use `has_auth()`
	require_auth(ride_contract_ac);

	// check the ride_type is "driver" or "commuter"
	check( (ride_type == "driver"_n) || (ride_type == "commuter"_n), "invalid ride_type");

	check(ride_qty != 0, "Ride quantity can't be zero");

	// instantiate the ridex table
	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(ride_type.value);

	// check if the row exist for given ride_type
	check(ridex_it != ridex_table.end(), "There is no data found for this ride_type.");

	// update the ridex_table with additional `ride_qty`
	ridex_table.modify(ridex_it, get_self(), [&](auto& row){
		row.ride_quota += ride_qty;
	});

}

// --------------------------------------------------------------------------------------------------------------------
void toeridex::withdraw( const name& user, 
							const asset& quantity) {
	require_auth(user);

	// check quantity is valid for all conditions as 'asset'
	check_quantity(quantity);

	// instantiate the `rexusrwallet` table
	rexusrwallet_index rexusrwallet_table(get_self(), user.value);
	auto rexusrwallet_it = rexusrwallet_table.find(ride_token_symbol.raw());

	// Make sure that the commuter is present in the table
	check( rexusrwallet_it != rexusrwallet_table.end(), "Sorry! There is no amount transferred by " + user.to_string() + "in the RIDEX wallet.");

	// if the quantity is less than or equal to ride wallet balance
	check(rexusrwallet_it->balance >= quantity, "The user is trying to overdraw from the ride wallet's balance.");
		
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), user, quantity, "user withdraws " + quantity.to_string() + " money.")
	).send();

	// update the previous balance with new balance after substraction
	rexusrwallet_table.modify(rexusrwallet_it, get_self(), [&](auto& row) {
		row.balance -= quantity;
	});

	// check if zero balance, then delete the data
	if( (rexusrwallet_it->balance).amount == 0 ) {
		rexusrwallet_table.erase(rexusrwallet_it);
	}

	// On execution, a receipt is sent
	send_receipt( user, user.to_string() + " withdraws " + quantity.to_string() + " amount." );

}


// --------------------------------------------------------------------------------------------------------------------
void toeridex::withdrawfull( const name& user ) {
	require_auth(user);

	// instantiate the `ridewallet` table
	rexusrwallet_index rexusrwallet_table(get_self(), user.value);
	auto rexusrwallet_it = rexusrwallet_table.find(ride_token_symbol.raw());

	// Make sure that the commuter is present in the table
	check( rexusrwallet_it != rexusrwallet_table.end(), "Sorry! There is no amount transferred by " + user.to_string() + "in the ride wallet.");

	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), user, rexusrwallet_it->balance, "commuter withdraws " + (rexusrwallet_it->balance).to_string() + " money.")
	).send();

	// erase after transferring the entire amount in ride wallet balance
	rexusrwallet_table.erase(rexusrwallet_it);
}


// --------------------------------------------------------------------------------------------------------------------
void toeridex::sendalert(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridex::send_alert(const name& user, 
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
void toeridex::sendreceipt(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridex::send_receipt(const name& user, 
								const string& message) {
	check(message.size() <= 256, "message has more than 256 bytes");

	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendreceipt"_n,
		std::make_tuple(user, message)
		).send();
}
