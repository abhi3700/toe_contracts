# Ride contract
## About
* It is a ride contract which is for 
	- [x] request ride by commuter
	- [ ] request pool ride by commuter(s)
	- [x] assign ride to driver
	- [x] cancel ride by commuter/driver
	- [x] alerting commuter about vehicle has reached the pick-up location.
	- [x] start the ride
	- [x] finish the ride
	- [x] driver receive payment just after the ride is finished.
	- [x] driver add/update the status (online/offline)
	- [x] sending alert
	- [x] sending receipt

## Contract
* contract name - `toeridetaxi`
* contract's account name - `toe1ridetaxi`

## Action
- `create`
- `assign`
- `cancelbycom`
- `cancelbydri`
- `changedes`
- `reachsrc`
- `start`
- `finish`
- `addfareact`
- `recvfare`
- `addristatus`
- `sendalert`
- `sendreceipt`

## Table
* `ridetaxi`
* Notes:
	- For each row, `ram_payer` would be users (driver/commuter) for their piece of data. TODO: 
		+ if the ride is costing too much for their use, then additional amount of CPU, NET will be added to their account during account creation.
	- Here, the scope of the `ridestaxi` table is chosen as __contract name__ (i.e.`get_self().value` syntactically), because there are 2 parties (driver, commuter) involved. So, if either of the user is used as scope, then the other portion of the other user will not be shown. E.g. In other applications where the table row consists of more than 1 party, then it is always better to use __contract name__ as the table's scope.




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
Warning, action <create> does not have a ricardian contract
Warning, action <setfiatpayst> does not have a ricardian contract
Warning, action <assign> does not have a ricardian contract
Warning, action <cancelbycom> does not have a ricardian contract
Warning, action <cancelbydri> does not have a ricardian contract
Warning, action <changedes> does not have a ricardian contract
Warning, action <reachsrc> does not have a ricardian contract
Warning, action <start> does not have a ricardian contract
Warning, action <finish> does not have a ricardian contract
Warning, action <addfareact> does not have a ricardian contract
Warning, action <recvfare> does not have a ricardian contract
Warning, action <addristatus> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <eraseride> does not have a ricardian contract
Warning, action <create> does not have a ricardian contract
Warning, action <setfiatpayst> does not have a ricardian contract
Warning, action <assign> does not have a ricardian contract
Warning, action <cancelbycom> does not have a ricardian contract
Warning, action <cancelbydri> does not have a ricardian contract
Warning, action <changedes> does not have a ricardian contract
Warning, action <reachsrc> does not have a ricardian contract
Warning, action <start> does not have a ricardian contract
Warning, action <finish> does not have a ricardian contract
Warning, action <addfareact> does not have a ricardian contract
Warning, action <recvfare> does not have a ricardian contract
Warning, action <addristatus> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <eraseride> does not have a ricardian contract
```

> NOTE: Here, actions is repeating during compilation bcoz it is likely using multiple jobs. So, it prints multiple times.

## Deploy
* deploy contract
```
/toe_contracts/toeridetaxi
$ cleost set contract toe1ridetaxi ./ -p toe1ridetaxi@active
Reading WASM from /mnt/f/Coding/github_repos/toe_contracts/toeridetaxi/toeridetaxi.wasm...
Publishing contract...
executed transaction: 9acd19a54d1bc42c4476d0d15a2f1b0568ef3338b489d014170f6079bedef862  21624 bytes  8781 us
#         eosio <= eosio::setcode               {"account":"toe1ridetaxi","vmtype":0,"vmversion":0,"code":"0061736d01000000019f022860000060047f7f7f7...
#         eosio <= eosio::setabi                {"account":"toe1ridetaxi","abi":"0e656f73696f3a3a6162692f312e3100110a6164646661726561637400030964726...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- EOS Resources usage:
		+ __RAM__: 715.41 KB
		+ __NET__: 21624 bytes
		+ __CPU__: 8781 us
	- Also, get to see the RAM consumption [here](https://jungle.bloks.io/transaction/9c3ea65b7e5f511ef44fec39adfaf79f87e1ecf81231e4b3cfa763d9396fc382?tab=ram-deltas) in "bloks.io" block explorer
* Adding eosio.code to permissions (for inline actions)
```
$ cleost set account permission toe1ridetaxi active --add-code
executed transaction: b97da9ceb63f41071bfa05282ef3d22207ca051306960eb0b8f4d2019de896df  184 bytes  324 us
#         eosio <= eosio::updateauth            {"account":"toe1ridetaxi","permission":"active","parent":"owner","auth":{"threshold":1,"keys":[{"key...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```


## Testing
### Action - `create`
* prerequisites:
	- commuter should be verified user
	- if __"crypto"__ pay_mode, then fare_crypto_est should be transferred beforehand
* <u>Low balance in crypto wallet:</u>
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "79.00", "17.8000 TOE", "1596732638", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! Low balance in the ride wallet.
pending console output:
```
* Case-I (crypto): create ride by `toecom111111` in __crypto__ mode
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "79.00", "15.8000 TOE", "1596820238", "2", "request a ride from home to office"]' -p toecom111111@active
executed transaction: 7d98280e31ead908bd9160ee9ca8c0dbe0accc5a6c22c1f31a9ce545d5ec6a3a  312 bytes  407 us
#  toe1ridetaxi <= toe1ridetaxi::create         {"commuter_ac":"toecom111111","src_lat_hash":"d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f1...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 requested a ride."}
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 requested a ride."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- trip related info: src loc & des loc
```md
- hashes of src, des lat & lon
d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2
40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035
5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8
367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0

- "Preet City ,Sector 86,Mohali, Sector 86, Sahibzada Ajit Singh Nagar, Punjab": "30.6715713,76.701094"
- "Semi-conductor Laboratory, Department of Space, Govt. of India, Phase 8, Industrial Area, Sector 73, Phase 8, Industrial Area, Sahibzada Ajit Singh Nagar, Punjab": "30.703957,76.6999052"
- "finish_timestamp_est": "Fri, 07 Aug 2020 22:40:38 IST" i.e. 1596820238
```
	- For details of vehicle type with fare, please refer this [img](../images/fare/preet_city_to_scl_ride_request.png) [Link](https://www.uber.com/in/en/price-estimate/)
	- For conversion of INR to TOE: 5 Rs. --> 1.0000 TOE
	- Use [this](https://www.epochconverter.com/) for IST to Unix timestamp conversion
![](../images/ride_exp_app/create_ride.jpg)

* `toecom111111` requested a ride again (with already previous ride ongoing) & gets error
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "79.00", "15.8000 TOE", "1596758438", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: There is already a ride ongoing by a commuter.
pending console output:
```
* Case-II (fiatdigi): create ride by `toecom111112` in __fiatdigi__ mode

### Action - `addristatus`
* Set online status for driver
```console
$ cleost push action toe1ridetaxi addristatus '["toedri111111", "online"]' -p toedri111111@active
executed transaction: f8e6294256ead52a9f762c3e743615f11fffdac77f55bfbae092d152348a6d2f  112 bytes  315 us
#  toe1ridetaxi <= toe1ridetaxi::addristatus    {"driver_ac":"toedri111111","status":"online"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

### Action - `assign`
* assign ride to driver if the status is not present in the table
```console
$ cleost push action toe1ridetaxi assign '['toedri111111', 'toecom111111', '1596819038']' -p toe1ridetaxi@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: driver's status row is not present in the table.
pending console output:
```
* assign ride to driver if the status is present in the table & the driver is online
```console
$ cleost push action toe1ridetaxi assign '['toedri111111', 'toecom111111', '1596819038']' -p toe1ridetaxi@active
executed transaction: e056b65aa183a87805146af7a66ac81802b2add8be80af23b5c7a66efb96550b  112 bytes  347 us
#  toe1ridetaxi <= toe1ridetaxi::assign         {"driver_ac":"toedri111111","commuter_ac":"toecom111111","reachsrc_timestamp_est":1596819038}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 is assigned with a ride."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toecom111111 is assigned with a driver: toedri111111"}
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 is assigned with a ride."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toecom111111 is assigned with a driver: toedri111111"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- trip related info: estimated time taken to reach the src
```
- "reachsrc_timestamp_est": "Fri, 07 Aug 2020 22:20:38 IST" i.e. 1596819038
```

### Action - `reachsrc`
* `toedri111111` reach the src_loc
```console
$ cleost push action toe1ridetaxi reachsrc '["toedri111111"]' -p toedri111111@active
executed transaction: bc185f5791ef6231964bf93eea6b26d2b50d03e6f073f52d7f67ce77d8fcff82  104 bytes  307 us
#  toe1ridetaxi <= toe1ridetaxi::reachsrc       {"driver_ac":"toedri111111"}
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 has reached the pick-up point."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Hello, toecom111111, your driver: toedri111111 has reached the pic...
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 has reached the pick-up point."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Hello, toecom111111, your driver: toedri111111 has reached the pic...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

### Action - `start`
* `toedri111111` start the ride
```console
$ cleost push action toe1ridetaxi start '["toedri111111"]' -p toedri111111@active
executed transaction: 6c12e14be9a80965eaa1fbe70bb9697ca7247d951315756c3a07fc0a82d7f8c4  104 bytes  848 us
#  toe1ridetaxi <= toe1ridetaxi::start          {"driver_ac":"toedri111111"}
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 starts the ride."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toedri111111 starts the ride."}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 starts the ride."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toedri111111 starts the ride."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

### Action - `changedes`
* `toecom111111` change destination in b/w the ride
	- you can change the `pay_mode`. Note that __Case-2__ & __Case-4__ has to be checked outisde TOE SC, then only `changedes` action will be requested.
		+ Case-1: if the pay_mode is __"crypto"__ as previous:
			- then ensure that min __ridewallet__ balance is `fare_crypto_est` (converted from fiat @ Market price rate noted during ride request), or else prompt to send the required amount using `sendfare` func inside `toeridewallet` contract.
		+ Case-2: if the pay_mode is changed from __"crypto"__ to __"fiatdigi"__:
			- then ensure that min fiat __ridewallet__ balance is `fiat_est` (calculated using fiat calculator), or else prompt to send the required amount in __fiat__ wallet.
		+ Case-3: if the pay_mode is changed from __"fiatdigi"__ to __"crypto"__:
			- then ensure that min __ridewallet__ balance is `fare_crypto_est` (converted from fiat @ Market price rate noted during ride request), or else prompt to send the required amount using `sendfare` func inside `toeridewallet` contract.
		+ Case-4: if the pay_mode is __"fiatdigi"__ as previous:
			- then ensure that min fiat __ridewallet__ balance is `fiat_est` (calculated using fiat calculator), or else prompt to send the required amount in __fiat__ wallet.
	- Here, calculate the new __fare_est__ (using Fare calculator), __fare_crypto_est__ (converted @ MP during __ride_request__)
	- trip related info: src loc & des loc
```md
- hashes of new des lat & lon
7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3
b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50

- "Preet City ,Sector 86,Mohali, Sector 86, Sahibzada Ajit Singh Nagar, Punjab": "30.6715713,76.701094"
- "Panjab University Sector 14, Chandigarh": "30.7580107,76.7662895"
- "finish_timestamp_est": "Sat, 08 Aug 2020 22:20:38 IST" i.e. 1596905438
- "fare_est": "180"
- "fare_crypto_est": "36.0000 TOE" i.e. 180/5, where MP --> 1 TOE = 5 INR
```
![](../images/ride_exp_app/change_des.jpg)
	- Error: low balance in the ride wallet
```console
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! Low balance in the ride wallet.
pending console output:
```
	- After transferring required TOE tokens to `ridewallet`, successfully changed destination
```
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
executed transaction: cfb3db081e8f7ec896627d0bce237c0ae49e6c7fa40743fb8a39fff40d6e63fa  216 bytes  447 us
#  toe1ridetaxi <= toe1ridetaxi::changedes      {"commuter_ac":"toecom111111","des_lat_hash":"7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430d...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 changes the destination location & the fare is update...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toecom111111 changes the destination location & the fare is update...
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 changes the destination location & the fare is update...
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toecom111111 changes the destination location & the fare is update...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```


### Action - `finish`


### Action - `eraseride`
* erase ride of `toecom111111` 
```console
$ cleost push action toe1ridetaxi eraseride '["toecom111111", "erase ride"]' -p toe1ridetaxi@active
executed transaction: ba8fc22b2517c5332946d3a332f587f2b3137fab4400a77f51f16d7e049e69a0  112 bytes  767 us
#  toe1ridetaxi <= toe1ridetaxi::eraseride      {"commuter_ac":"toecom111111","memo":"erase ride"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## TODO
- [ ] addrating action
- [ ] increase `rides_limit` when a ride is finished & the corresponding pay_mode is `crypto`
- [ ] add a field 'Market rate' from fiat to crypto at the time of 'request_ride'
