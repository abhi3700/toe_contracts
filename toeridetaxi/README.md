# Ride contract
## About
* It is a ride contract which is for 
	- [x] request ride by commuter
	- [ ] request pool ride by commuter(s)
	- [x] set separately pay_mode & pay_status for non-crypto modes
	- [x] assign ride to driver
	- [x] cancel ride by commuter/driver
	- [x] alerting commuter about vehicle has reached the pick-up location.
	- [x] start the ride
	- [x] finish the ride
	- [x] add actual fare after the ride is marked as complete
	- [x] driver receive payment just after the ride is finished.
	- [x] driver add/update the status (online/offline)
	- [x] sending alert
	- [x] sending receipt

## Contract
* contract name - `toeridetaxi`
* contract's account name - `toe1ridetaxi`

## Action
* `create`
* `setfipaymost`
* `assign`
* `cancelbycom`
* `cancelbydri`
* `changedes`
* `reachsrc`
* `start`
* `finish`
* `addfareact`
* `recvfare`
* `addristatus`
* `sendalert`
* `sendreceipt`

## Table
* `ridetaxi`
	- For each row, `ram_payer` could be:
		+ M-1: driver, commuters, contract for their part of data as per their action. 
		+ M-2: contract only for all the ride data.
would be users (driver/commuter) for their piece of data. TODO: 
	- if the ride is costing too much for their use, then additional amount of CPU, NET will be added to their account during account creation.
	- Here, the scope of the `ridestaxi` table is chosen as __contract name__ (i.e.`get_self().value` syntactically), because there are 2 parties (driver, commuter) involved. So, if either of the user is used as scope, then the other portion of the other user will not be shown. E.g. In other applications where the table row consists of more than 1 party, then it is always better to use __contract name__ as the table's scope.
	- the scope can be a country as well.... in order to separate the rides of different countries.
* `dristatus`
* `rtststamp`
* `rtsfuelprltr`

## Compile
```console
cabeos_contracts/toeridetaxi
$ eosio-cpp src/toeridetaxi.cpp -o toeridetaxi.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <create> does not have a ricardian contract
Warning, action <setfipaymost> does not have a ricardian contract
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
Warning, action <setfipaymost> does not have a ricardian contract
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
* Test cases of different rides - create -> changedes
	- [ ] Case-1: crypto pay_mode
	- [ ] Case-2: fiatdigi pay_mode
	- [ ] Case-3: fiatcash pay_mode
	- [x] Case-4: crypto-crypto pay_mode
		+ `create`
		+ `assign`
		+ `reachsrc`
		+ `start`
		+ `changedes`
		+ `finish`
		+ `addfareact`
		+ `recvfare`
		+ `eraseride`
	- [ ] Case-5: crypto-fiatdigi pay_mode
	- [ ] Case-6: crypto-fiatcash pay_mode
	- [ ] Case-7: fiatdigi-crypto pay_mode
	- [ ] Case-8: fiatdigi-fiatdigi pay_mode
	- [ ] Case-9: fiatdigi-fiatcash pay_mode
	- [ ] Case-10: fiatcash-crypto pay_mode
	- [ ] Case-11: fiatcash-fiatdigi pay_mode
	- [ ] Case-12: fiatcash-fiatcash pay_mode

### A. Case-I (crypto): create ride by `toecom111111` in __crypto__ mode

#### 1.
##### Action - `create`
* prerequisites:
	- commuter should be verified user
	- if __"crypto"__ pay_mode, then fare_crypto_est should be transferred beforehand
* <u>Low balance in crypto wallet:</u>
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "n", "79.00", "5.00", "17.8000 TOE", "1601159711", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! There is no amount transferred by toecom111111 in the ride wallet.
pending console output:
```
* `toecom111111` requesting ride with no RIDEX rides bought, gets error:
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "y", "79.00", "5.00", "15.8000 TOE", "1601159891", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! There is no ride to consume.
pending console output:
```
* create ride by `toecom111111` in __crypto__ mode with no i.e. __"n"__ ridex usage
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "n", "79.00", "5.00", "17.8000 TOE", "1601159711", "2", "request a ride from home to office"]' -p toecom111111@active
executed transaction: 7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139  320 bytes  302 us
#  toe1ridetaxi <= toe1ridetaxi::create         {"commuter_ac":"toecom111111","src_lat_hash":"d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f1...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 requested a ride."}
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 requested a ride."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "requested",
        "driver_ac": "",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 0,
        "reachsrc_timestamp_est": 0,
        "reachsrc_timestamp_act": 0,
        "cancel_timestamp": 0,
        "start_timestamp": 0,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toecom111111"
    }
  ],
  "more": false,
  "next_key": ""
}
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
- "finish_timestamp_est": "Wed, 19 Aug 2020 16:30:38 IST" i.e. 1597834838
```
	- For details of vehicle type with fare, please refer this [img](../images/fare/preet_city_to_scl_ride_request.png) [Link](https://www.uber.com/in/en/price-estimate/)
	- For conversion of INR to TOE: 5 Rs. --> 1.0000 TOE
	- Use [this](https://www.epochconverter.com/) for IST to Unix timestamp conversion
![](../images/ride_exp_app/create_ride.jpg)

* `toecom111111` requested a ride again (with already previous ride ongoing) & gets error
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "n", "79.00", "5.00", "17.8000 TOE", "1601159711", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The ride looks like already set by 'toecom111111' using 'crypto'.
pending console output:
```

##### Action - `addristatus`
* Set online status for driver
```console
$ cleost push action toe1ridetaxi addristatus '["toedri111111", "online"]' -p toedri111111@active
executed transaction: 76780ff823aea9693aa0f4ed3535f6ca83a5156f315c5e85b505db245f702b87  112 bytes  397 us
#  toe1ridetaxi <= toe1ridetaxi::addristatus    {"driver_ac":"toedri111111","status":"online"}
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"Your ride_status is updated to: 'online'."}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"Your ride_status is updated to: 'online'."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `dridestatus`
```console
$ cleost get table toe1ridetaxi toedri111111 dridestatus --show-payer
{
  "rows": [{
      "data": {
        "user_type": "driver",
        "status": "online"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```

##### Action - `assign`
* assign ride to driver: Facing an error, if the status is not present in the table
```console
$ cleost push action toe1ridetaxi assign '['toedri111111', '57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628', '1601162891']' -p toe1ridetaxi@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: driver's status row is not present. Please, add using 'addristatus' action.
pending console output:
```
* assign ride to driver if the status is present in the table & the driver is online
```console
$ cleost push action toe1ridetaxi assign '['toedri111111', '57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628', '1601162891']' -p toe1ridetaxi@active
executed transaction: a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0  136 bytes  288 us
#  toe1ridetaxi <= toe1ridetaxi::assign         {"driver_ac":"toedri111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 is assigned with a ride."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toecom111111 is assigned with a driver: toedri111111"}
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 is assigned with a ride."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toecom111111 is assigned with a driver: toedri111111"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "enroute",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 0,
        "cancel_timestamp": 0,
        "start_timestamp": 0,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toe1ridetaxi"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- trip related info: estimated time taken to reach the src
```
- "reachsrc_timestamp_est": "Sun, 09 Aug 2020 05:12:38 IST" i.e. 1596930158
```
* Error during re-assign the same ride
```console
$ cleost push action toe1ridetaxi assign '['toedri111111', '57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628', '1601162891']' -p toe1ridetaxi@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: the commuter is already assigned with a driver: toedri111111. So, the ride can't be reassigned
pending console output:
```

##### Action - `reachsrc`
* `toedri111111` reach the src_loc
```console
$ cleost push action toe1ridetaxi reachsrc '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628"]' -p toedri111111@active
executed transaction: 553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10  136 bytes  233 us
#  toe1ridetaxi <= toe1ridetaxi::reachsrc       {"driver_ac":"toedri111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 has reached the pick-up point."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Hello, toecom111111, your driver: toedri111111 has reached the pic...
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 has reached the pick-up point."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Hello, toecom111111, your driver: toedri111111 has reached the pic...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "waiting",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 0,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retry the same action & gets error:
```console
$ cleost push action toe1ridetaxi reachsrc '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the ride_status is already marked as 'waiting'. So, you can't modify.
pending console output:
``` 

##### Action - `start`
* `toedri111111` start the ride
```console
$ cleost push action toe1ridetaxi start '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "n"]' -p toedri111111@active
executed transaction: 533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806  144 bytes  242 us
#  toe1ridetaxi <= toe1ridetaxi::start          {"driver_ac":"toedri111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 starts the ride."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toedri111111 starts the ride."}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 starts the ride."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toedri111111 starts the ride."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "ontrip",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          },{
            "first": "start",
            "second": "533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 1601240373,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "n",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retry the action & gets error:
```console
$ cleost push action toe1ridetaxi start '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "n"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the trip is already started. So, you can't restart the ride.
pending console output:
```

##### Action - `changedes`
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
- "finish_timestamp_est": "Sun, 09 Aug 2020 05:50:38 IST" i.e. 1596932438
- "fare_est": "180"
- "fare_crypto_est": "36.0000 TOE" i.e. 180/5, where MP --> 1 TOE = 5 INR
```
![](../images/ride_exp_app/change_des.jpg)
	- Error: low balance in the ride wallet
```console
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "n", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! Low balance in the ride wallet. Please, transfer '18.2000 TOE'.
pending console output:
```
	- After transferring required TOE tokens to `ridewallet`, successfully changed destination
```
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "n", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
executed transaction: 51806838895dc1fc81903f2484a9af25208192612e92d08e836e8daa4030eb69  224 bytes  198 us
#  toe1ridetaxi <= toe1ridetaxi::changedes      {"commuter_ac":"toecom111111","des_lat_hash":"7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430d...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 changes the destination location & the fare is update...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toecom111111 changes the destination location & the fare is update...
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 changes the destination location & the fare is update...
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toecom111111 changes the destination location & the fare is update...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "ontrip",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          },{
            "first": "start",
            "second": "533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806"
          },{
            "first": "changedes",
            "second": "51806838895dc1fc81903f2484a9af25208192612e92d08e836e8daa4030eb69"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 1601240373,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "n",
        "fare_est": "180.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toecom111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toecom111111` change destination with the same lat & lon and gets error:
```console
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "n", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry, both modified latitude & longitude are same as its stored counterpart.
pending console output:
```


##### Action - `finish`
* `toedri111111` finish ride
```console
$ cleost push action toe1ridetaxi finish '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628"]' -p toedri111111@active
executed transaction: 60d3c18923ca91c9d917dbe3ea5fe99ee58d7260a8169b3527286d7de7092a61  136 bytes  320 us
#  toe1ridetaxi <= toe1ridetaxi::finish         {"driver_ac":"toedri111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 finishes the ride."}
#  toe1111ridex <= toe1111ridex::addridequota   {"ride_type":"driver","ride_qty":1}
#  toe1111ridex <= toe1111ridex::addridequota   {"ride_type":"commuter","ride_qty":1}
#  toe1userauth <= toe1userauth::setridetotal   {"user":"toedri111111","user_type":"driver","ride_total":4}
#  toe1userauth <= toe1userauth::setridetotal   {"user":"toecom111111","user_type":"commuter","ride_total":4}
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 finishes the ride."}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_total is updated to: 7"}
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_total is updated to: 7"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_total is updated to: 7"}
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_total is updated to: 7"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "complete",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          },{
            "first": "start",
            "second": "533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806"
          },{
            "first": "changedes",
            "second": "51806838895dc1fc81903f2484a9af25208192612e92d08e836e8daa4030eb69"
          },{
            "first": "finish",
            "second": "60d3c18923ca91c9d917dbe3ea5fe99ee58d7260a8169b3527286d7de7092a61"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 1601240373,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601240982,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "n",
        "fare_est": "180.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retries the action and gets error:
```console
$ cleost push action toe1ridetaxi finish '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the ride by the driver is already marked as complete. So, you can't modify.
pending console output:
```

##### Action - `addfareact`
* this action can be removed by merging with `finish` action as an inline action:
	1. calling the fiat actual price as an external API
	2. then converting the `fare_act` to `fiat_crypto_act` @ the MP (noted during ride request)

* `toedri111111` adds actual fare & fare_crypto (considering MP during ride request)
	- data
```json
"fare_act": "190"	// considering extra time
"fare_crypto_act": "38.0000 TOE"		// converting from fare_act @ MP during request_ride
```
	- push action
```console
$ cleost push action toe1ridetaxi addfareact '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "190", "38.0000 TOE"]' -p toedri111111@active
executed transaction: 85043c1acaee126d07e2dcf5c65cbef4a96e401a8021b66325a16fe78b57a2b2  152 bytes  417 us
#  toe1ridetaxi <= toe1ridetaxi::addfareact     {"driver_ac":"toedri111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 adds the actual fare in INR & TOE"}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Now toecom111111 has to pay 190.000000 INR or '38.0000 TOE'."}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 adds the actual fare in INR & TOE"}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Now toecom111111 has to pay 190.000000 INR or '38.0000 TOE'."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          },{
            "first": "start",
            "second": "533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806"
          },{
            "first": "changedes",
            "second": "51806838895dc1fc81903f2484a9af25208192612e92d08e836e8daa4030eb69"
          },{
            "first": "finish",
            "second": "60d3c18923ca91c9d917dbe3ea5fe99ee58d7260a8169b3527286d7de7092a61"
          },{
            "first": "addfareact",
            "second": "85043c1acaee126d07e2dcf5c65cbef4a96e401a8021b66325a16fe78b57a2b2"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 1601240373,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601240982,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "n",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retries the action and gets error:
```console
$ cleost push action toe1ridetaxi addfareact '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "190", "38.0000 TOE"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the actual fare is already set. You can't modify
pending console output:
```

##### Action - `driaddrating`
```console
$ cleost push action toe1ridetaxi driaddrating '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "4.5"]' -p toedri111111@active
executed transaction: 929864857b883fa77c9ec93406bb48e104bccfc540d73961836db43aa54623b1  136 bytes  266 us
#  toe1ridetaxi <= toe1ridetaxi::driaddrating   {"driver_ac":"toedri111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 rates the ride with ride_id 'b598400c2f04cfc505e22663...
#  toe1userauth <= toe1userauth::setratingavg   {"user":"toecom111111","user_type":"commuter","rating_avg":"4.50000000000000000"}
#  toe1userauth <= toe1userauth::setriderated   {"user":"toecom111111","user_type":"commuter","ride_rated":1}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 rates the ride with ride_id 'b598400c2f04cfc505e22663...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the average rating is updated to: 4.500000"}
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the average rating is updated to: 4.500000"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_rated is updated to: 1"}
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_rated is updated to: 1"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          },{
            "first": "start",
            "second": "533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806"
          },{
            "first": "changedes",
            "second": "51806838895dc1fc81903f2484a9af25208192612e92d08e836e8daa4030eb69"
          },{
            "first": "finish",
            "second": "60d3c18923ca91c9d917dbe3ea5fe99ee58d7260a8169b3527286d7de7092a61"
          },{
            "first": "addfareact",
            "second": "85043c1acaee126d07e2dcf5c65cbef4a96e401a8021b66325a16fe78b57a2b2"
          },{
            "first": "driaddrating",
            "second": "929864857b883fa77c9ec93406bb48e104bccfc540d73961836db43aa54623b1"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 1601240373,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601240982,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "n",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "done",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "4.50000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
  - view the table `dridestatus` to check that the driver's status is changed back to online
```console
$ cleost get table toe1ridetaxi toedri111111 dridestatus --show-payer
{
  "rows": [{
      "data": {
        "user_type": "driver",
        "status": "online"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```

##### Action - `comaddrating`
```console
$ cleost push action toe1ridetaxi comaddrating '["toecom111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "4.8"]' -p toecom111111@active
executed transaction: ab759f6efdc091a507a287831b94ef5a76dca6d0cbcea26669c8dc992b6eed99  136 bytes  299 us
#  toe1ridetaxi <= toe1ridetaxi::comaddrating   {"commuter_ac":"toecom111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e1484...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 rates the ride with ride_id 'b598400c2f04cfc505e22663...
#  toe1userauth <= toe1userauth::setratingavg   {"user":"toedri111111","user_type":"driver","rating_avg":"4.65000009536743164"}
#  toe1userauth <= toe1userauth::setriderated   {"user":"toedri111111","user_type":"driver","ride_rated":1}
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 rates the ride with ride_id 'b598400c2f04cfc505e22663...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the average rating is updated to: 4.650000"}
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the average rating is updated to: 4.650000"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_rated is updated to: 2"}
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_rated is updated to: 2"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          },{
            "first": "start",
            "second": "533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806"
          },{
            "first": "changedes",
            "second": "51806838895dc1fc81903f2484a9af25208192612e92d08e836e8daa4030eb69"
          },{
            "first": "finish",
            "second": "60d3c18923ca91c9d917dbe3ea5fe99ee58d7260a8169b3527286d7de7092a61"
          },{
            "first": "addfareact",
            "second": "85043c1acaee126d07e2dcf5c65cbef4a96e401a8021b66325a16fe78b57a2b2"
          },{
            "first": "driaddrating",
            "second": "929864857b883fa77c9ec93406bb48e104bccfc540d73961836db43aa54623b1"
          },{
            "first": "comaddrating",
            "second": "ab759f6efdc091a507a287831b94ef5a76dca6d0cbcea26669c8dc992b6eed99"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 1601240373,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601240982,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "n",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "done",
        "rating_dri": "4.80000019073486328",
        "rating_status_com": "done",
        "rating_com": "4.50000000000000000"
      },
      "payer": "toecom111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```

##### Action - `recvfare`
* `toedri111111` receives `fare_crypto_act` from the ridewallet contract which has wallet owned by `toecom111111`
```console
$ cleost push action toe1ridetaxi recvfare '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "receives fare for finishing a ride"]' -p toedri111111@active
executed transaction: 8a98e20a293b310874d5b0c03bd4661e5ca898a7ca520683bd932f0d4900856e  168 bytes  459 us
#  toe1ridetaxi <= toe1ridetaxi::recvfare       {"driver_ac":"toedri111111","ride_id":"57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406...
#  toe14rwallet <= toe14rwallet::disburse       {"receiver_ac":"toedri111111","wallet_holder":"toecom111111","quantity":"38.0000 TOE","memo":"receiv...
#  toe1111token <= toe1111token::transfer       {"from":"toe14rwallet","to":"toedri111111","quantity":"28.5000 TOE","memo":"ridewallet disburses 28....
#  toe1111token <= toe1111token::transfer       {"from":"toe14rwallet","to":"toeridesfees","quantity":"9.5000 TOE","memo":"ridewallet disburses 9.50...
#  toe14rwallet <= toe14rwallet::sendalert      {"user":"toedri111111","message":"toedri111111 recieves 28.5000 TOE amount for purpose: receives far...
#  toe14rwallet <= toe14rwallet::sendalert      {"user":"toeridesfees","message":"toeridesfees recieves 9.5000 TOE amount for purpose: receives fare...
#  toe14rwallet <= toe14rwallet::sendalert      {"user":"toecom111111","message":"ridewallet of 'toecom111111' is deducted by 38.0000 TOE amount."}
#  toe14rwallet <= toe1111token::transfer       {"from":"toe14rwallet","to":"toedri111111","quantity":"28.5000 TOE","memo":"ridewallet disburses 28....
>> Either money is not sent to the contract or contract itself is the commuter.
#  toedri111111 <= toe1111token::transfer       {"from":"toe14rwallet","to":"toedri111111","quantity":"28.5000 TOE","memo":"ridewallet disburses 28....
#  toe14rwallet <= toe1111token::transfer       {"from":"toe14rwallet","to":"toeridesfees","quantity":"9.5000 TOE","memo":"ridewallet disburses 9.50...
>> Either money is not sent to the contract or contract itself is the commuter.
#  toeridesfees <= toe1111token::transfer       {"from":"toe14rwallet","to":"toeridesfees","quantity":"9.5000 TOE","memo":"ridewallet disburses 9.50...
#  toedri111111 <= toe14rwallet::sendalert      {"user":"toedri111111","message":"toedri111111 recieves 28.5000 TOE amount for purpose: receives far...
#  toeridesfees <= toe14rwallet::sendalert      {"user":"toeridesfees","message":"toeridesfees recieves 9.5000 TOE amount for purpose: receives fare...
#  toecom111111 <= toe14rwallet::sendalert      {"user":"toecom111111","message":"ridewallet of 'toecom111111' is deducted by 38.0000 TOE amount."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidtodri",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "7c264e376213a5f31cb62c8f385945064e6f82369a2b2a5d8f10d491a1efa139"
          },{
            "first": "assign",
            "second": "a3ff8d08b18977fee68e761e831979a4022972c1f8cb60ffc8505577241aa4f0"
          },{
            "first": "reachsrc",
            "second": "553bb258b02ecf8e1bba1675af8562ceb1666fcd4a829c3398aed850c9417f10"
          },{
            "first": "start",
            "second": "533f96a6b2a5aa8afad3b5544b15d4e9bbf520692c3c37620c483fefa23f7806"
          },{
            "first": "changedes",
            "second": "51806838895dc1fc81903f2484a9af25208192612e92d08e836e8daa4030eb69"
          },{
            "first": "finish",
            "second": "60d3c18923ca91c9d917dbe3ea5fe99ee58d7260a8169b3527286d7de7092a61"
          },{
            "first": "addfareact",
            "second": "85043c1acaee126d07e2dcf5c65cbef4a96e401a8021b66325a16fe78b57a2b2"
          },{
            "first": "driaddrating",
            "second": "929864857b883fa77c9ec93406bb48e104bccfc540d73961836db43aa54623b1"
          },{
            "first": "comaddrating",
            "second": "ab759f6efdc091a507a287831b94ef5a76dca6d0cbcea26669c8dc992b6eed99"
          },{
            "first": "recvfare",
            "second": "8a98e20a293b310874d5b0c03bd4661e5ca898a7ca520683bd932f0d4900856e"
          }
        ],
        "create_timestamp": 1601238588,
        "assign_timestamp": 1601240169,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601240259,
        "cancel_timestamp": 0,
        "start_timestamp": 1601240373,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601240982,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "n",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "done",
        "rating_dri": "4.80000019073486328",
        "rating_status_com": "done",
        "rating_com": "4.50000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the ridewallet balance of commuter - `toecom111111`
```console
$ cleost get table toe14rwallet toecom111111 ridewallet --show-payer
{
  "rows": [],
  "more": false,
  "next_key": ""
}
```
	- view the ridewallet balance of driver - `toedri111111`. Receiving 75% of crypto_act_fare i.e. "28.5000 TOE"
```console
$ cleost get table toe1111token toedri111111 accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "58.9801 TOE"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the ridewallet balance of ridefees account - `toeridesfees`. Receiving 25% commission i.e. "9.5000 TOE"
```console
$ cleost get table toe1111token toeridesfees accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "19.0000 TOE"
      },
      "payer": "toe14rwallet"
    }
  ],
  "more": false,
  "next_key": ""
}
```

* `toedri111111` tries to recive the fare again & gets error:
```console
$ cleost push action toe1ridetaxi recvfare '["toedri111111", "57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628", "receives fare for finishing a ride"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the crypto fare for completed ride to driver: 'toedri111111' is already transferred.
pending console output:
```

##### Action - `erase`
* erase ride of `toecom111111` 
  - Get an error when the driver's rating is still pending
```console
$ cleost push action toe1ridetaxi erase '["toecom111111"]' -p toe1ridetaxi@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The rating is not yet done by driver.
pending console output:
```
  - In this case, erase using `testdelride` action
* successfully erased
```console
$ cleost push action toe1ridetaxi erase '["toecom111111"]' -p toe1ridetaxi@active
executed transaction: e629004bb7f0ed42660348e5a817195a6b36fb6e1b34515ce4024f5eb89ef553  104 bytes  21687 us
#  toe1ridetaxi <= toe1ridetaxi::erase          {"commuter_ac":"toecom111111"}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"the ride_id 'b598400c2f04cfc505e22663cb41a95728664048e1159c3e0da8a...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"the ride_id 'b598400c2f04cfc505e22663cb41a95728664048e1159c3e0da8a...
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"the ride_id 'b598400c2f04cfc505e22663cb41a95728664048e1159c3e0da8a...
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"the ride_id 'b598400c2f04cfc505e22663cb41a95728664048e1159c3e0da8a...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [],
  "more": false,
  "next_key": ""
}
```


==========================================================================================
#### 2.
##### Action - `create`
* prerequisites:
  - commuter should be verified user
  - if __"crypto"__ pay_mode, then fare_crypto_est should be transferred beforehand
* <u>Low balance in crypto wallet:</u>
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "n", "79.00", "5.00", "17.8000 TOE", "1601159711", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! There is no amount transferred by toecom111111 in the ride wallet.
pending console output:
```
* `toecom111111` requesting ride with no RIDEX rides bought, gets error:
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "y", "79.00", "5.00", "15.8000 TOE", "1601159891", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! There is no ride to consume.
pending console output:
```
* create ride by `toecom111111` in __crypto__ mode with no i.e. __"n"__ ridex usage
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "n", "79.00", "5.00", "17.8000 TOE", "1601159711", "2", "request a ride from home to office"]' -p toecom111111@active
executed transaction: 08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81  320 bytes  328 us
#  toe1ridetaxi <= toe1ridetaxi::create         {"commuter_ac":"toecom111111","src_lat_hash":"d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f1...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 requested a ride."}
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 requested a ride."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "requested",
        "driver_ac": "",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 0,
        "reachsrc_timestamp_est": 0,
        "reachsrc_timestamp_act": 0,
        "cancel_timestamp": 0,
        "start_timestamp": 0,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toecom111111"
    }
  ],
  "more": false,
  "next_key": ""
}
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
- "finish_timestamp_est": "Wed, 19 Aug 2020 16:30:38 IST" i.e. 1597834838
```
  - For details of vehicle type with fare, please refer this [img](../images/fare/preet_city_to_scl_ride_request.png) [Link](https://www.uber.com/in/en/price-estimate/)
  - For conversion of INR to TOE: 5 Rs. --> 1.0000 TOE
  - Use [this](https://www.epochconverter.com/) for IST to Unix timestamp conversion
![](../images/ride_exp_app/create_ride.jpg)

* `toecom111111` requested a ride again (with already previous ride ongoing) & gets error
```console
$ cleost push action toe1ridetaxi create '["toecom111111", "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2", "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035", "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8", "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0", "toego", "crypto", "n", "79.00", "5.00", "17.8000 TOE", "1601159711", "2", "request a ride from home to office"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The ride looks like already set by 'toecom111111' using 'crypto'.
pending console output:
```

<!-- ##### Action - `addristatus`
* Set online status for driver
```console
$ cleost push action toe1ridetaxi addristatus '["toedri111111", "online"]' -p toedri111111@active
executed transaction: 76780ff823aea9693aa0f4ed3535f6ca83a5156f315c5e85b505db245f702b87  112 bytes  397 us
#  toe1ridetaxi <= toe1ridetaxi::addristatus    {"driver_ac":"toedri111111","status":"online"}
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"Your ride_status is updated to: 'online'."}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"Your ride_status is updated to: 'online'."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `dridestatus`
```console
$ cleost get table toe1ridetaxi toedri111111 dridestatus --show-payer
{
  "rows": [{
      "data": {
        "user_type": "driver",
        "status": "online"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
 -->
##### Action - `assign`
* assign ride to driver if the status is present in the table & the driver is online
```console
$ cleost push action toe1ridetaxi assign '['toedri111111', 'ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60', '1601162891']' -p toe1ridetaxi@active
executed transaction: afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c  136 bytes  292 us
#  toe1ridetaxi <= toe1ridetaxi::assign         {"driver_ac":"toedri111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 is assigned with a ride."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toecom111111 is assigned with a driver: toedri111111"}
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 is assigned with a ride."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toecom111111 is assigned with a driver: toedri111111"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "enroute",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 0,
        "cancel_timestamp": 0,
        "start_timestamp": 0,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toe1ridetaxi"
    }
  ],
  "more": false,
  "next_key": ""
}
```
  - trip related info: estimated time taken to reach the src
```
- "reachsrc_timestamp_est": "Sun, 09 Aug 2020 05:12:38 IST" i.e. 1596930158
```
* Error during re-assign the same ride
```console
$ cleost push action toe1ridetaxi assign '['toedri111111', 'ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60', '1601162891']' -p toe1ridetaxi@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: the commuter is already assigned with a driver: toedri111111. So, the ride can't be reassigned
pending console output:
```

##### Action - `reachsrc`
* `toedri111111` reach the src_loc
```console
$ cleost push action toe1ridetaxi reachsrc '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60"]' -p toedri111111@active
executed transaction: 94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4  136 bytes  223 us
#  toe1ridetaxi <= toe1ridetaxi::reachsrc       {"driver_ac":"toedri111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 has reached the pick-up point."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Hello, toecom111111, your driver: toedri111111 has reached the pic...
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 has reached the pick-up point."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Hello, toecom111111, your driver: toedri111111 has reached the pic...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "waiting",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 0,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retry the same action & gets error:
```console
$ cleost push action toe1ridetaxi reachsrc '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the ride_status is already marked as 'waiting'. So, you can't modify.
pending console output:
``` 

##### Action - `start`
* `toedri111111` start the ride with __"y"__ ridex_status. This means no/lesser commission will be deducted
```console
$ cleost push action toe1ridetaxi start '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "y"]' -p toedri111111@active
executed transaction: 043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38  144 bytes  243 us
#  toe1ridetaxi <= toe1ridetaxi::start          {"driver_ac":"toedri111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53...
#  toe1111ridex <= toe1111ridex::consumeride    {"user":"toedri111111","user_type":"driver","ride_type":"driver","ride_qty":1}
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 starts the ride."}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toedri111111 starts the ride."}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 starts the ride."}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"toedri111111 starts the ride."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the ridex ride accounts for `toedri111111`
```console
$ cleost get table toe1111ridex toedri111111 rexuseraccnt --show-payer
{
  "rows": [{
      "data": {
        "ride_type": "driver",
        "rides_limit": 7
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "ontrip",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "5c513dcebaf81415ff93c17e545889f9807e23d7f5f6ad3819d7482e489a3ae8",
        "des_lon_hash": "367b6a46f8d4c738ec5090a7828fdee6b441ff157744d907e41e6d26b3cf46b0",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          },{
            "first": "start",
            "second": "043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 1601245748,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "y",
        "fare_est": "79.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "17.8000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retry the action & gets error:
```console
$ cleost push action toe1ridetaxi start '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "y"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the trip is already started. So, you can't restart the ride.
pending console output:
```

##### Action - `changedes`
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
- "finish_timestamp_est": "Sun, 09 Aug 2020 05:50:38 IST" i.e. 1596932438
- "fare_est": "180"
- "fare_crypto_est": "36.0000 TOE" i.e. 180/5, where MP --> 1 TOE = 5 INR
```
![](../images/ride_exp_app/change_des.jpg)
  - Error: low balance in the ride wallet
```console
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "n", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! Low balance in the ride wallet. Please, transfer '16.0000 TOE'.
pending console output:
```
  - After transferring required TOE tokens to `ridewallet`, successfully changed destination
```
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "n", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
executed transaction: 234338c5aed1f0279347bd10a4cb9f519482ae321efa017e5399d8784a83d0e2  224 bytes  277 us
#  toe1ridetaxi <= toe1ridetaxi::changedes      {"commuter_ac":"toecom111111","des_lat_hash":"7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430d...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 changes the destination location & the fare is update...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toecom111111 changes the destination location & the fare is update...
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 changes the destination location & the fare is update...
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toecom111111 changes the destination location & the fare is update...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "ontrip",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          },{
            "first": "start",
            "second": "043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38"
          },{
            "first": "changedes",
            "second": "234338c5aed1f0279347bd10a4cb9f519482ae321efa017e5399d8784a83d0e2"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 1601245748,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 0,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "y",
        "fare_est": "180.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toecom111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toecom111111` change destination with the same lat & lon and gets error:
```console
$ cleost push action toe1ridetaxi changedes '["toecom111111", "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3", "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50", "n", "180", "36.0000 TOE", "crypto", "change destination"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry, both modified latitude & longitude are same as its stored counterpart.
pending console output:
```


##### Action - `finish`
* `toedri111111` finish ride
```console
$ cleost push action toe1ridetaxi finish '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60"]' -p toedri111111@active
executed transaction: 1639adf85cab8d786fdf50594dd80c0250da27dc3b0e956c042d0a6d1c039e64  136 bytes  284 us
#  toe1ridetaxi <= toe1ridetaxi::finish         {"driver_ac":"toedri111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 finishes the ride."}
#  toe1111ridex <= toe1111ridex::addridequota   {"ride_type":"driver","ride_qty":1}
#  toe1111ridex <= toe1111ridex::addridequota   {"ride_type":"commuter","ride_qty":1}
#  toe1userauth <= toe1userauth::setridetotal   {"user":"toedri111111","user_type":"driver","ride_total":1}
#  toe1userauth <= toe1userauth::setridetotal   {"user":"toecom111111","user_type":"commuter","ride_total":1}
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"toedri111111 finishes the ride."}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_total is updated to: 8"}
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_total is updated to: 8"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_total is updated to: 8"}
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_total is updated to: 8"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "complete",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          },{
            "first": "start",
            "second": "043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38"
          },{
            "first": "changedes",
            "second": "234338c5aed1f0279347bd10a4cb9f519482ae321efa017e5399d8784a83d0e2"
          },{
            "first": "finish",
            "second": "1639adf85cab8d786fdf50594dd80c0250da27dc3b0e956c042d0a6d1c039e64"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 1601245748,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601246436,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "y",
        "fare_est": "180.00000000000000000",
        "fare_act": "0.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "0 ",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retries the action and gets error:
```console
$ cleost push action toe1ridetaxi finish '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the ride by the driver is already marked as complete. So, you can't modify.
pending console output:
```

##### Action - `addfareact`
* this action can be removed by merging with `finish` action as an inline action:
  1. calling the fiat actual price as an external API
  2. then converting the `fare_act` to `fiat_crypto_act` @ the MP (noted during ride request)

* `toedri111111` adds actual fare & fare_crypto (considering MP during ride request)
  - data
```json
"fare_act": "190" // considering extra time
"fare_crypto_act": "38.0000 TOE"    // converting from fare_act @ MP during request_ride
```
  - push action
```console
$ cleost push action toe1ridetaxi addfareact '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "190", "38.0000 TOE"]' -p toedri111111@active
executed transaction: 68afe2f8d69b7dbc966c93b657a537cd45f5ff1e1aab1408f94356ea182aeaaf  152 bytes  226 us
#  toe1ridetaxi <= toe1ridetaxi::addfareact     {"driver_ac":"toedri111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 adds the actual fare in INR & TOE"}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Now toecom111111 has to pay 190.000000 INR or '38.0000 TOE'."}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 adds the actual fare in INR & TOE"}
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"Now toecom111111 has to pay 190.000000 INR or '38.0000 TOE'."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          },{
            "first": "start",
            "second": "043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38"
          },{
            "first": "changedes",
            "second": "234338c5aed1f0279347bd10a4cb9f519482ae321efa017e5399d8784a83d0e2"
          },{
            "first": "finish",
            "second": "1639adf85cab8d786fdf50594dd80c0250da27dc3b0e956c042d0a6d1c039e64"
          },{
            "first": "addfareact",
            "second": "68afe2f8d69b7dbc966c93b657a537cd45f5ff1e1aab1408f94356ea182aeaaf"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 1601245748,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601246436,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "y",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` retries the action and gets error:
```console
$ cleost push action toe1ridetaxi addfareact '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "190", "38.0000 TOE"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the actual fare is already set. You can't modify
pending console output:
```

##### Action - `driaddrating`
```console
$ cleost push action toe1ridetaxi driaddrating '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "4.57"]' -p toedri111111@active
executed transaction: dfefeb70ef6e8fcd488d6e402395fa14f49171a5addefea48111003df340f369  136 bytes  372 us
#  toe1ridetaxi <= toe1ridetaxi::driaddrating   {"driver_ac":"toedri111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 rates the ride with ride_id '9a3b6c5985ec601861227158...
#  toe1userauth <= toe1userauth::setratingavg   {"user":"toecom111111","user_type":"commuter","rating_avg":"4.53499984741210938"}
#  toe1userauth <= toe1userauth::setriderated   {"user":"toecom111111","user_type":"commuter","ride_rated":1}
#  toedri111111 <= toe1ridetaxi::sendreceipt    {"user":"toedri111111","message":"toedri111111 rates the ride with ride_id '9a3b6c5985ec601861227158...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the average rating is updated to: 4.535000"}
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the average rating is updated to: 4.535000"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_rated is updated to: 2"}
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the ride_rated is updated to: 2"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          },{
            "first": "start",
            "second": "043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38"
          },{
            "first": "changedes",
            "second": "234338c5aed1f0279347bd10a4cb9f519482ae321efa017e5399d8784a83d0e2"
          },{
            "first": "finish",
            "second": "1639adf85cab8d786fdf50594dd80c0250da27dc3b0e956c042d0a6d1c039e64"
          },{
            "first": "addfareact",
            "second": "68afe2f8d69b7dbc966c93b657a537cd45f5ff1e1aab1408f94356ea182aeaaf"
          },{
            "first": "driaddrating",
            "second": "dfefeb70ef6e8fcd488d6e402395fa14f49171a5addefea48111003df340f369"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 1601245748,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601246436,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "y",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "done",
        "rating_dri": "0.00000000000000000",
        "rating_status_com": "",
        "rating_com": "4.57000017166137695"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
  - view the table `dridestatus` to check that the driver's status is changed back to online
```console
$ cleost get table toe1ridetaxi toedri111111 dridestatus --show-payer
{
  "rows": [{
      "data": {
        "user_type": "driver",
        "status": "online"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* retries rating & gets error
```console
$ cleost push action toe1ridetaxi driaddrating '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "4.57"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The ride is already rated by 'toedri111111
pending console output:
```

##### Action - `comaddrating`
```console
$ cleost push action toe1ridetaxi comaddrating '["toecom111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "4.89"]' -p toecom111111@active
executed transaction: ecbc3b9cee760c4575e953abd643ff83010c00c81c793a29b8b23076c37bfbaa  136 bytes  361 us
#  toe1ridetaxi <= toe1ridetaxi::comaddrating   {"commuter_ac":"toecom111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef...
#  toe1ridetaxi <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 rates the ride with ride_id '9a3b6c5985ec601861227158...
#  toe1userauth <= toe1userauth::setratingavg   {"user":"toedri111111","user_type":"driver","rating_avg":"4.73000001907348633"}
#  toe1userauth <= toe1userauth::setriderated   {"user":"toedri111111","user_type":"driver","ride_rated":1}
#  toecom111111 <= toe1ridetaxi::sendreceipt    {"user":"toecom111111","message":"toecom111111 rates the ride with ride_id '9a3b6c5985ec601861227158...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the average rating is updated to: 4.730000"}
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the average rating is updated to: 4.730000"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_rated is updated to: 3"}
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the ride_rated is updated to: 3"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidbycom",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          },{
            "first": "start",
            "second": "043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38"
          },{
            "first": "changedes",
            "second": "234338c5aed1f0279347bd10a4cb9f519482ae321efa017e5399d8784a83d0e2"
          },{
            "first": "finish",
            "second": "1639adf85cab8d786fdf50594dd80c0250da27dc3b0e956c042d0a6d1c039e64"
          },{
            "first": "addfareact",
            "second": "68afe2f8d69b7dbc966c93b657a537cd45f5ff1e1aab1408f94356ea182aeaaf"
          },{
            "first": "driaddrating",
            "second": "dfefeb70ef6e8fcd488d6e402395fa14f49171a5addefea48111003df340f369"
          },{
            "first": "comaddrating",
            "second": "ecbc3b9cee760c4575e953abd643ff83010c00c81c793a29b8b23076c37bfbaa"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 1601245748,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601246436,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "y",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "done",
        "rating_dri": "4.88999986648559570",
        "rating_status_com": "done",
        "rating_com": "4.57000017166137695"
      },
      "payer": "toecom111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* retries rating
```console
$ cleost push action toe1ridetaxi comaddrating '["toecom111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "4.89"]' -p toecom111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The ride is already rated by commuter: 'toecom111111'
pending console output:
```

##### Action - `recvfare`
* `toedri111111` receives `fare_crypto_act` from the ridewallet contract which has wallet owned by `toecom111111`
```console
$ cleost push action toe1ridetaxi recvfare '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "receives fare for finishing a ride"]' -p toedri111111@active
executed transaction: 5ccb3932f76b1dec57e3907647b678caca284eecf9f6591fe1b25bb720bb27e2  168 bytes  323 us
#  toe1ridetaxi <= toe1ridetaxi::recvfare       {"driver_ac":"toedri111111","ride_id":"ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53...
#  toe14rwallet <= toe14rwallet::disburse       {"receiver_ac":"toedri111111","wallet_holder":"toecom111111","quantity":"38.0000 TOE","memo":"receiv...
#  toe1111token <= toe1111token::transfer       {"from":"toe14rwallet","to":"toedri111111","quantity":"38.0000 TOE","memo":"ridewallet disburses 38....
#  toe14rwallet <= toe14rwallet::sendalert      {"user":"toedri111111","message":"toedri111111 recieves 38.0000 TOE amount for purpose: receives far...
#  toe14rwallet <= toe14rwallet::sendalert      {"user":"toecom111111","message":"ridewallet of 'toecom111111' is deducted by 38.0000 TOE amount."}
#  toe14rwallet <= toe1111token::transfer       {"from":"toe14rwallet","to":"toedri111111","quantity":"38.0000 TOE","memo":"ridewallet disburses 38....
>> Either money is not sent to the contract or contract itself is the commuter.
#  toedri111111 <= toe1111token::transfer       {"from":"toe14rwallet","to":"toedri111111","quantity":"38.0000 TOE","memo":"ridewallet disburses 38....
#  toedri111111 <= toe14rwallet::sendalert      {"user":"toedri111111","message":"toedri111111 recieves 38.0000 TOE amount for purpose: receives far...
#  toecom111111 <= toe14rwallet::sendalert      {"user":"toecom111111","message":"ridewallet of 'toecom111111' is deducted by 38.0000 TOE amount."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [{
      "data": {
        "commuter_ac": "toecom111111",
        "ride_status": "actfareadded",
        "driver_ac": "toedri111111",
        "ride_id": "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60",
        "src_lat_hash": "d362b4ab0413925388f778207c8de2a4af0b9f88204e9e6160c3f10d0a35bda2",
        "src_lon_hash": "40e6a963269233d76eeadf5b9c373878eb6e70b9a3a07a372a2eee75b7060035",
        "des_lat_hash": "7b598dfa7b2c9f051c2f55ef335e61b9911798a88773b27c60430deab84c21b3",
        "des_lon_hash": "b75f80aa9ac46065fe405df0b84908d8f0c689b9d1f344964efad64409e43a50",
        "vehicle_type": "toego",
        "seat_count": 2,
        "pay_mode": "crypto",
        "crypto_paystatus": "paidtodri",
        "fiat_paystatus": "",
        "action_txnid_vector": [{
            "first": "create",
            "second": "08f5cbfe1af81d7c03f1a91677eea76411f3f63a9575fbcbcf6e638546afad81"
          },{
            "first": "assign",
            "second": "afc99adf1815f4a97c4f2a7786dcdb1fca8017696f24d07c4cc7b3d75e330c2c"
          },{
            "first": "reachsrc",
            "second": "94c5e62b538785b6c4e245e6e67df23a977a18dd147b4f2ba1231403e079f5a4"
          },{
            "first": "start",
            "second": "043ffe56905b19bed63af2df850f0877ee3faa770701fae60bd3963d96204c38"
          },{
            "first": "changedes",
            "second": "234338c5aed1f0279347bd10a4cb9f519482ae321efa017e5399d8784a83d0e2"
          },{
            "first": "finish",
            "second": "1639adf85cab8d786fdf50594dd80c0250da27dc3b0e956c042d0a6d1c039e64"
          },{
            "first": "addfareact",
            "second": "68afe2f8d69b7dbc966c93b657a537cd45f5ff1e1aab1408f94356ea182aeaaf"
          },{
            "first": "driaddrating",
            "second": "dfefeb70ef6e8fcd488d6e402395fa14f49171a5addefea48111003df340f369"
          },{
            "first": "comaddrating",
            "second": "ecbc3b9cee760c4575e953abd643ff83010c00c81c793a29b8b23076c37bfbaa"
          },{
            "first": "recvfare",
            "second": "5ccb3932f76b1dec57e3907647b678caca284eecf9f6591fe1b25bb720bb27e2"
          }
        ],
        "create_timestamp": 1601245244,
        "assign_timestamp": 1601245392,
        "reachsrc_timestamp_est": 1601162891,
        "reachsrc_timestamp_act": 1601245538,
        "cancel_timestamp": 0,
        "start_timestamp": 1601245748,
        "changedes_timestamp": 0,
        "finish_timestamp_act": 1601246436,
        "finish_timestamp_est": 1601159711,
        "addfareact_timestamp": 0,
        "ridex_usagestatus_com": "n",
        "ridex_usagestatus_dri": "y",
        "fare_est": "180.00000000000000000",
        "fare_act": "190.00000000000000000",
        "market_price": "5.00000000000000000",
        "fare_crypto_est": "36.0000 TOE",
        "fare_crypto_act": "38.0000 TOE",
        "rating_status_dri": "done",
        "rating_dri": "4.88999986648559570",
        "rating_status_com": "done",
        "rating_com": "4.57000017166137695"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
  - view the ridewallet balance of commuter - `toecom111111`
```console
$ cleost get table toe14rwallet toecom111111 ridewallet --show-payer
{
  "rows": [{
      "data": {
        "balance": "2.0000 TOE"
      },
      "payer": "toe14rwallet"
    }
  ],
  "more": false,
  "next_key": ""
}
```
  - view the ridewallet balance of driver - `toedri111111`. Receiving 100% of crypto_act_fare i.e. "38.0000 TOE"
```console
$ cleost get table toe1111token toedri111111 accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "96.9801 TOE"
      },
      "payer": "toedri111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `toedri111111` tries to recive the fare again & gets error:
```console
$ cleost push action toe1ridetaxi recvfare '["toedri111111", "ab390ea2587122611860ec85596c3b9adb7962d7c813d7d50c193efb1ef53e60", "receives fare for finishing a ride"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! the crypto fare for completed ride to driver: 'toedri111111' is already transferred.
pending console output:
```

##### Action - `erase`
* erase ride of `toecom111111` 
  - Get an error when the driver's rating is still pending
```console
$ cleost push action toe1ridetaxi erase '["toecom111111"]' -p toe1ridetaxi@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The rating is not yet done by driver.
pending console output:
```
  - In this case, erase using `testdelride` action
* successfully erased
```console
$ cleost push action toe1ridetaxi erase '["toecom111111"]' -p toe1ridetaxi@active
executed transaction: ec7b20a1572b46ce753fa52725e0334089083482b5ffe64a25b2ddb7c7f45638  104 bytes  376 us
#  toe1ridetaxi <= toe1ridetaxi::erase          {"commuter_ac":"toecom111111"}
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"the ride_id '9a3b6c5985ec601861227158a20e39ab603ef51efb3e190cd5d71...
#  toe1ridetaxi <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"the ride_id '9a3b6c5985ec601861227158a20e39ab603ef51efb3e190cd5d71...
#  toedri111111 <= toe1ridetaxi::sendalert      {"user":"toedri111111","message":"the ride_id '9a3b6c5985ec601861227158a20e39ab603ef51efb3e190cd5d71...
#  toecom111111 <= toe1ridetaxi::sendalert      {"user":"toecom111111","message":"the ride_id '9a3b6c5985ec601861227158a20e39ab603ef51efb3e190cd5d71...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - view the table `ridestaxi`
```console
$ cleost get table toe1ridetaxi toe1ridetaxi ridestaxi --show-payer --lower toecom111111 --limit 1
{
  "rows": [],
  "more": false,
  "next_key": ""
}
```
=================================================================================================================================================================

### B. Case-II (fiatdigi): create ride by `toecom111111` in __fiatdigi__ mode


## Extra
* delete `ridetaxi` table
```console
$ cleost push action toe1ridetaxi testdelride '["4bdc2764b40b86b62b1faf067b438e3513e677ffad8c2a9ebb9aceedac2413e9"]' -p toe1ridetaxi@active
executed transaction: 1c0f2b06e2ae5369e279892755c3477ad4ead86423080d4c87b8d0af9bbb529f  128 bytes  421 us
#  toe1ridetaxi <= toe1ridetaxi::testdelride    {"ride_id":"4bdc2764b40b86b62b1faf067b438e3513e677ffad8c2a9ebb9aceedac2413e9"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## TODO
* [ ] addrating action
	- here, the `dristatus` would be changed from "assigned" to "online"
* [ ] Can `finish`, `addfareact`, `recvfare` be merged into one action by automatically transferring the money after adding the actual fare. 
* [ ] Implement pool ride. Ensure that the `dridestatus` table is only modified when the ride is not pool type. This is because, when in pool, the "online" to "assigned" is not true for the next commuter to be picked-up. For the next commuter also, the driver's status should be "online", but not "assgined".
* [ ] In `rides` table, whether only commuter/contract should pay for the entire row for all operations, is to be decided based on the row data consumption at the end of the ride. Basically, note the increment in the last ram_payer's RAM, among all ride operations.
* [ ] Redesign the flow from "finish >> addfareact", because in current scenario, the ride is marked 'finished' & after that the `addfareact` action is done. Rather it should be reversed maybe. Basically, the ride is finished only when the commuter is having balance in the ridewallet.

### NOTES
* All the actions are ensured to process sequentially based on checking the `ride_status` field of the table, so that an action can't be accessed before using the previous required action. E.g. `cancelbycom` can only be accessed in __"requested"__, __"enroute"__, __"waiting"__ ride status.
*	addfareact action is not used as inline inside finish action bcoz there are some external factors like waiting time, route etc,...(to be fetched from traditional Database) involved in fare calculation.
* `toeridetaxi` contract is linked:
	- via `disburse` (as an inline action inside `toeridetaxi::recvfare` action) to `toeridewallet` contract
	- via `addridequota` (as an inline action inside `toeridetaxi::finish` action) to `toeridex` contract

* additional `ride_quota` gets increased only if the ride is marked __"complete"__ & the pay_mode chosen is __"crypto"__.
* Fare disbursement to driver after finished ride:
	- "fiatdigi": 25% commission
	- "fiatcash": 25% commission
	- "crypto": 25% commission
	- "crypto" with RIDEX: 0% commission
* Based on `ridex_userstatus_com` == 'y', outside SC, the fare_est would be surge-free.
* Based on `ridex_userstatus_dri` == 'y', outside SC, the recv_fare_act would be commission-free.
*	Anything stored back into server:
	- after erase action (called after `cancel` or `addfareact` or `recvfare`)
* `ride_id` created by hashing these 2 params:
	- commuter_ac
	- timestamp at ride creation
* ride rating will be done via `toeridetaxi` contract.
*	Ride rating:
	- M-1: if rating for a ride is not done within 'x' hours, then the person don't get chance to rate later & gets his own `ride_rating_todo` decreased by 1.
		+ cons: what would be the opponent's rating then.
	- M-2: rating done via `toeuserauth` contract. At any point of time, the rating is done for a past ride.
```cpp
	ACTION addrating( const name& user,
						checksum256 ride_id,
						uint32_t 
						uint32_t addfareact_timestamp,
						float rating)

```
		+ cons: if accessed from CLI, then how to check if the ride_id is legit. bcoz the ride data is no more available in the contract table, but rather moved to storage DB.
* Commuter shall always have 1 row in the table. Because, it can't book another ride until the rating_wait_time is over after finishing the current ride.
* But driver can have many rides in 2 cases:
	- pool ride
	- the finished ride waiting for commuter to rate & then it will be erased. Or the time elapsed (since `addfareact` action) is >= rating_wait_time & then the ride gets deleted.