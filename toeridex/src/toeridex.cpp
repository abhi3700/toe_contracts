#include "../include/toeridex.hpp"

// --------------------------------------------------------------------------------
void toeridex::initridex(
						// const name& token_issuer, 		// doesn't matter whether you give this argument
						const name& ride_type,
						const asset& toe_qty,
						uint64_t ride_qty ) 
{
	// require the authority of toe_owner ac - "bhubtoeindia"
	require_auth(token_issuer);
	// require_auth(get_self());

	// check if token_issuer is the one from the token contract's stats table.
	stats_index stats_table(token_contract_ac, ride_token_symbol.code().raw());
	auto stats_it = stats_table.find(ride_token_symbol.code().raw());

	check(stats_it != stats_table.end(), "the token symbol doesn't exist");
	check(stats_it->issuer == token_issuer, "The contract initialized toeken issuer doesn't match with stats table's issuer.");

	// check the ride_type is "driver" or "commuter"
	check( (ride_type == "driver"_n) || (ride_type == "commuter"_n), "invalid ride_type");

	// check fareamount is valid for all conditions as 'asset'
	check_quantity(toe_qty);

	// check the ride_qty is non-zero
	check(ride_qty != 0, "Ride quantity can't be zero");

	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(ride_type.value);

	check(ridex_it == ridex_table.end(), "The values for this ride_type is already initialized.");

	// check the balance of 'token_issuer' greater than drawn amount 'toe_qty'
	accounts_index tokenissuer_table(token_contract_ac, token_issuer.value);
	const auto& from = tokenissuer_table.get(toe_qty.symbol.code().raw(), "no balance object found" );
	check( from.balance.amount >= toe_qty.amount, "overdrawn balance" );

	// send the toe_qty from 'issuer' to 'ridex_supply_ac' using inline action
	action(
		permission_level{token_issuer, "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(token_issuer, ridex_supply_ac, toe_qty, std::string("transfer initial toe quantity"))
	).send();

/*	// modify ridex_table
	ridex_table.emplace(get_self(), [&](auto& row) {
		row.ride_type = ride_type;
		row.ride_quota = ride_qty;
		row.toe_balance = toe_qty;
	});

	// On successful execution, a receipt is sent
	send_receipt( token_issuer, "initialized RIDEX with " + toe_qty.to_string() + " & " + std::to_string(ride_qty) + " RIDE");
*/
}

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

	// add ride_qty to the ridexaccount table
	ridexaccount_index ridexaccount_table(get_self(), buyer.value);
	auto ridexaccount_it = ridexaccount_table.find(ride_type.value);

	if(ridexaccount_it == ridexaccount_table.end()) {
		ridexaccount_table.emplace(buyer, [&](auto& row){
			row.ride_type = ride_type;
			row.rides_limit = ride_qty;
		});
	}
	else {
		ridexaccount_table.modify(ridexaccount_it, buyer, [&](auto& row){
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

	ridexaccount_index ridexaccount_table(get_self(), seller.value);
	auto ridexaccount_it = ridexaccount_table.find(ride_type.value);
	check(ridexaccount_it != ridexaccount_table.end(), "Sorry! There is no ride to sell.");
	check(ride_qty <= ridexaccount_it->rides_limit, "The ride no. asked, is more than the limit of the seller.");

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
	ridexaccount_table.modify(ridexaccount_it, seller, [&](auto& row){
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
