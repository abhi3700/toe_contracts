# CabEOS Smart Contracts (SC)

The Smart Contracts is entirely written in C++.

The main contracts for CabEOS are as follows:

* Token
* Ride
* Stake

## Contracts
* Token
	- Actions
		+ [ ] create
		+ [ ] issue
		+ [ ] transfer
		+ [ ] retire
		+ [ ] open
		+ [ ] close
		+ [ ] inflate

* Ride
	- Actions
		+ [ ] create 			// creating the table by the contract ac
		+ [ ] bookify			// represents book/modify
			- ride_status
			- commuter_ac
			- lat_src
			- lon_src
			- lat_des
			- lon_des
			- book_time
		+ [ ] modify
		+ [ ] cancel
		+ [ ] start
		+ [ ] finish
		+ [ ] rate 			// for both driver/commuter
		+ [ ] delete 		// delete the ride entry, even if the rating is not done or will be done later. Bcoz, the RAM can't be consumed due to this delay.
	- Table-1					// showing the `enroute` & `on-trip` rides
	- Workflow (Explained):
		- Here, the ride table will be created only by the contract account using `create` action.
		- And then destroyed using `erase` action.

* Stake
	- Actions
		+ [ ] buy		// ride
		+ [ ] sell	// ride
		+ [ ] setmardri		// market rate for driver
		+ [ ] setmarcom 	// market rate for commuter
	- Table-1			// stores the driver/commuter with their respective tokens staked & corresponding issued rides
	- Table-2			// market rate for driver/commuter

* Govern
	- Actions
		+ [ ] createpoll 		// choices
		+ [ ] modify		// choices
		+ [ ] vote
	- Table-1 				// for conducting the poll
	- Table-2 				// showing the appointed leaders in the communities

> #### NOTES
>    - Here, contract to contract communication would happen like `Ride::finish()` >> `Token::transfer()`;

## Utility
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

<!-- 
* `gen_otp` 
	- generate OTP
	- For unique OTP, link the rand no. corresponding to the hash generated from (
		+ `eos_ac_driver`, 
		+ `eos_ac_commuter`, 
		+ `timestamp`)
* `check_otp`

NOTE: Here, if this is implemented, then it has to be 
1. hashed & stored into the RAM table 				// 3649 -> "545435abf884....4548d3b"
2. encrypted & sent to the commuter				// "brfbdfjebd....fvfdv"
3. decrypted by commuter to say it to the driver		// 3649
4. the decrypted OTP will be checked with the hash (one stored 
		in the RAM table in step 1.)    // 3649 -> "545435abf884....4548d3b" == "545435abf884....4548d3b"
 
 -->

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

|driver_ac|ride_status|commuter_ac|lat_src|lon_src|lat_des|lon_des|book_timestamp|start_timestamp|finish_timestamp|fare_est|fare_act|pick_up_time|waiting_time|tip|driver_rating|driver_comment|commuter_rating|commuter_comment|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

```
where,

driver_ac --> driver EOS account name
ride_status --> enroute/on-trip based on the ride.
commuter_ac --> commuter EOS account name
lat_src --> source location's latitude
lon_src --> source location's longitude
lat_des --> destination location's latitude
lon_des --> destination location's longitude
book_timestamp --> timestamp at which the booking is done.
start_timestamp --> timestamp at which ride starts
finish_timestamp --> timestamp at which ride is finished or cancelled.
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
<!-- * __OTP generation algorithm__ -->

* __Incentive earning algorithm__
	- ride:
		+ commuter/driver needs to rate the ride asap. based on the timestamp difference (from ride finish_timestamp), the incentive would decrease.
	- governance:
		+ participating in governance via voting/election

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


## Data Analysis (For customized experience)
* Show users the travel related Ads based on their trips.
	- if the trip is of `out-of-station` type, then show them 
		+ the best hotels for stay, partner with hotels or such platforms. Here, earn a percentage share.
		+ the best restaurants to eat, partner with them or such platforms. Here, earn a percentage share.


## Trials
* <u>Case-1:</u>
	- __Driver:__ Here, the driver's location is present in contract table with their ride_status as `open`, & they are trying to update the real-time location in Contract table.
	- __Commuter:__ The commuter when books the ride, gets the driver from the contract table based on nearby location & pick-up time.
	- __Constraints:__
		+ driver can finish it's account resources (RAM, CPU, NET) by constantly updating the real-time loc.
		+ If the commuter account's enough resources (CPU, NET) is getting used, then for assigning the ride, we will use another action to do. In this case, the contract's resources (CPU, NET) will be used. 
* <u>Case-2:</u>
	- __Driver:__ Here, the driver's location is present in external-db with their ride_status as `open`, & they are trying to update the real-time location in external-db.
	- __Commuter:__ The commuter when books the ride, gets the driver from the external-db based on nearby location & pick-up time.
	- __Constraints:__
		+ Here, the driver's account resources are intact, as it is communicating constantly with external-db, but not contract table. Also, the external-db has to be communicated with, inside the contract itself.
		+ If the commuter account's enough resources (CPU, NET) is getting used, then for assigning the ride, we will use another action to do. In this case, the contract's resources (CPU, NET) will be used. 


## NOTES (for Technical Whitepaper, Ricardian contracts)
* This notes is for writing the 
	- Technical Whitepaper
	- Ricardian contracts

### Payment mode
* The payment modes could be:
	- `crypto`: i.e. DApp's own cryptocurrency.
	- `fiatdigi`: i.e. National currency in digital mode.
	- `fiatcash`: i.e. National currency in cash mode.

* In the contract's table, all the fares (`fare_est`, `fare_act`) will be shown in national currency.
* Outside the contract, market conversion from `fare_est`/`fare_act` to `fare_crypto_est`/`fare_crypto_act` respectively, is done using CryptoMarket API.
* Inside the contracts' actions, whenever a commuter opts for `crypto` payment mode, then the equivalent amount of DApp's currency (i.e. `TOE`) at that timestamp as per the market rate (calculated outside before committing to the blockchain), is transferred to the Ride wallet (i.e. `ridewallet`) for auto-pay after the ride is completed successfully.
* Keeping market volatility in view, the `fare_crypto_act` is to be paid as calculated during setting the `fare_act`. Basically, conversion value calculated before setting into the Blockchain record, is considered.
* Any form of fare conversion is considered (outside) in 3 actions (in Blockchain Smart contracts) only:
	- `create`: create a ride | `fare_est` ---> `fare_crypto_est`
	- `changedes`: change destination location during the ride | `fare_est` ---> `fare_crypto_est`
	- `addfareact`: adding actual fare considering wait time, ride time in view | `fare_act` ---> `fare_crypto_act`

> NOTE: In order to avoid such obstacles, it is ensured that there is little extra balance maintained in the `ridewallet`, especially if the commuter is a frequent one.


### Voting
Following topics are to be considered for voting among the community members (drivers, commuters):

* The value for __fiat ---> crypto__ conversion in fare, is considered the one, which is calculated before setting into the Blockchain record.
	- Q. Should the fare conversion be done during the dispersal i.e. `recvfare` action (inside)? 
		+ For this, we can use `eos-api` codebase & market API inside the action.
