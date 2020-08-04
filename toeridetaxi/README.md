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
Warning, action <addpaymost> does not have a ricardian contract
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
Warning, action <addristatus> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <eraseride> does not have a ricardian contract
Warning, action <addpaymost> does not have a ricardian contract
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
* create ride by `toecom111111` in __crypto__ mode
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "30.6715713", "76.701094", "30.703957", "76.6999052", "toego", "crypto", "79.00", "15.8000 TOE", "1596483938", "2"]' -p toecom111111@active
```
	- trip related info: src loc & des loc
```md
- "Preet City ,Sector 86,Mohali, Sector 86, Sahibzada Ajit Singh Nagar, Punjab": "30.6715713,76.701094"
- "Semi-conductor Laboratory, Department of Space, Govt. of India, Phase 8, Industrial Area, Sector 73, Phase 8, Industrial Area, Sahibzada Ajit Singh Nagar, Punjab": "30.703957,76.6999052"
- "finish_timestamp_est": "Mon, 03 Aug 2020 21:20:38 IST" i.e. 1596469838
- 
```
	- For details of vehicle type with fare, please refer this [img](../images/fare/preet_city_to_scl_ride_request.png)
	- For conversion of INR to TOE: 5 Rs. --> 1.0000 TOE
	- Use [this](https://www.epochconverter.com/) for IST to Unix timestamp conversion
* create ride by `toecom111112` in __fiatcash__ mode


## TODO
- [ ] addrating action
- [ ] add memo in every action (wherever needed)
- [ ] increase `rides_limit` when a ride is finished & the corresponding pay_mode is `crypto`
