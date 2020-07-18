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
* actions
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
	- `ridestaxi`
	- `faretaxi`

## Table
* the table is stored in the contract acount.
* For each row, `ram_payer` would be users (driver/commuter) for their piece of data. TODO: 
	- if the ride is costing too much for their use, then additional amount of CPU, NET will be added to their account during account creation.
* Table:
	- `ride`
		+ `taxi`
		+ `bike`

Ride table
----------------

Ride actions
--------------------
- Here, if the table is created automatically once contract is deployed, then 'create' action not needed.
- add a security layer i.e. check for loc data  when the ride is booked & the action is repeatedly pushed with the same data, it should not consume RAM, CPU, NET. It could be a technical glitch.
- To decide whether 'bookify' or 'book' & 'modify', check whether default values in argument can be parsed like in 'ride_status' If yes, then make 2 actions - 'book' & 'modify'
- [ ] create (to create the table)
- book or bookify
- [ ] modify