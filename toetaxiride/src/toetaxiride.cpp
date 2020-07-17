#include "../include/toetaxiride.hpp"

void toetaxiride::create(
	const name& commuter_ac,
	double src_lat, 
	double src_lon, 
	double des_lat, 
	double des_lon,
	const string& vehicle_type,
	const string& pay_mode,
	double fare_est,
	uint32_t finish_timestamp_est,
	uint32_t seat_count = 2		// define only for Pool rides. passed as default [Optional] parameter, which should be defined always as last param
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
		(vehicle_type == "TOEX")
		|| (vehicle_type == "TOEXL")
		// || (vehicle_type == "TOEPool")
		|| (vehicle_type == "TOESUV")
		|| (vehicle_type == "TOEBlack")
		|| (vehicle_type == "TOESelect")
		// || (vehicle_type == "TOEExpressPool")
		, "Sorry! The vehicle type is not available with us.");


	// check the `pay_mode` as crypto/fiatdigi/fiatcash
	check(
		(pay_mode == "crypto")
		|| (pay_mode == "fiatdigi")
		|| (pay_mode == "fiatcash")
		, "Sorry! The payment mode is not compatible.");

	// if pay_mode is 'crypto', ensure the fare_amount is present in the faretaxi balance.
	if(pay_mode == "crypto") {
		faretaxi_index faretaxi_table(get_self(), commuter_ac.value);
		auto fare_it = faretaxi_table.find(ride_token_symbol.raw());

		// ensure that the min. ride wallet's balance has `fare_est` value
		if ((fare_it->balance).amount < fare_est) {
			print("Sorry! Low balance in the ride wallet.");
			return;
		}
	}

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// ensure the ride is created for first time i.e. the ride by commuter doesn't exist.
	check( ride_it == ridetaxi_table.end(), "A ride is already created by " + name{commuter_ac}.to_string() + ". Please use \'changedes\' void to change dest. location");

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
		row.finish_timestamp_est = finish_timestamp_est;
	});

	// On successful execution, an alert is sent
	send_alert(commuter_ac, name{commuter_ac}.to_string() + " requested a ride.");

}

// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::assign( const name& driver_ac, 
				const name& commuter_ac ) {
	require_auth(driver_ac);

	// @TODO: check whether the `driver_ac` is a verified driver by reading the `auth` table

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// Ensure that there is a ride by `commuter_ac`
	check( ride_it != ridetaxi_table.end(), "Ride by the " + name{commuter_ac}.to_string() + " doesn't exist.");

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
void toetaxiride::cancelbycom( const name& commuter_ac ) {
	require_auth(commuter_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	check( ride_it != ridetaxi_table.end(), "Ride by the commuter doesn't exist.");	
	ridetaxi_table.erase(ride_it);

	// On successful execution, an alert is sent
	send_alert(commuter_ac, name{commuter_ac}.to_string() + " cancels the ride.");
}


// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::cancelbydri( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(driver_ac.value);

	check( ride_it != ridetaxi_table.end(), "Ride by the driver doesn't exist.");	
	ridetaxi_table.erase(ride_it);

	// On successful execution, an alert is sent
	send_alert(driver_ac, name{driver_ac}.to_string() + " cancels the ride.");
}

// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::changedes( const name& commuter_ac,
					double des_lat, 
					double des_lon,
					double fare_est,
					const string& pay_mode ) {
	require_auth(commuter_ac);

	// @TODO: check whether the `commuter_ac` is a verified commuter by reading the `auth` table

	// check the `pay_mode` as crypto/fiatdigi/fiatcash
	check(
		(pay_mode == "crypto")
		|| (pay_mode == "fiatdigi")
		|| (pay_mode == "fiatcash")
		, "Sorry! The payment mode is not compatible.");

	// if pay_mode is 'crypto', ensure the fare_amount is present in the faretaxi balance.
	if(pay_mode == "crypto") {
		faretaxi_index faretaxi_table(get_self(), commuter_ac.value);
		auto fare_it = faretaxi_table.find(ride_token_symbol.raw());

		// ensure that the min. ride wallet's balance has `fare_est` value
		if ((fare_it->balance).amount < fare_est) {
			print("Sorry! Low balance in the ride wallet.");
			return;
		}
	}

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// ensure that the ride by the commuter exists
	check(ride_it != ridetaxi_table.end(), "Sorry! No ride created by the commuter.");
	ridetaxi_table.modify(ride_it, commuter_ac, [&](auto& row) {
		row.des_lat = des_lat;
		row.des_lon = des_lon;
		row.fare_est = fare_est;
		row.pay_mode = pay_mode;
	});

	// On successful execution, an alert is sent
	send_alert(commuter_ac, name{commuter_ac}.to_string() + " changes the destination location.");

}

// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::reachsrc( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// modify
	driver_idx.modify(ride_it, driver_ac, [&](auto& row) {
		row.ride_status = "waiting"_n;
		row.reachsrc_timestamp = now();

	});

	// On successful execution, an alert is sent
	send_alert(ride_it->commuter_ac, 
		"Hello, " + name{ride_it->commuter_ac}.to_string() + ", your driver: " + name{driver_ac}.to_string() + " has reached the pick-up point.");

}

// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::start( const name& driver_ac/*, const name& commuter_ac*/ ) {
	require_auth( driver_ac ); 

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
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
	send_alert(driver_ac, name{driver_ac}.to_string() + " starts the ride.");
}

// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::finish( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
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
void toetaxiride::addfareact( const name& driver_ac,
					double fare_act) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure that there is a ride by a commuter.
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// modify
	driver_idx.modify(ride_it, driver_ac, [&] (auto& row) {
		row.fare_act = fare_act;
	});

	// On successful execution, an alert is sent
	send_alert(driver_ac, name{driver_ac}.to_string() + " adds the actual fare");
	send_alert(ride_it->commuter_ac, "Now " + name{ride_it->commuter_ac}.to_string() + " has to pay " + std::to_string(fare_act) + " (INR).");
}

// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::sendfare(
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
	// Check for token symbol to be 'TOE'
	check( quantity.symbol == ride_token_symbol, "The token transferred is different");

	// instantiate the `fareamount` table
	faretaxi_index faretaxi_table(get_self(), commuter_ac.value);
	auto fare_it = faretaxi_table.find(ride_token_symbol.raw());

	// update (add/modify) the balance
	if(fare_it == faretaxi_table.end()) {
		faretaxi_table.emplace(get_self(), [&](auto& row) {
			row.balance = quantity;
		});
	} else {
		faretaxi_table.modify(fare_it, get_self(), [&](auto& row) {
			row.balance += quantity;
		});
	}

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
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

	// On successful execution, an alert is sent
	send_alert(
		commuter_ac, 
		name{commuter_ac}.to_string() + " transfers an amount of " 
		+ quantity.to_string() + " to the contract -> " + name(contract_ac).to_string() );

}


void toetaxiride::recvfare( const name& driver_ac ) {
	require_auth( driver_ac );

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto driver_idx = ridetaxi_table.get_index<"bydriver"_n>();
	auto ride_it = driver_idx.find(driver_ac.value);

	// ensure ride assigned to driver exist 
	check( ride_it != driver_idx.end(), "Sorry! no ride is assigned to " + name{driver_ac}.to_string());

	// check if the ride by driver is finished
	check( ride_it->ride_status == "complete"_n, "Sorry! The ride is not completed yet.");

/*	check if there is any balance & it is greater than the 'fare_act'
	corresponding to the ride
*/	// instantiate the `fareamount` table
	faretaxi_index faretaxi_table(get_self(), (ride_it->commuter_ac).value);
	auto fare_it = faretaxi_table.find(ride_token_symbol.raw());

	// check if balance is >= fare_act
	if((fare_it->balance).amount < ride_it->fare_act) {
		print("Sorry, the commuter doesn't have sufficient balance in the ride wallet.");
		return;
	}

	// TODO: convert the market price of fare (calculated in fiat) into 'TOE'.
	// Assume 1 TOE = 5 USD = 375 INR
	int64_t fareamount_in_toe = (ride_it->fare_act)/375.00;		// convert 'INR' to 'TOE'
	auto fare_toe = asset(fareamount_in_toe, symbol("TOE", 4));		// create a asset variable for converted fare (in TOE)

	// send the fare to the driver using inline action
	action(
		permission_level{get_self(), "active"_n},
		"toe1111token"_n,
		"transfer"_n,
		std::tuple(get_self(), driver_ac, fare_toe, "fare sent to " + name{driver_ac}.to_string())
		).send();

	// change the pay_status to `paid`
	driver_idx.modify( ride_it, driver_ac, [&](auto& row){
		row.pay_status = "paidbydri";
	});

	// erase the `fareamount` record only if the balance amount is zero
	if( (fare_it->balance).amount == 0 ) {		// @TODO: Test for whether asset balance is 0.0000 or 0
		faretaxi_table.erase(fare_it);

		// On successful execution, an alert is sent
		send_alert(driver_ac, name{driver_ac}.to_string() + " receives the actual fare");
	} else {
		send_alert(ride_it->commuter_ac, 
					"Now " + name{ride_it->commuter_ac}.to_string() + " has a balance of " + (fare_it->balance).to_string());
	}


}


// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::sendalert(
	const name& user,
	const string& message
	) {
	require_auth(get_self());

	require_recipient(user);
}

void toetaxiride::send_alert(const name& user, const string& message) {
	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendalert"_n,
		std::make_tuple(user, message)
		).send();
}

// --------------------------------------------------------------------------------------------------------------------
void toetaxiride::eraseride( const name& commuter_ac ) {
	require_auth( get_self() );

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// ensure there is a ride by commuter_ac
	check( ride_it != ridetaxi_table.end(), "Sorry! there is no ride created by commuter_ac.");

	// erase the ride
	ridetaxi_table.erase( ride_it );
}