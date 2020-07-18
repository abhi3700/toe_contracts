void toeridetaxi::sendfare(
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

	// instantiate the `fareamount` table
	ridewallet_index ridewallet_table(get_self(), commuter_ac.value);
	auto fare_it = ridewallet_table.find(ride_token_symbol.raw());

	// update (add/modify) the balance
	if(fare_it == ridewallet_table.end()) {
		ridewallet_table.emplace(get_self(), [&](auto& row) {
			row.balance = quantity;
		});
	} else {
		ridewallet_table.modify(fare_it, get_self(), [&](auto& row) {
			row.balance += quantity;
		});
	}

	// instantiate the `ride` table to read data
	ridetaxi_index ridetaxi_table("toe1111rtaxi"_n, "toe1111rtaxi"_n.value);
	auto ride_it = ridetaxi_table.find(commuter_ac.value);

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
	send_alert(
		commuter_ac, 
		name{commuter_ac}.to_string() + " transfers an amount of " 
		+ quantity.to_string() + " to the contract -> " + name(contract_ac).to_string() );

}
