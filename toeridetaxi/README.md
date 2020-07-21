# Ride contract
## Brief
* It is a contract which is for 
	- [x] request ride by commuter
	- [ ] request pool ride by commuter(s)
	- [x] assign ride to driver
	- [x] cancel ride by commuter/driver
	- [x] alerting commuter about vehicle has reached the pick-up location.
	- [x] start the ride
	- [x] finish the ride
	- [x] sending alert
	- [x] sending receipt

## About
* contract name - `toeridetaxi`
* contract's account name - `toe1111rtaxi`
* action
	- `create`
	- `assign`
	- `cancelbycom`
	- `cancelbydri`
	- `changedes`
	- `reachsrc`
	- `start`
	- `finish`
	- `addfareact`
	- `sendfare`
	- `recvfare`
* table
	- `ridetaxi`

## Table
* the table is stored in the contract acount.
* For each row, `ram_payer` would be users (driver/commuter) for their piece of data. TODO: 
	- if the ride is costing too much for their use, then additional amount of CPU, NET will be added to their account during account creation.
* Table:
	- `ride`
		+ `taxi`
		+ `bike`





Ride actions
--------------------
- Here, if the table is created automatically once contract is deployed, then 'create' action not needed.
- add a security layer i.e. check for loc data  when the ride is booked & the action is repeatedly pushed with the same data, it should not consume RAM, CPU, NET. It could be a technical glitch.
- To decide whether 'bookify' or 'book' & 'modify', check whether default values in argument can be parsed like in 'ride_status' If yes, then make 2 actions - 'book' & 'modify'
- [ ] create (to create the table)
- book or bookify
- [ ] modify

## Compile
```console
cabeos_contracts/toeridetaxi
$ eosio-cpp src/toeridetaxi.cpp -o toeridetaxi.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <addpay> does not have a ricardian contract
Warning, action <create> does not have a ricardian contract
Warning, action <assign> does not have a ricardian contract
Warning, action <cancelbycom> does not have a ricardian contract
Warning, action <cancelbydri> does not have a ricardian contract
Warning, action <changedes> does not have a ricardian contract
Warning, action <reachsrc> does not have a ricardian contract
Warning, action <start> does not have a ricardian contract
Warning, action <finish> does not have a ricardian contract
Warning, action <addfareact> does not have a ricardian contract
Warning, action <recvfare> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <eraseride> does not have a ricardian contract
Warning, action <addpay> does not have a ricardian contract
Warning, action <create> does not have a ricardian contract
Warning, action <assign> does not have a ricardian contract
Warning, action <cancelbycom> does not have a ricardian contract
Warning, action <cancelbydri> does not have a ricardian contract
Warning, action <changedes> does not have a ricardian contract
Warning, action <reachsrc> does not have a ricardian contract
Warning, action <start> does not have a ricardian contract
Warning, action <finish> does not have a ricardian contract
Warning, action <addfareact> does not have a ricardian contract
Warning, action <recvfare> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <eraseride> does not have a ricardian contract
```

> NOTE: Here, actions is repeating during compilation bcoz it is likely using multiple jobs. So, it prints multiple times.

## TODO
- [ ] repetitive code snippet into functions like this:
```cpp
	// TODO: convert the market price of fare (calculated in fiat) into 'TOE'.
	// Assume 1 TOE = 5 USD = 375 INR
	int64_t fareamount_in_toe = (fare_est)/375.00;     // convert 'INR' to 'TOE'
	auto fare_toe = asset(fareamount_in_toe, symbol("TOE", 4));     // create a asset variable for converted fare (in TOE)

	ridewallet_index ridewallet_table("toe11rwallet"_n, commuter_ac.value);
	auto wallet_it = ridewallet_table.find(ride_token_symbol.raw());

	// if pay_mode is 'crypto', ensure the fare_amount is present in the faretaxi balance.
	if(pay_mode == "crypto") {
		// ensure that the min. ride wallet's balance has `fare_est` value
		if ((wallet_it->balance) < fare_toe) {
			// print("Sorry! Low balance in the ride wallet.");
			send_alert(commuter_ac, "Sorry! Low balance in the ride wallet.");
			return;
		}
	}

```