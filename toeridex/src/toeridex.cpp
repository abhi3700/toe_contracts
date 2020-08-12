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


	check( 
		(found_1 != string::npos) ||
		(found_2 != string::npos) ||
		(found_3 != string::npos)
		, "invalid memo type for RIDEX contract");

	/*
		Set respective table data for different found(s)
	*/
	if(found_1 != string::npos) {
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

	} else if(found_2 != string::npos) {
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

	} else if(found_3 != string::npos) {
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
	check(ridex_it != ridex_table.end(), "There is no data found for this ride_type.");

	// initialized the ride_expenditure
	auto ride_expenditure = asset(0.0000, ride_token_symbol);

	// calc ride_expenditure amount using the Bancor formula
	ride_expenditure.amount = (ride_qty * (ridex_it->toe_balance.amount))/( ride_qty + (ridex_it->ride_quota) );

	// initialized the ride_expend_supply
	auto ride_expend_supply = asset(0.0000, ride_token_symbol);
	ride_expend_supply.amount = supply_factor * (ride_expenditure.amount);

	// initialized the ride_expend_fees
	auto ride_expend_fees = asset(0.0000, ride_token_symbol);
	ride_expend_fees.amount = fees_factor * (ride_expenditure.amount);

	// send the ride_expend_supply from buyer to toeridexsupp using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(buyer, ridex_supply_ac, ride_expend_supply, "buy " + std::to_string(ride_qty) + " ride(s)")
		).send();

	// send the toe_qty from buyer to toeridexsupp using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(buyer, ridex_fees_ac, ride_expend_fees, "fees for buy " + std::to_string(ride_qty) + " ride(s)")
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

	rexuseraccnt_index rexuseraccnt_table(get_self(), seller.value);
	auto rexuseraccnt_it = rexuseraccnt_table.find(ride_type.value);
	check(rexuseraccnt_it != rexuseraccnt_table.end(), "Sorry! There is no ride to sell.");
	check(ride_qty <= rexuseraccnt_it->rides_limit, "The ride no. asked, is more than the limit of the seller.");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// check if the row exist for given ride_type
	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(ride_type.value);
	check(ridex_it != ridex_table.end(), "There is no data found for this ride_type.");

	// initialized the ride_expenditure
	auto ride_expenditure = asset(0.0000, ride_token_symbol);

	// calc ride_expenditure amount using the Bancor formula
	ride_expenditure.amount = (ride_qty * (ridex_it->toe_balance.amount))/( ride_qty + (ridex_it->ride_quota) );

	// initialized the ride_expend_supply
	auto ride_expend_supply = asset(0.0000, ride_token_symbol);
	ride_expend_supply.amount = supply_factor * (ride_expenditure.amount);

	// initialized the ride_expend_fees
	auto ride_expend_fees = asset(0.0000, ride_token_symbol);
	ride_expend_fees.amount = fees_factor * (ride_expenditure.amount);

	// send the ride_expend_supply from toeridexsupp to seller using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(ridex_supply_ac, seller, ride_expend_supply, "sell " + std::to_string(ride_qty) + " ride(s)")
		).send();

	// send the ride_expend_fees from toeridexsupp to seller using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(ridex_supply_ac, seller, ride_expend_fees, "fees for sell " + std::to_string(ride_qty) + " ride(s)")
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
