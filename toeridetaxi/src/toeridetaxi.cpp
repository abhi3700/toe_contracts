#include "../include/toeridetaxi.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::addpaymost( const name& commuter_ac ) {
	// restrict this action's use only to 'ride wallet' contract
	require_auth("toe11rwallet"_n);

	// restrict this action's use to set of contract accounts
	// check(has_auth("toe11rwallet"_n) || has_auth("toe12rwallet"_n), "missing authority of either of these contracts");
	// check(has_auth("toe11rwallet"_n) && has_auth("toe12rwallet"_n), "missing authority of either of these contracts");


	// instantiate the `ridewallet` table from `ridewallet` contract
	ridewallet_index ridewallet_table("toe11rwallet"_n, commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// check if there is a non-zero balance
	check( (wallet_it->balance).amount != 0, "Sorry!, zero balance in the ride wallet.");

	// instantiate the `ride` table to read data
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// update(add/modify) the & `pay_status`
	if(ride_it == ridetaxi_table.end()) {
		ridetaxi_table.emplace(commuter_ac, [&] (auto& row) {
			row.commuter_ac = commuter_ac;
			row.pay_mode = "crypto";
			row.pay_status = "paidbycom";
		});
	} else {
		ridetaxi_table.modify(ride_it, commuter_ac, [&] (auto& row) {
			row.pay_mode = "crypto";
			row.pay_status = "paidbycom";
		});
	}
}


// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::create(
	const name& commuter_ac,
	double src_lat, 
	double src_lon, 
	double des_lat, 
	double des_lon,
	const name& vehicle_type,
	const string& pay_mode,
	float fare_est,
	asset fare_crypto_est,
	uint32_t finish_timestamp_est,
	uint32_t seat_count = 2     // define only for Pool rides. passed as default [Optional] parameter, which should be defined always as last param
	) {
	// require authority of commuter_ac
	// also checks for whether it is an eosio account or not. 
	require_auth(commuter_ac);  

	// check whether the `commuter_ac` is a verified commuter by reading the `auth` table
	user_index user_table("toe1userauth"_n, commuter_ac.value);
	auto user_it = user_table.find(commuter_ac.value);

	check( user_it != user_table.end(), "The commuter is not added in the Auth Table.");
	check( user_it->type == "commuter"_n, "The given user is not a commuter");
	check( user_it->user_status == "verify"_n, "The commuter is not verified yet.");

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
		(pay_mode == "crypto")
		|| (pay_mode == "fiatdigi")
		|| (pay_mode == "fiatcash")
		, "Sorry! The payment mode is not compatible.");

	// instantiate the `ridewallet` table.
	ridewallet_index ridewallet_table("toe11rwallet"_n, commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// if pay_mode is 'crypto', ensure the fare_amount is present in the faretaxi balance.
	if(pay_mode == "crypto") {
		// ensure that the min. ride wallet's balance has `fare_est` value
		if (
			(wallet_it->balance) < fare_crypto_est) {
			// print("Sorry! Low balance in the ride wallet.");					// only for debugging
			send_alert(commuter_ac, "Sorry! Low balance in the ride wallet.");
			return;
		}
	}

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	if(pay_mode == "crypto") {
		// modify the ride details for commuter
		ridetaxi_table.modify(ride_it, commuter_ac, [&]( auto& row ) {
			row.src_lat = src_lat;
			row.src_lon = src_lon;
			row.des_lat = des_lat;
			row.des_lon = des_lon;
			row.vehicle_type = vehicle_type;
			row.seat_count = seat_count;
			row.pay_mode = pay_mode,
			row.fare_est = fare_est;
			row.fare_crypto_est = fare_crypto_est;
			row.finish_timestamp_est = finish_timestamp_est;
		});
	} else {
		// add/update the ride details for commuter
		ridetaxi_table.emplace(commuter_ac, [&]( auto& row ) {
			row.commuter_ac = commuter_ac;
			row.src_lat = src_lat;
			row.src_lon = src_lon;
			row.des_lat = des_lat;
			row.des_lon = des_lon;
			row.vehicle_type = vehicle_type;
			row.seat_count = seat_count;
			row.pay_mode = pay_mode,
			row.fare_est = fare_est;
			row.fare_crypto_est = fare_crypto_est;
			row.finish_timestamp_est = finish_timestamp_est;
		});
	}

	// On successful execution, a receipt is sent
	send_receipt(commuter_ac, commuter_ac.to_string() + " requested a ride.");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::assign( const name& driver_ac, 
				const name& commuter_ac,
				uint32_t reachsrc_timestamp_est ) {
	require_auth(driver_ac);

	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	user_index user_table("toe1userauth"_n, driver_ac.value);
	auto user_it = user_table.find(driver_ac.value);

	check( user_it != user_table.end(), "The driver is not added in the Auth Table.");
	check( user_it->type == "driver"_n, "The given user is not a driver");
	check( user_it->user_status == "verify"_n, "The driver is not verified yet.");

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// Ensure that there is a ride by `commuter_ac`
	check( ride_it != ridetaxi_table.end(), "Ride by the " + commuter_ac.to_string() + " doesn't exist.");

	ridetaxi_table.modify(ride_it, driver_ac, [&](auto& row) {
		row.driver_ac = driver_ac;
		row.assign_timestamp = now();
		row.ride_status = "enroute"_n;
		row.reachsrc_timestamp_est = reachsrc_timestamp_est;
	});

	// On successful execution, an alert is sent
	send_alert(driver_ac, driver_ac.to_string() + " is assigned with a ride.");
	send_alert(commuter_ac, commuter_ac.to_string() + " is assigned with a driver.");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::cancelbycom( const name& commuter_ac ) {
	require_auth(commuter_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	check( ride_it != ridetaxi_table.end(), "Ride by the commuter doesn't exist."); 
	ridetaxi_table.erase(ride_it);

	// On successful execution, an alert is sent
	send_receipt(commuter_ac, commuter_ac.to_string() + " cancels the ride.");
	send_alert(ride_it->driver_ac, commuter_ac.to_string() + " cancels the ride.");
}


// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::cancelbydri( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	check( ride_it != driver_idx.end(), "Ride by the driver doesn't exist.");   
	driver_idx.erase(ride_it);

	// On successful execution, a receipt is sent
	send_receipt(driver_ac, driver_ac.to_string() + " cancels the ride.");
	// On successful execution, an alert is sent
	send_alert(ride_it->commuter_ac, driver_ac.to_string() + " cancels the ride.");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::changedes( const name& commuter_ac,
					double des_lat, 
					double des_lon,
					float fare_est,
					asset fare_crypto_est,
					const string& pay_mode ) {
	require_auth(commuter_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// ensure that the ride by the commuter exists
	check(ride_it != ridetaxi_table.end(), "Sorry! No ride created by the commuter.");

	// check the `pay_mode` as crypto/fiatdigi/fiatcash
	check(
		(pay_mode == "crypto")
		|| (pay_mode == "fiatdigi")
		|| (pay_mode == "fiatcash")
		, "Sorry! The payment mode is not compatible.");

	// instantiate the `ridewallet` table from `ridewallet` contract
	ridewallet_index ridewallet_table("toe11rwallet"_n, commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// if pay_mode is 'crypto', ensure the fare_amount is present in the faretaxi balance.
	if(pay_mode == "crypto") {
		// ensure that the min. ride wallet's balance has `fare_crypto_est` value
		if ((wallet_it->balance) < fare_crypto_est) {
			// print("Sorry! Low balance in the ride wallet.");						// only for debugging
			send_alert(commuter_ac, "Sorry! Low balance in the ride wallet.");
			return;
		}
	}

	ridetaxi_table.modify(ride_it, commuter_ac, [&](auto& row) {
		row.des_lat = des_lat;
		row.des_lon = des_lon;
		row.fare_est = fare_est;
		row.fare_crypto_est = fare_crypto_est;
		row.pay_mode = pay_mode;
	});

	// On successful execution, an alert is sent
	send_receipt(commuter_ac, commuter_ac.to_string() + " changes the destination location to " + std::to_string(des_lat) + ", " + std::to_string(des_lon)
									+ " and the fare is updated to " + std::to_string(fare_est) + " (in INR) & " + fare_crypto_est.to_string() + " (in TOE).");
	send_alert(ride_it->driver_ac, commuter_ac.to_string() + " changes the destination location to " + std::to_string(des_lat) + ", " + std::to_string(des_lon)
									+ " and the fare is updated to " + std::to_string(fare_est) + " (in INR) & " + fare_crypto_est.to_string() + " (in TOE).");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::reachsrc( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());

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
void toeridetaxi::start( const name& driver_ac/*, const name& commuter_ac*/ ) {
	require_auth( driver_ac ); 

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());

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

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure ride assigned to driver exist 
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());

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
								asset fare_crypto_act) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());

	// modify
	driver_idx.modify(ride_it, driver_ac, [&] (auto& row) {
		row.fare_act = fare_act;
		row.fare_crypto_act = fare_crypto_act;
	});

	// On successful execution, an alert is sent
	send_receipt(driver_ac, driver_ac.to_string() + " adds the actual fare in INR & TOE");
	send_alert(ride_it->commuter_ac, 
				"Now " + (ride_it->commuter_ac).to_string() + " has to pay " + 
									std::to_string(fare_act) + " (in INR) & " + 
									fare_crypto_act.to_string() + " (in TOE)."
									);
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::recvfare( const name& driver_ac ) {
	require_auth( driver_ac );

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure ride assigned to driver exist 
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + driver_ac.to_string());

	// check if the ride by driver is finished
	check( ride_it->ride_status == "complete"_n, "Sorry! The ride is not completed yet.");

	// check if the pay_mode is `crypto`
	check( ride_it->pay_mode == "crypto", "Sorry! the payment mode opted by commuter is not crypto.");

/*  check if there is any balance & it is greater than the 'fare_crypto_act'
	corresponding to the ride
*/  // instantiate the `ridewallet` table
	ridewallet_index ridewallet_table("toe11rwallet"_n, (ride_it->commuter_ac).value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// check if balance < fare_act, then return
	if( wallet_it->balance < ride_it->fare_crypto_act ) {
		// print("Sorry, the commuter doesn't have sufficient balance in the ride wallet.");		// only for debugging
		send_alert(ride_it->commuter_ac, "Sorry, the commuter doesn't have sufficient balance in the ride wallet.");
		return;
	}

	// send the fare to the driver using inline action
	action(
		permission_level{get_self(), "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::make_tuple(get_self(), driver_ac, ride_it->fare_crypto_act, "fare sent to " + driver_ac.to_string())
		).send();

	// change the pay_status to `paid`
	driver_idx.modify( ride_it, driver_ac, [&](auto& row){
		row.pay_status = "paidtodri";
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

	// check driver must be a verified user
	// check whether the `driver_ac` is a verified driver by reading the `auth` table
	user_index user_table("toe1userauth"_n, driver_ac.value);
	auto user_it = user_table.find(driver_ac.value);

	check( user_it != user_table.end(), "The driver is not added in the Auth Table.");
	check( user_it->type == "driver"_n, "The given user is not a driver");
	check( user_it->user_status == "verify"_n, "The driver is not verified yet.");

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
void toeridetaxi::eraseride( const name& commuter_ac ) {
	require_auth( get_self() );

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// ensure there is a ride by commuter_ac
	check( ride_it != ridetaxi_table.end(), "Sorry! there is no ride created by commuter_ac.");

	// erase the ride
	ridetaxi_table.erase( ride_it );
}