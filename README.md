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
* `rating_driver`
	- rating of a driver (of all rides till date)
* `rating_commuter`
	- rating of a commuter (of all rides till date)

## Object
* Token
	- inflation
	- burn
* Ride
	- fare (fiat/crypto). crypto benefits:
		+ driver:
			- commission-free rides
			- weekly/monthly incentives (monitoring behavior)
		+ commuter:
			- surge-free rides
			- weekly/monthly incentives (monitoring behavior)
* Stake
	- Here, users will have to stake their token to get
		+ monthly/annually interest
		+ more no. of surge-free rides 
		+ commission-free rides

## Table
* Ride
* Stake

## Algorithms
* __Incentive earning algorithm__

* __Surge reduction model__

* __Ride allocation algorithm__
	- <u>pick-up time:</u> It depends on these 2 factors:
		+ driver's location
		+ congestion level

	- <u>Time swap:</u> When ride is requested, if 2 cabs are found to have this (img below) type of situation, where D1 is relatively far than D2 from C1 & viceversa for C2, then D2, D1 is assigned to C1, C2 respectively.

<p align="center">
  <img src="./images/time_swap.jpg" alt="Time swap" width="" height="">
</p>
	