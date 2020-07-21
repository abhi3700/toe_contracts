#include "../include/toeridetaxi.hpp"
#include "../../toeridewallet/include/toeridewallet.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::addpay( const name& commuter_ac ) {
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
	asset fare_crypto,
	uint32_t finish_timestamp_est,
	uint32_t seat_count = 2     // define only for Pool rides. passed as default [Optional] parameter, which should be defined always as last param
	) {
	// require authority of commuter_ac
	// also checks for whether it is an eosio account or not. 
	require_auth(commuter_ac);  

	// @TODO: check whether the `commuter_ac` is a verified commuter by reading the `auth` table

	// Ensure that the `vehicle_type` is either of this list:
	/*
			- [x] TOEX
			- [x] TOEXL
			- [ ] TOEPool
			- [x] TOESUV
			- [x] TOEBlack
			- [x] TOESelect
			- [ ] TOEExpressPool
	*/
	check(
		(vehicle_type == "toex"_n)
		|| (vehicle_type == "toexl"_n)
		// || (vehicle_type == "toepool"_n)
		|| (vehicle_type == "toesuv"_n)
		|| (vehicle_type == "toeblack"_n)
		|| (vehicle_type == "toeselect"_n)
		// || (vehicle_type == "toeexprpool"_n)
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
		if ((wallet_it->balance) < fare_crypto) {
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
			row.fare_crypto = fare_crypto;
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
			row.fare_crypto = fare_crypto;
			row.finish_timestamp_est = finish_timestamp_est;
		});
	}

	// On successful execution, a receipt is sent
	send_receipt(commuter_ac, commuter_ac.to_string() + " requested a ride.");

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::assign( const name& driver_ac, 
				const name& commuter_ac ) {
	require_auth(driver_ac);

	// @TODO: check whether the `driver_ac` is a verified driver by reading the `auth` table

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// Ensure that there is a ride by `commuter_ac`
	check( ride_it != ridetaxi_table.end(), "Ride by the " + commuter_ac.to_string() + " doesn't exist.");

	ridetaxi_table.modify(ride_it, driver_ac, [&](auto& row) {
		row.driver_ac = driver_ac;
		row.assign_timestamp = now();
		row.ride_status = "enroute"_n;
	});

	// On successful execution, an alert is sent
	send_alert(driver_ac, name{driver_ac}.to_string() + " is assigned with a ride.");
	send_alert(commuter_ac, name{commuter_ac}.to_string() + " is assigned with a driver.");

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
	send_receipt(commuter_ac, name{commuter_ac}.to_string() + " cancels the ride.");
	send_alert(ride_it->driver_ac, name{commuter_ac}.to_string() + " cancels the ride.");
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
					asset fare_crypto,
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
		// ensure that the min. ride wallet's balance has `fare_est` value
		if ((wallet_it->balance) < fare_crypto) {
			// print("Sorry! Low balance in the ride wallet.");						// only for debugging
			send_alert(commuter_ac, "Sorry! Low balance in the ride wallet.");
			return;
		}
	}

	ridetaxi_table.modify(ride_it, commuter_ac, [&](auto& row) {
		row.des_lat = des_lat;
		row.des_lon = des_lon;
		row.fare_est = fare_est;
		row.fare_crypto = fare_crypto;
		row.pay_mode = pay_mode;
	});

	// On successful execution, an alert is sent
	send_receipt(commuter_ac, name{commuter_ac}.to_string() + " changes the destination location to " + std::to_string(des_lat) + ", " + std::to_string(des_lon));
	send_alert(ride_it->driver_ac, name{commuter_ac}.to_string() + " changes the destination location to " + std::to_string(des_lat) + ", " + std::to_string(des_lon));

}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::reachsrc( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// modify
	driver_idx.modify(ride_it, driver_ac, [&](auto& row) {
		row.ride_status = "waiting"_n;
		row.reachsrc_timestamp = now();

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
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// modify
	driver_idx.modify(ride_it, driver_ac, [&](auto& row) {
		row.ride_status = "ontrip"_n;
		row.start_timestamp = now();

	});

	// On successful execution, an alert is sent
	send_receipt(driver_ac, name{driver_ac}.to_string() + " starts the ride.");
	send_alert(ride_it->commuter_ac, name{driver_ac}.to_string() + " starts the ride.");
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::finish( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure ride assigned to driver exist 
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// modify
	driver_idx.modify(ride_it, driver_ac, [&](auto& row) {
		row.ride_status = "complete"_n;
		row.finish_timestamp_act = now();

	});

	// On successful execution, an alert is sent
	send_alert(driver_ac, name{driver_ac}.to_string() + " finishes the ride.");
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::addfareact( const name& driver_ac,
					float fare_act) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// modify
	driver_idx.modify(ride_it, driver_ac, [&] (auto& row) {
		row.fare_act = fare_act;
	});

	// On successful execution, an alert is sent
	send_receipt(driver_ac, name{driver_ac}.to_string() + " adds the actual fare");
	send_alert(ride_it->commuter_ac, "Now " + name{ride_it->commuter_ac}.to_string() + " has to pay " + std::to_string(fare_act) + " (INR).");
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::recvfare( const name& driver_ac ) {
	require_auth( driver_ac );

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), get_self().value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure ride assigned to driver exist 
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// check if the ride by driver is finished
	check( ride_it->ride_status == "complete"_n, "Sorry! The ride is not completed yet.");

/*  check if there is any balance & it is greater than the 'fare_act'
	corresponding to the ride
*/  // instantiate the `ridewallet` table
	ridewallet_index ridewallet_table("toe11rwallet"_n, (ride_it->commuter_ac).value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// check if balance < fare_act, then return
	if((wallet_it->balance).amount < ride_it->fare_act) {
		// print("Sorry, the commuter doesn't have sufficient balance in the ride wallet.");		// only for debugging
		send_alert(ride_it->commuter_ac, "Sorry, the commuter doesn't have sufficient balance in the ride wallet.");
		return;
	}

	// send the fare to the driver using inline action
	action(
		permission_level{get_self(), "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::tuple(get_self(), driver_ac, ride_it->fare_crypto, "fare sent to " + name{driver_ac}.to_string())
		).send();

	// change the pay_status to `paid`
	driver_idx.modify( ride_it, driver_ac, [&](auto& row){
		row.pay_status = "paidtodri";
	});

	// erase the `fareamount` record only if the balance amount is zero
	if( (wallet_it->balance).amount == 0 ) {        // @TODO: Test for whether asset balance is 0.0000 or 0
		ridewallet_table.erase(wallet_it);

		// On successful execution, an alert is sent
		send_alert(driver_ac, name{driver_ac}.to_string() + " receives the actual fare");
	} else {
		send_alert(driver_ac, name{driver_ac}.to_string() + " receives the actual fare");       // even if non-zero balance, then also the fare might have been transferred.
		send_alert(ride_it->commuter_ac, 
					"Now " + name{ride_it->commuter_ac}.to_string() + " has a balance of " + (wallet_it->balance).to_string());
	}


}


// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::sendalert(
	const name& user,
	const string& message
	) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridetaxi::send_alert(const name& user, const string& message) {
	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendalert"_n,
		std::make_tuple(user, message)
		).send();
}

// --------------------------------------------------------------------------------------------------------------------
void toeridetaxi::sendreceipt(
	const name& user,
	const string& message
	) {
	require_auth(get_self());

	require_recipient(user);
}

void toeridetaxi::send_receipt(const name& user, const string& message) {
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