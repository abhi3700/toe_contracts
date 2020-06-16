# CabEOS Smart Contracts (SC)

The Smart Contracts is entirely written in C++.

The main contracts for CabEOS are as follows:

* Token
* Ride
* Stake

## Utility
* `gen_otp` 
	- generate OTP
	- For unique OTP, link the rand no. corresponding to the hash generated from (
		+ `eos_ac_driver`, 
		+ `eos_ac_commuter`, 
		+ `date`, 
		+ `timestamp`)
* `ride_fare_est`
	- estimated ride fare
	- auto calculate
* `ride_fare_act`
	- actual ride fare
	- auto calculate
* `ride_eta`
	- estimated ride time
	- auto calculate
* `ride_ata`
	- actual ride time
	- auto calculate
* `past_rides`
	- past rides of an user (driver/commuter)
	- info which will be shown:
		+ source loc
		+ destination loc
		+ start time (with last updated time)
		+ end time (with last updated time)
* `current_ride`
	- current ride of an user (driver/commuter)
	- info which will be shown:
		+ source loc
		+ destination loc
		+ start time (with last updated time)
		+ end time (estimated time)
		+ status: running
