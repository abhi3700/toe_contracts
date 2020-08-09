#include "../include/toeridetaxi.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::create(
	const name& commuter_ac,
	checksum256 src_lat_hash, 
	checksum256 src_lon_hash, 
	checksum256 des_lat_hash, 
	checksum256 des_lon_hash,
	const name& vehicle_type,
	const name& pay_mode,
	float fare_est,
	float market_price,
	const asset& fare_crypto_est,
	uint32_t finish_timestamp_est,
	uint32_t seat_count, // define only for Pool rides. passed as default [Optional] parameter, which should be defined always as last param
	const string& memo
	) {
	// require authority of commuter_ac
	// also checks for whether it is an eosio account or not. 
	require_auth(commuter_ac);  

	// check whether the `commuter_ac` is a verified commuter by reading the `auth` table
	check_userauth(commuter_ac, "commuter"_n);

	// Ensure that the `vehicle_type` is either of this list:
	/*
			- [x] TOE Auto
			- [x] TOE Moto
			- [x] TOE Go
			- [x] TOE Go Executive
			- [x] TOE Premier
			- [x] TOE Premier Executive
			- [x] TOE XL
			- [x] TOE Go Intercity
			- [x] TOE XL Intercity
	*/
	check(
		(vehicle_type == "toeauto"_n)
		|| (vehicle_type == "toemoto"_n)
		|| (vehicle_type == "toego"_n)
		|| (vehicle_type == "toegoexec"_n)
		|| (vehicle_type == "toepremier"_n)
		|| (vehicle_type == "toeselect"_n)
		|| (vehicle_type == "toepremexec"_n)
		|| (vehicle_type == "toexl"_n)
		|| (vehicle_type == "toegointcity"_n)
		|| (vehicle_type == "toexlintcity"_n)
		, "Sorry! The vehicle type is not available with us.");


	// check the `pay_mode` as crypto/fiatdigi/fiatcash
	check(
		(pay_mode == "crypto"_n)
		|| (pay_mode == "fiatdigi"_n)
		|| (pay_mode == "fiatcash"_n)
		, "Sorry! The payment mode is not compatible.");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `ridewallet` table.
	ridewallet_index ridewallet_table(wallet_contract_ac, commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	check(wallet_it != ridewallet_table.end(), "the symbol doesn't exist in the wallet. Please, transfer some balance to your wallet.");

	// if pay_mode is 'crypto', ensure the fare_amount is present in the faretaxi balance.
	if(pay_mode == "crypto"_n) {
		// ensure that the ride wallet's min. balance has `fare_est` value
		check( wallet_it->balance >= fare_crypto_est, "Sorry! Low balance in the ride wallet.");

		// if ( (wallet_it->balance) < fare_crypto_est) {
		// 	send_alert(commuter_ac, "Sorry! Low balance in the ride wallet.");
		// 	return;
		// }
	}

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// As the ride has to be created only once, that's why `check()` is applied before both 
	// `emplace()` & `modify()` during __"crypto"__ & __"non-crypto" pay_mode

	// add the ride details by commuter for __"crypto"__ pay_mode
	if(pay_mode == "crypto"_n) {
		// possibility-1: with "crypto" pay_mode trying again
		// possibility-2: with "crypto" pay_mode after trying `setfipaymost` action i.e. set "non-crypto" & then create ride using "crypto" pay_mode. So, here, create using same stored pay_mode
	 	check(ride_it == ridetaxi_table.end(), 
	 		(ride_it->pay_mode == "crypto"_n) ? "The ride looks like already set by \'"+ commuter_ac.to_string() + "\' using \'" + ride_it->pay_mode.to_string() + 
	 												"\'." : 
	 											"The ride looks like already set by \'"+ commuter_ac.to_string() + "\' using \'" + ride_it->pay_mode.to_string() + 
	 												"\'. So, please use the same pay_mode to create the ride.");
	
		ridetaxi_table.emplace(commuter_ac, [&]( auto& row ) {
			row.commuter_ac = commuter_ac;
			row.ride_status = "requested"_n;
			row.src_lat_hash = src_lat_hash;
			row.src_lon_hash = src_lon_hash;
			row.des_lat_hash = des_lat_hash;
			row.des_lon_hash = des_lon_hash;
			row.vehicle_type = vehicle_type;
			row.seat_count = seat_count;
			row.pay_mode = pay_mode,
			row.fare_est = fare_est;
			row.market_price = market_price;
			row.fare_crypto_est = fare_crypto_est;
			row.finish_timestamp_est = finish_timestamp_est;
			
			// set only for __"crypto"__ pay_mode
			row.crypto_paystatus = "paidbycom"_n;
		});
	 }
	// add the ride details by commuter for __"non-crypto"__ pay_mode
	else {
	 	check(ride_it != ridetaxi_table.end(), "First set the fiat pay status using \'setfiatpayst\' action before requesting a ride.");
	 	check(ride_it->pay_mode == pay_mode, "The pay_mode during ride request doesn't match with the one stored earlier in the table.");
		check(ride_it->ride_status != "requested"_n, "Sorry, the ride is already requested. So, can't be re-created, but destination can be changed during \'on-trip\'");

		ridetaxi_table.modify(ride_it, commuter_ac, [&]( auto& row ) {
			row.commuter_ac = commuter_ac;
			row.ride_status = "requested"_n;
			row.src_lat_hash = src_lat_hash;
			row.src_lon_hash = src_lon_hash;
			row.des_lat_hash = des_lat_hash;
			row.des_lon_hash = des_lon_hash;
			row.vehicle_type = vehicle_type;
			row.seat_count = seat_count;
			row.pay_mode = pay_mode,
			row.fare_est = fare_est;
			row.market_price = market_price;
			row.fare_crypto_est = fare_crypto_est;
			row.finish_timestamp_est = finish_timestamp_est;

			// pay_mode already set during `setfiatpayst` action. So, not needed
		});
	 } 
	

	// On successful execution, a receipt is sent
	send_receipt(commuter_ac, commuter_ac.to_string() + " requested a ride.");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::setfipaymost( const name& commuter_ac,
								const name& pay_mode,
								const string& memo )
{
	require_auth( commuter_ac );

	// check whether the `commuter_ac` is a verified commuter by reading the `auth` table
	check_userauth(commuter_ac, "commuter"_n);

	// check the pay_mode as "fiatdigi" or "fiatcash"
	check( (pay_mode == "fiatdigi"_n) || (pay_mode == "fiatcash"_n), "In order to use this action, the pay_mode must be fiatdigi or fiatcash.");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	if(ride_it == ridetaxi_table.end()) {
		ridetaxi_table.emplace(commuter_ac, [&](auto& row) {
			row.commuter_ac = commuter_ac;
			row.pay_mode = pay_mode;		// set "fiatdigi" or "fiatcash"

			if (pay_mode == "fiatdigi"_n) {
				row.fiat_paystatus = "paidbycom"_n;
			}
		});
	}
	else {
		ridetaxi_table.modify(ride_it, commuter_ac, [&](auto& row) {
			row.pay_mode = pay_mode;
			if (pay_mode == "fiatdigi"_n) {
				row.fiat_paystatus = "paidbycom"_n;
			}
		});
	}

	// On successful execution, a receipt is sent
	if(pay_mode == "fiatdigi"_n) {
		send_receipt(commuter_ac, " You set the ride's pay_mode as: \'" + pay_mode.to_string() + "\' & pay_status as \'" + ride_it->fiat_paystatus.to_string() + "\'.");
	}
	else {		// __"fiatcash"__ pay_mode
		send_receipt(commuter_ac, " You set the ride's pay_mode as: \'" + pay_mode.to_string() + "\'.");
	}

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::assign( const name& driver_ac, 
							const name& commuter_ac,
							uint32_t reachsrc_timestamp_est ) {
	require_auth(get_self());

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	// check if the driver is online
	check_dridestatus(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// Ensure that there is a ride by `commuter_ac`
	check( ride_it != ridetaxi_table.end(), "Ride by the " + commuter_ac.to_string() + " doesn't exist.");

	// Ensure that the ride_status is not marked as "enroute"
	check( ride_it->ride_status != "enroute"_n, "the commuter is already assigned with a driver: " + driver_ac.to_string() + ". So, the ride can't be reassigned");

	ridetaxi_table.modify(ride_it, get_self(), [&](auto& row) {
		row.driver_ac = driver_ac;
		row.assign_timestamp = now();
		row.ride_status = "enroute"_n;
		row.reachsrc_timestamp_est = reachsrc_timestamp_est;
	});

	// On successful execution, an alert is sent
	send_alert(driver_ac, driver_ac.to_string() + " is assigned with a ride.");
	send_alert(commuter_ac, commuter_ac.to_string() + " is assigned with a driver: " + driver_ac.to_string());

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::cancelbycom( const name& commuter_ac,
								const string& memo ) {
	require_auth(commuter_ac);

	// check whether the `commuter_ac` is a verified commuter by reading the `auth` table
	check_userauth(commuter_ac, "commuter"_n);

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	check( ride_it != ridetaxi_table.end(), "Ride by the commuter doesn't exist."); 
	check( 
		(ride_it->ride_status == "requested"_n) || 
		(ride_it->ride_status == "enroute"_n) || 
		(ride_it->ride_status == "waiting"_n)
		, "The ride status must be either \'requested\' or \'enroute\' or \'waiting\' in order to cancel ride by commuter.");

	ridetaxi_table.erase(ride_it);

	// On successful execution, an alert is sent
	send_receipt(commuter_ac, commuter_ac.to_string() + " cancels the ride.");
	send_alert(ride_it->driver_ac, commuter_ac.to_string() + " cancels the ride.");
}


// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::cancelbydri( const name& driver_ac,
								const string& memo ) {
	require_auth(driver_ac);

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	check( ride_it != driver_idx.end(), "Ride by the driver doesn't exist.");   
	check( 
		(ride_it->ride_status == "requested"_n) || 
		(ride_it->ride_status == "enroute"_n) || 
		(ride_it->ride_status == "waiting"_n)
		, "The ride status must be either \'requested\' or \'enroute\' or \'waiting\' in order to cancel ride by driver.");

	driver_idx.erase(ride_it);

	// On successful execution, a receipt is sent
	send_receipt(driver_ac, driver_ac.to_string() + " cancels the ride.");
	// On successful execution, an alert is sent
	send_alert(ride_it->commuter_ac, driver_ac.to_string() + " cancels the ride.");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::changedes( const name& commuter_ac,
					checksum256 des_lat_hash, 
					checksum256 des_lon_hash,
					float fare_est,
					const asset& fare_crypto_est,
					const name& pay_mode,
					const string& memo ) {
	require_auth(commuter_ac);

	// check whether the `commuter_ac` is a verified commuter by reading the `auth` table
	check_userauth(commuter_ac, "commuter"_n);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// ensure the action is accessed during __"ontrip"__ ride_status
	check(ride_it->ride_status == "ontrip"_n, "The ride status must be \'ontrip\' in order to use this action.");

	// ensure that the new des_lat_hash or des_lon_hash is different than it's stored counterpart 
	check(
		(ride_it->des_lat_hash != des_lat_hash) || 
		(ride_it->des_lon_hash != des_lon_hash), "Sorry, both modified latitude & longitude are same as its stored counterpart.");

	// ensure that the ride by the commuter exists
	check(ride_it != ridetaxi_table.end(), "Sorry! No ride created by the commuter.");

	// check the `pay_mode` as crypto/fiatdigi/fiatcash
	check(
		(pay_mode == "crypto"_n)
		|| (pay_mode == "fiatdigi"_n)
		|| (pay_mode == "fiatcash"_n)
		, "Sorry! The payment mode is not compatible.");

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `ridewallet` table from `ridewallet` contract
	ridewallet_index ridewallet_table(wallet_contract_ac, commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	check(wallet_it != ridewallet_table.end(), "the symbol doesn't exist in the wallet. Please, transfer some balance to your wallet.");

	// Case-1: if the pay_mode is __"crypto"__ as previous:
	// if pay_mode is 'crypto', ensure the fare_amount is present in the 'ridewallet' balance.
	if((ride_it->pay_mode == "crypto"_n) && (pay_mode == "crypto"_n)) {
		// ensure that the ride wallet's min. balance has `fare_est` value
		check( wallet_it->balance >= fare_crypto_est, "Sorry! Low balance in the ride wallet.");

		// if ( (wallet_it->balance) < fare_crypto_est) {
		// 	send_alert(commuter_ac, "Sorry! Low balance in the ride wallet.");
		// 	return;
		// }
	}

	// Case-2 & Case-4: Not considered inside Smart contract as the wallet exists outside Blockchain world.

	// Case-3: if the pay_mode is changed from __"fiatdigi"__ to __"crypto"__:
	if((ride_it->pay_mode == "fiatdigi"_n) && (pay_mode == "crypto"_n)) {
		// ensure that the ride wallet's min. balance has `fare_est` value
		check( wallet_it->balance >= fare_crypto_est, "Sorry! Low balance in the ride wallet.");

		// if ( (wallet_it->balance) < fare_crypto_est) {
		// 	send_alert(commuter_ac, "Sorry! Low balance in the ride wallet.");
		// 	return;
		// }
	}

	ridetaxi_table.modify(ride_it, commuter_ac, [&](auto& row) {
		row.des_lat_hash = des_lat_hash;
		row.des_lon_hash = des_lon_hash;
		row.fare_est = fare_est;
		row.fare_crypto_est = fare_crypto_est;
		row.pay_mode = pay_mode;

		if (pay_mode == "crypto"_n) {
			row.crypto_paystatus = "paidbycom"_n;
		}
	});

	// On successful execution, an alert is sent
	send_receipt(commuter_ac, commuter_ac.to_string() + " changes the destination location & the fare is updated to " + std::to_string(fare_est) + " (in INR) & " + fare_crypto_est.to_string() + " (in TOE).");
	send_alert(ride_it->driver_ac, commuter_ac.to_string() + " changes the destination location & the fare is updated to " + std::to_string(fare_est) + " (in INR) & " + fare_crypto_est.to_string() + " (in TOE).");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::reachsrc( const name& driver_ac ) {
	require_auth(driver_ac);

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());
	check( ride_it->ride_status != "waiting"_n, "Sorry! the ride_status is already marked as \'waiting\'. So, you can't modify.");

	// modify
	driver_idx.modify(ride_it, driver_ac, [&](auto& row) {
		row.ride_status = "waiting"_n;
		row.reachsrc_timestamp_act = now();

	});

	// On successful execution a receipt & an alert is sent
	send_receipt(driver_ac, 
		driver_ac.to_string() + " has reached the pick-up point.");
	send_alert(ride_it->commuter_ac, 
		"Hello, " + (ride_it->commuter_ac).to_string() + ", your driver: " + driver_ac.to_string() + " has reached the pick-up point.");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::start( const name& driver_ac ) {
	require_auth( driver_ac ); 

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());
	check( ride_it->ride_status != "ontrip"_n, "Sorry! the trip is already started. So, you can't modify.");

	// modify
	driver_idx.modify(ride_it, driver_ac, [&](auto& row) {
		row.ride_status = "ontrip"_n;
		row.start_timestamp = now();

	});

	// On successful execution, an alert is sent
	send_receipt(driver_ac, driver_ac.to_string() + " starts the ride.");
	send_alert(ride_it->commuter_ac, driver_ac.to_string() + " starts the ride.");
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::finish( const name& driver_ac ) {
	require_auth(driver_ac);

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure ride assigned to driver exist 
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());
	check( ride_it->ride_status != "complete"_n, "Sorry! the ride by the driver is already marked as complete. So, you can't modify.");

	// modify
	driver_idx.modify(ride_it, driver_ac, [&](auto& row) {
		row.ride_status = "complete"_n;
		row.finish_timestamp_act = now();

	});

	// On successful execution, an alert is sent
	send_alert(driver_ac, driver_ac.to_string() + " finishes the ride.");
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::addfareact( const name& driver_ac,
								float fare_act,
								const asset& fare_crypto_act) {
	require_auth(driver_ac);

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());
	check( ride_it->fare_act == 0.00, "Sorry! the actual fare is already set. You can't modify");
	check( ride_it->ride_status == "complete"_n, "The ride must be marked as complete. Otherwise, this action can't be used.");

	// modify
	driver_idx.modify(ride_it, driver_ac, [&] (auto& row) {
		row.fare_act = fare_act;
		row.fare_crypto_act = fare_crypto_act;
	});

	// On successful execution, an alert is sent
	send_receipt(driver_ac, driver_ac.to_string() + " adds the actual fare in INR & TOE");
	send_alert(ride_it->commuter_ac, 
				"Now " + (ride_it->commuter_ac).to_string() + " has to pay " + 
									std::to_string(fare_act) + " (in INR) or " + 
									fare_crypto_act.to_string() + " (in TOE)."
									);
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::recvfare( const name& driver_ac ) {
	require_auth( driver_ac );

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure ride assigned to driver exist 
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());

	// check if the ride by driver is finished
	check( ride_it->ride_status == "complete"_n, "Sorry! The ride is not completed yet.");

	// check if the pay_mode is `crypto`
	check( ride_it->pay_mode == "crypto"_n, "Sorry! the payment mode opted by commuter is not crypto.");

/*  check if there is any balance & it is greater than the 'fare_crypto_act'
	corresponding to the ride
*/  // instantiate the `ridewallet` table
	ridewallet_index ridewallet_table(wallet_contract_ac, (ride_it->commuter_ac).value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	check(wallet_it != ridewallet_table.end(), "the symbol doesn't exist in the wallet. Please, transfer some balance to your wallet.");

	// check if balance < fare_act, then return
	if( wallet_it->balance < ride_it->fare_crypto_act ) {
		// print("Sorry, the commuter doesn't have sufficient balance in the ride wallet.");		// only for debugging
		send_alert(ride_it->commuter_ac, "Sorry, the commuter doesn't have sufficient balance in the ride wallet.");
		return;
	}

	// send the fare to the driver using inline action
	action(
		permission_level{get_self(), "active"_n},
		token_contract_ac,
		"transfer"_n,
		std::make_tuple(get_self(), driver_ac, ride_it->fare_crypto_act, "fare sent to " + driver_ac.to_string())
		).send();

	// change the crypto pay status to `paid`
	driver_idx.modify( ride_it, driver_ac, [&](auto& row){
		row.crypto_paystatus = "paidtodri"_n;
	});

	// erase the `fareamount` record only if the balance amount is zero
	if( (wallet_it->balance).amount == 0 ) {        // @TODO: Test for whether asset balance is 0.0000 or 0
		ridewallet_table.erase(wallet_it);

		// On successful execution, an alert is sent
		send_alert(driver_ac, driver_ac.to_string() + " receives the actual fare");
	} else {
		send_alert(driver_ac, driver_ac.to_string() + " receives the actual fare");       // even if non-zero balance, then also the fare might have been transferred.
		send_alert(ride_it->commuter_ac, 
					"Now " + (ride_it->commuter_ac).to_string() + " has a balance of " + (wallet_it->balance).to_string());
	}


}

void toeridetaxi::addristatus( const name& driver_ac,
								const name& status )
{
	require_auth(driver_ac);

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	check_userauth(driver_ac, "driver"_n);

	// check the status is either online/offline
	check( (status == "online"_n) || (status == "offline"_n), "status must be either online/offline.");

	dridestatus_index dridestatus_table(get_self(), driver_ac.value);
	auto dridestatus_it = dridestatus_table.find(status.value);

	if(dridestatus_it == dridestatus_table.end()) {
		dridestatus_table.emplace(driver_ac, [&](auto& row){
			row.status = status;
		});
	} else{
		dridestatus_table.modify(dridestatus_it, driver_ac, [&](auto& row){
			row.status = status;
		});
	}
}


// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::sendalert(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridetaxi::send_alert(const name& user, 
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
void toeridetaxi::sendreceipt(const name& user,
							const string& message) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridetaxi::send_receipt(const name& user, 
								const string& message) {
	check(message.size() <= 256, "message has more than 256 bytes");

	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendreceipt"_n,
		std::make_tuple(user, message)
		).send();
}


// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::eraseride( const name& commuter_ac,
								const string& memo ) {
	require_auth( get_self() );

	check(memo.size() <= 256, "memo has more than 256 bytes");

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// ensure there is a ride by commuter_ac
	check( ride_it != ridetaxi_table.end(), "Sorry! there is no ride created by commuter_ac.");

	// erase the ride
	ridetaxi_table.erase( ride_it );
}