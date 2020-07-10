ACTION toetaxiride::creatify(
	const name& commuter_ac,
	double src_lat, 
	double src_lon, 
	double des_lat, 
	double des_lon,
	string vehicle_type 
	) {
	// require authority of commuter_ac
	// also checks for whether it is an eosio account or not. 
	require_auth(commuter_ac);	

	// @TODO: check whether the `commuter_ac` is a verified commuter by reading the `auth` table

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	// add/update the ride details for commuter
	if(ride_it == ridetaxi_table.end()) {
		ridetaxi_table.emplace(commuter_ac, [&]( auto& row ) {
			row.commuter_ac = commuter_ac;
			row.src_lat = src_lat;
			row.src_lon = src_lon;
			row.des_lat = des_lat;
			row.des_lon = des_lon;
			row.vehicle_type = vehicle_type;
		})
	} else {
		ridetaxi_table.emplace(it, commuter_ac, [&] (auto& row) {
			row.src_lat = src_lat;
			row.src_lon = src_lon;
			row.des_lat = des_lat;
			row.des_lon = des_lon;
			row.vehicle_type = vehicle_type;
		})
	}

	send_alert(commuter_ac, " requested a ride.");

}

// --------------------------------------------------------------------------------------------------------------------
ACTION toetaxiride::cancelbycom( const name& commuter_ac ) {
	require_auth(commuter_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

	check( ride_it != ridetaxi_table.end(), "Ride by the commuter doesn't exist.");	
	ridetaxi_table.erase(ride_it);
}


// --------------------------------------------------------------------------------------------------------------------
ACTION toetaxiride::cancelbydri( const name& driver_ac ) {
	require_auth(driver_ac);

	// instantiate the `ride` table
	ridetaxi_index ridetaxi_table(get_self(), "taxi"_n.value);
	auto ride_it = ridetaxi_table.find(driver_ac.value);

	check( ride_it != ridetaxi_table.end(), "Ride by the driver doesn't exist.");	
	ridetaxi_table.erase(ride_it);
}



// --------------------------------------------------------------------------------------------------------------------
ACTION toetaxiride::sendfare(
	name commuter_ac, 
	name contract_ac, 
	asset quantity, 
	string memo
	) {

	// check for conditions if either or both of these are true for `from` & `to`
	if(contract_ac != get_self() ||  commuter_ac == get_self()) {		// atleast one of the condition is true
		print("Either money is not sent to the contract")
		return;
	}


	/*
		@TODO
		- create the `toetoken` contract
	*/ 
	// if symbol doesn't match then, return the money back. So, fire an inline action to external contract for transfer back to commuter_ac
	// Check for token symbol to be 'TOE'
	// check( quantity.symbol == ride_token_symbol, "The token transferred is different");
	if(quantity.symbol != ride_token_symbol) {
		action(
			permission_level{get_self(), "active"_n},
			"toe1111token"_n,
			"transfer",
			std::make_tuple(get_self(), commuter_ac, quantity, "Return money as the symbol is different.")
			).send();
		return;
	}

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

	// update(add/modify) the `pay_mode` & `pay_status`
	if(ride_it == ridetaxi_table.end()) {
		ridetaxi_table.emplace(commuter_ac, [&] (auto& row) {
			row.pay_mode = "crypto";
			row.pay_status = "preride";
		});
	} else {
		ridetaxi_table.modify(it, commuter_ac, [&] (auto& row) {
			row.pay_mode = "crypto";
			row.pay_status = "preride";
		});
	}

}

// --------------------------------------------------------------------------------------------------------------------
ACTION toetaxiride::sendalert(
	const name& user,
	const string& message
	) {
	require_auth(get_self());

	require_receipient(user);
}

void toetaxiride::send_alert(const name& user, const string& message) {
	action(
		permission_level(get_self(), "active"_n),
		get_self(),
		"sendalert"_n,
		std::make_tuple(user, name{user}.to_string() + message)
		).send();
}
