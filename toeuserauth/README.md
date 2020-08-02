# User Auth Contract
## About
* It is to add/update user details
* Here, a validator (CARES partner) verifies/blacklists the added user details
* Company validates the validators. & then, the validators verify other validators.

> NOTE: A validator can't verify itself. But, can verify other validator/driver/commuter.

* [ ] It is to store the `rating` into the `toe.auth` contract's table.

## Contract
* contract name - `toeuserauth`
* contract's account name - `toe1userauth`

## Action
* `creatifyuser`
* `vbdricom`
* `compvbdator`
* `deluser`
* `sendalert`
* `sendreceipt`

## Table 
* `user`


## Compile
```
/toeuserauth
$ eosio-cpp src/toeuserauth.cpp -o toeuserauth.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <creatifyuser> does not have a ricardian contract
Warning, action <vbdricom> does not have a ricardian contract
Warning, action <compvbvdator> does not have a ricardian contract
Warning, action <deluser> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <creatifyuser> does not have a ricardian contract
Warning, action <vbdricom> does not have a ricardian contract
Warning, action <compvbvdator> does not have a ricardian contract
Warning, action <deluser> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
```
## Deploy
* Deploy contract
```
/toeuserauth
$ cleost set contract toe1userauth ./ -p toe1userauth@active
Reading WASM from /mnt/f/Coding/github_repos/toe_contracts/toeuserauth/toeuserauth.wasm...
Publishing contract...
executed transaction: 04adc13ce81a6b326b031a18ad7ef8b594a75ba2433ead9b901e6eb3d9f3b183  9744 bytes  1118 us
#         eosio <= eosio::setcode               {"account":"toe1userauth","vmtype":0,"vmversion":0,"code":"0061736d0100000001d1012160000060017e00600...
#         eosio <= eosio::setabi                {"account":"toe1userauth","abi":"0e656f73696f3a3a6162692f312e3100070c636f6d707662766461746f7200030e7...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* Adding eosio.code to permissions (for inline actions)
```
$ cleost set account permission toe1userauth active --add-code
executed transaction: 6b1b5e87de2f8d1d8ef9ba8a6e30beef776bdf64d0db1ca2ac07c92b1e75f631  184 bytes  215 us
#         eosio <= eosio::updateauth            {"account":"toe1userauth","permission":"active","parent":"owner","auth":{"threshold":1,"keys":[{"key...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Testing
### Action - `creatifyuser`
#### Driver - `toedri111111`
* `toedri111111` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111111", "driver", "e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a0f86763e9ab3298c385", "register by adding info as a driver" ]' -p toedri111111@active
executed transaction: 60429714ae4dc6cdbb771aabefcc404985a9f1765bb683b7bb527cfae1499cf3  168 bytes  999 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111111","type":"driver","profile_hash":"e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111111","message":"the user details is added."}
#  toedri111111 <= toe1userauth::sendreceipt    {"user":"toedri111111","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- calculated __profile_hash__ for driver's info:
```cpp
string name = "Ramesh Chakravorty";
string address = "5 Garfa 8th Lane, Jadavpur, Kolkata - 700045";
string country_id = "653278229043"; 
```
```console
e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a0f86763e9ab3298c385
```

* view the table after `toedri111111` registers as a __driver__
```console
$ cleost get table toe1userauth toedri111111 users
{
  "rows": [{
      "user": "toedri111111",
      "type": "driver",
      "profile_hash": "e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a0f86763e9ab3298c385",
      "user_status": "added",
      "add_timestamp": 1596366825,
      "update_timestamp": 0,
      "verify_timestamp": 0,
      "blist_timestamp": 0,
      "validator_verify": "",
      "validator_blacklist": "",
      "rating": "0.00000000000000000",
      "ride_total": 0,
      "ride_rated": 0
    }
  ],
  "more": false,
  "next_key": ""
}
```

* You can check that the `toedri111111` driver is notified, by clicking the [link](https://junglehistory.cryptolions.io/v2/history/get_actions?account=toedri111111) in the browser:
```json
{
	"query_time_ms":6.909,"cached":false,"lib":0,"total":{"value":2,"relation":"eq"},
	"actions":[
		{
			"@timestamp":"2020-08-02T11:13:45.000","timestamp":"2020-08-02T11:13:45.000","block_num":104842897,
			"trx_id":"60429714ae4dc6cdbb771aabefcc404985a9f1765bb683b7bb527cfae1499cf3",
			"act":{
				"account":"toe1userauth",
				"name":"sendreceipt",
				"authorization":[{"actor":"toe1userauth","permission":"active"}],
				"data":{
					"user":"toedri111111",
					"message":"the user details is added."
				}
			},
			"notified":["toe1userauth","toedri111111"],
			"global_sequence":613579697,"producer":"cartel","action_ordinal":2,"creator_action_ordinal":1
		},
		{
			"@timestamp":"2020-08-02T11:13:45.000","timestamp":"2020-08-02T11:13:45.000","block_num":104842897,
			"trx_id":"60429714ae4dc6cdbb771aabefcc404985a9f1765bb683b7bb527cfae1499cf3",
			"act":{
				"account":"toe1userauth",
				"name":"creatifyuser",
				"authorization":[{"actor":"toedri111111","permission":"active"}],
				"data":{
					"user":"toedri111111",
					"type":"driver",
					"profile_hash":"E5DEDA7CAFA7B8C861B352E34D6741461EE17A7C2385A0F86763E9AB3298C385",
					"memo":"register for being driver"
				}
			},
			"notified":["toe1userauth"],
			"cpu_usage_us":1318,"net_usage_words":21,
			"account_ram_deltas":[{"account":"toedri111111","delta":700}],
			"global_sequence":613579696,"producer":"cartel","action_ordinal":1,"creator_action_ordinal":0
		}
	]
}
```
	- NOTES:
		+ For main action `creatifyuser`, only the contract (`toe1userauth`) account is notified.
		+ For inline action `sendreceipt`, both the contract (`toe1userauth`) & user (`toedri111111`) accounts are notified.
		+ both the actions are stored in same block no. __104842897__ with same txn_id __60429714ae4dc6cdbb771aabefcc404985a9f1765bb683b7bb527cfae1499cf3__

## Utility
* [Calculate profile hash](./utils)
	- `toedri111111` driver
```cpp
string name = "Ramesh Chakravorty";
string address = "5 Garfa 8th Lane, Jadavpur, Kolkata - 700045";
string country_id = "653278229043"; 
```
```console
e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a0f86763e9ab3298c385
```

