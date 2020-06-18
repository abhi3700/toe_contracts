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
	- fetched from storage DB.
	- info which will be shown:
		+ source loc
		+ destination loc
		+ start time (with last updated time)
		+ end time (with last updated time)
* `current_ride`
	- current ride of an user (driver/commuter)
	- fetched from blockchain RAM.
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

## Table (in Blockchain RAM)
* Token

* Ride
	- Here, each driver has a row dedicated in Blockchain DB (RAM)
	- When the ride is completed, then the data is moved to a Storage DB (redis type)
	- The row is created when the driver is in 2 stages - `enroute` & `on-trip` 
	- The row is deleted otherwise, i.e. `offline` & `open`
	- The table looks like this:

|ride_no|driver_ac|ride_status|commuter_ac|lat_src|lon_src|lat_des|lon_des|start_timestamp|finish_timestamp|fare_est|fare_act|pick_up_time|waiting_time|tip|driver_rating|driver_comment|commuter_rating|commuter_comment|
|-|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|1|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|2|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|3|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

```
where,

ride_no --> serial number as per the rides created
ride_status --> ongoing/cancelled/finished based on the ride ongoing.
driver_ac --> driver EOS account name
commuter_ac --> commuter EOS account name
lat_src --> source location's latitude
lon_src --> source location's longitude
lat_des --> destination location's latitude
lon_des --> destination location's longitude
start_timestamp --> timestamp at which ride starts
finish_timestamp --> timestamp at which ride finishes
fare_est --> estimate fare before the ride starts
fare_act --> actual fare after the ride finishes
pick_up_time --> time taken by the driver from current loc (at allotment time) to pick-up/source location.
waiting_time --> total waiting time during the ride. Basically, after the ride starts. 5 minutes is exempted (before the ride starts).
tip --> tip amount for the ride
driver_rating --> driver's rating for the ride
driver_comment --> driver's comment (if any)
commuter_rating --> commuter's rating for the ride
commuter_comment --> commuter's comment (if any)
```

* Stake
	- Here, the reason of staking is getting no. of rides i.e. `commission-free`/`surge-free`, which is an asset to a driver/commuter.
	- Each user (driver/commuter) has a row assigned like this:

	| user | type | token | rides |
	|------|------|-------|-------|
	| cabeos1111d1 | driver | 100 | 15 |
	| cabeos1111c3 | commuter | 20 | 40 |

```
where,

user --> a driver/commuter's account name
type --> driver/commuter
token --> platform's token
rides --> no. of available rides (commission-free/surge-free) to the driver/commuter respectively.
```


## Database (SQL, Redis)


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

* __Ride trading algorithm__
	- Driver is able to trade their available rides (commission-free) & make money out of it, depending on the market rate during buy/sell.
	- Commuter is able to trade their available rides (surge-free) & make money out of it, depending on the market rate during buy/sell.
	- Control factors are introduced to drive the asset's (i.e. ride) price by followings:
		+ No. of trips happening in the platform.
		+ Gas price

	