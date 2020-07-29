#include "../include/toeridex.hpp"

// --------------------------------------------------------------------------------
void toeridex::initridex( const name& toe_issuer,
					const name& type,
					const asset& toe_qty,
					uint64_t ride_qty ) 
{
	// require the authority of toe_owner ac - "bhubtoeindia"
	require_auth(toe_issuer);

	// TODO: check if toe_issuer is the one from the token contract's stats table.

	// check the type is "driver" or "commuter"
	check( (type == "driver"_n) || (type == "commuter"_n), "invalid type");

	// check the amount is non-zero
	check( toe_qty.amount > 0, "the amount must be positive.");

	// check the symbol is 'TOE'
	check(toe_qty.symbol == ride_token_symbol, "The symbol is different than \'TOE\'");

	// check the ride_qty is non-zero
	check(ride_qty != 0, "Ride quantity can't be zero");

	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(type.value);

	check(ridex_it == ridex_table.end(), "The values for this type is already initialized.");

	// send the toe_qty from issuer to toeridexsupp using inline action
	action(
		permission_level{toe_issuer, "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::make_tuple(toe_issuer, "toeridexsupp"_n, toe_qty, "transfer initial toe_qty")
		).send();

	// modify ridex_table
	ridex_table.emplace(get_self(), [&](auto& row) {
		row.type = type;
		row.ride_quota = ride_qty;
		row.toe_balance = toe_qty;
	});

	// On successful execution, a receipt is sent
	send_receipt( toe_issuer, "initialized RIDEX with " + toe_qty.to_string() + " TOE " + 
								" & " + std::to_string(ride_qty) + " RIDE");

}

void toeridex::buyride( const name& buyer,
				const name& type,
				uint64_t ride_qty,
				const string& memo )
{
	require_auth( buyer );

	// check the buyer is enlisted in the `toeuserauth` contract table
	user_index user_table("toe1userauth"_n, buyer.value);
	auto user_it = user_table.find(buyer.value);

	check(user_it != user_table.end(), "Sorry! The buyer is not registered with us.");
	check(user_it->user_status == "verified"_n, "Sorry! The buyer is not yet verified.");

	// check the type is "driver" or "commuter"
	check( (type == "driver"_n) || (type == "commuter"_n), "invalid type");

	// check the ride_qty is non-zero
	check(ride_qty != 0, "Ride quantity can't be zero");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// check if the row exist for given type
	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(type.value);
	check(ridex_it != ridex_table.end(), "There is no data found for this type.");

	// initialized the ride_price
	auto ride_price = asset(0.0000, ride_token_symbol);

	// calc ride_price amount using the Bancor formula
	ride_price.amount = (ride_qty * (ridex_it->toe_balance.amount))/( ride_qty + (ridex_it->ride_quota) );

	// initialized the ride_price_supp
	auto ride_price_supp = asset(0.0000, ride_token_symbol);
	ride_price_supp.amount = 0.995 * (ride_price.amount);

	// initialized the ride_price_fees
	auto ride_price_fees = asset(0.0000, ride_token_symbol);
	ride_price_fees.amount = 0.005 * (ride_price.amount);

	// send the ride_price_supp from buyer to toeridexsupp using inline action
	action(
		permission_level{buyer, "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::make_tuple(buyer, "toeridexsupp"_n, ride_price_supp, "buy " + std::to_string(ride_qty) + " ride(s)")
		).send();

	// send the toe_qty from buyer to toeridexsupp using inline action
	action(
		permission_level{buyer, "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::make_tuple(buyer, "toeridexfees"_n, ride_price_fees, "fees for buy " + std::to_string(ride_qty) + " ride(s)")
		).send();

	// update the ride_table with new `ride_quota` & `toe_balance`
	ridex_table.modify(ridex_it, get_self(), [&](auto& row){
		row.ride_quota -= ride_qty;
		row.toe_balance += ride_price;
	});

	// add ride_qty to the ridexaccount table
	ridexaccount_index ridexaccount_table(get_self(), buyer.value);
	auto ridexaccount_it = ridexaccount_table.find(type.value);

	if(ridexaccount_it == ridexaccount_table.end()) {
		ridexaccount_table.emplace(buyer, [&](auto& row){
			row.type = type;
			row.rides_limit = ride_qty;
		});
	}
	else {
		ridexaccount_table.modify(ridexaccount_it, buyer, [&](auto& row){
			row.rides_limit += ride_qty;
		});
	}

}
// --------------------------------------------------------------------------------------------------------------------
void toeridex::sellride( const name& seller,
						const name& type,
						uint64_t ride_qty,
						const string& memo)
{
	require_auth( seller );

	// check the seller is enlisted in the `toeuserauth` contract table
	user_index user_table("toe1userauth"_n, seller.value);
	auto user_it = user_table.find(seller.value);

	check(user_it != user_table.end(), "Sorry! The seller is not registered with us.");
	check(user_it->user_status == "verified"_n, "Sorry! The seller is not yet verified.");

	// check the type is "driver" or "commuter"
	check( (type == "driver"_n) || (type == "commuter"_n), "invalid type");

	check(ride_qty != 0, "Ride quantity can't be zero");

	ridexaccount_index ridexaccount_table(get_self(), seller.value);
	auto ridexaccount_it = ridexaccount_table.find(type.value);
	check(ridexaccount_it != ridexaccount_table.end(), "Sorry! There is no ride to sell.");
	check(ride_qty <= ridexaccount_it->rides_limit, "The ride no. asked, is more than the limit of the seller.");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// check if the row exist for given type
	ridex_index ridex_table(get_self(), get_self().value);
	auto ridex_it = ridex_table.find(type.value);
	check(ridex_it != ridex_table.end(), "There is no data found for this type.");

	// initialized the ride_price
	auto ride_price = asset(0.0000, ride_token_symbol);

	// calc ride_price amount using the Bancor formula
	ride_price.amount = (ride_qty * (ridex_it->toe_balance.amount))/( ride_qty + (ridex_it->ride_quota) );

	// initialized the ride_price_supp
	auto ride_price_supp = asset(0.0000, ride_token_symbol);
	ride_price_supp.amount = 0.995 * (ride_price.amount);

	// initialized the ride_price_fees
	auto ride_price_fees = asset(0.0000, ride_token_symbol);
	ride_price_fees.amount = 0.005 * (ride_price.amount);

	// send the ride_price_supp from toeridexsupp to seller using inline action
	action(
		permission_level{"toeridexsupp"_n, "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::make_tuple("toeridexsupp"_n, seller, ride_price_supp, "sell " + std::to_string(ride_qty) + " ride(s)")
		).send();

	// send the ride_price_fees from toeridexsupp to seller using inline action
	action(
		permission_level{"toeridexsupp"_n, "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::make_tuple("toeridexsupp"_n, seller, ride_price_fees, "fees for sell " + std::to_string(ride_qty) + " ride(s)")
		).send();

	// update the ride_table with new `ride_quota` & `toe_balance`
	ridex_table.modify(ridex_it, get_self(), [&](auto& row){
		row.ride_quota += ride_qty;
		row.toe_balance -= ride_price;
	});

	// deduct ride_qty to the ridexaccount table	
	ridexaccount_table.modify(ridexaccount_it, seller, [&](auto& row){
		row.rides_limit -= ride_qty;
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
