void cabeosride::sendfare(
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


	// @TODO - if symbol doesn't match then, return the money back. So, fire an inline action to external contract for transfer back to commuter_ac
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
	fareamount_index fareamount_table(get_self(), commuter_ac.value);
	auto fare_it = fareamount_table.find(ride_token_symbol.raw());

	// update (add/modify) the balance
	if(fare_it == fareamount_table.end()) {
		fareamount_table.emplace(get_self(), [&](auto& row) {
			row.balance = quantity;
		});
	} else {
		fareamount_table.modify(fare_it, get_self(), [&](auto& row) {
			row.balance += quantity;
		});
	}

	// instantiate the `ride` table
	ride_index ride_table(get_self(), get_self().value);
	auto ride_it = ride_table.find(commuter_ac.value);

	// update(add/modify) the `pay_mode` & `pay_status`
	if(ride_it == ride_table.end()) {
		ride_table.emplace(commuter_ac, [&] (auto& row) {
			row.pay_mode = "crypto";
			row.pay_status = "preride";
		});
	} else {
		ride_table.modify(it, commuter_ac, [&] (auto& row) {
			row.pay_mode = "crypto";
			row.pay_status = "preride";
		});
	}

}
