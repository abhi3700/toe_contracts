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
#### 1. Driver - `toedri111111`
* `toedri111111` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111111", "driver", "e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a0f86763e9ab3298c385", "register by adding info as a driver" ]' -p toedri111111@active
executed transaction: 60429714ae4dc6cdbb771aabefcc404985a9f1765bb683b7bb527cfae1499cf3  168 bytes  999 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111111","type":"driver","profile_hash":"e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111111","message":"the user details is added."}
#  toedri111111 <= toe1userauth::sendreceipt    {"user":"toedri111111","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
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
		+ for this action, resources used:
			- RAM: __0.684 KB__ of `toedri111111` (mentioned in `emplace`, `modify` methods of table) 
			- NET: __176 bytes__ of `toedri111111` (mentioned in `require_auth()`)
			- CPU: __879 us__ of `toedri111111` (mentioned in `require_auth()`)
		+ <u>Observation:</u> The __RAM__, __NET__ are constant for different execution of the `creatifyuser` action. But, the __CPU__ is different i.e. range: __326-991 us__

#### 2. Driver - `toedri111112`
Just like driver - `toedri111111` add & check the info.
* `toedri111112` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111112", "driver", "961a7c07040fe3fff086860b04943db2216cb7a4192293e373c7efa8d9d348fb", "register by adding info as a driver" ]' -p toedri111112@active
executed transaction: a50ef59b153701b8087d9ca7c1160fc636f86c2aa530f4ec59f3065977bdb16d  176 bytes  402 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111112","type":"driver","profile_hash":"961a7c07040fe3fff086860b04943db2216cb7a419229...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111112","message":"the user details is added."}
#  toedri111112 <= toe1userauth::sendreceipt    {"user":"toedri111112","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 3. Driver - `toedri111113`
Just like driver - `toedri111111` add & check the info.
* `toedri111113` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111113", "driver", "847081360ffb58362154d0b7e328d18d9bfec74809a1035f8eb0a9c0907549b6", "register by adding info as a driver" ]' -p toedri111113@active
executed transaction: 13b9689eb356464901aa441a1ce15f5be929b110bfb7f82337a060b4f8342ac1  176 bytes  879 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111113","type":"driver","profile_hash":"847081360ffb58362154d0b7e328d18d9bfec74809a10...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111113","message":"the user details is added."}
#  toedri111113 <= toe1userauth::sendreceipt    {"user":"toedri111113","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 4. Driver - `toedri111114`
Just like driver - `toedri111111` add & check the info.
* `toedri111114` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111114", "driver", "42b5a35892f4103dce2d707e86cca61df5af06d5dc898418c50b8df5166c7e81", "register by adding info as a driver" ]' -p toedri111114@active
executed transaction: 84a68df68bc7e0b7f9110984d644b10b9492584d41da616626ac28b0d35de94f  176 bytes  882 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111114","type":"driver","profile_hash":"42b5a35892f4103dce2d707e86cca61df5af06d5dc898...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111114","message":"the user details is added."}
#  toedri111114 <= toe1userauth::sendreceipt    {"user":"toedri111114","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 5. Driver - `toedri111115`
Just like driver - `toedri111111` add & check the info.
* `toedri111115` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111115", "driver", "071a229f32a7bcca404af704f079bde8ea5e4282f577134f83d0f826c3d8c097", "register by adding info as a driver" ]' -p toedri111115@active
executed transaction: 35624cb57f9a59f5e6b1ba35a4f73b6ac6083278aa0e5cb496ce277c2c7e312c  176 bytes  463 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111115","type":"driver","profile_hash":"071a229f32a7bcca404af704f079bde8ea5e4282f5771...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111115","message":"the user details is added."}
#  toedri111115 <= toe1userauth::sendreceipt    {"user":"toedri111115","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 6. Driver - `toedri111121`
Just like driver - `toedri111111` add & check the info.
* `toedri111121` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111121", "driver", "41dc4b05b5aebc3b06997240eeb8525686453267f233519dabaa4e49e979011c", "register by adding info as a driver" ]' -p toedri111121@active
executed transaction: 9c8b3075603bd2f93f64a07148f42e04b750e63b15440e96c1eefd667137d468  176 bytes  384 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111121","type":"driver","profile_hash":"41dc4b05b5aebc3b06997240eeb8525686453267f2335...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111121","message":"the user details is added."}
#  toedri111121 <= toe1userauth::sendreceipt    {"user":"toedri111121","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 7. Driver - `toedri111122`
Just like driver - `toedri111111` add & check the info.
* `toedri111122` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111122", "driver", "33bb90fbb8e292be5d7b8db0a076020a4b8f035b4bccfb0bd5fd84ba26b2eb4e", "register by adding info as a driver" ]' -p toedri111122@active
executed transaction: 3339ddd63e9ee521a25257d5bed9c12a76ef6ff24ec06a802b3558a90721a132  176 bytes  326 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111122","type":"driver","profile_hash":"33bb90fbb8e292be5d7b8db0a076020a4b8f035b4bccf...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111122","message":"the user details is added."}
#  toedri111122 <= toe1userauth::sendreceipt    {"user":"toedri111122","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 8. Driver - `toedri111123`
Just like driver - `toedri111111` add & check the info.
* `toedri111123` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111123", "driver", "f3469407c99952d21fc54a79c7a9b03530216a0f55e2fa2dc3e38e7960975c37", "register by adding info as a driver" ]' -p toedri111123@active
executed transaction: 0822e630fb18344737f7a07d8038cddc3ad45edb2b5ad3a82973a8487e7b76fd  176 bytes  411 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111123","type":"driver","profile_hash":"f3469407c99952d21fc54a79c7a9b03530216a0f55e2f...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111123","message":"the user details is added."}
#  toedri111123 <= toe1userauth::sendreceipt    {"user":"toedri111123","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 9. Driver - `toedri111124`
Just like driver - `toedri111111` add & check the info.
* `toedri111124` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111124", "driver", "6aff4d0192db8cc6212004b45994c587b303b6c865f843e703d349ddde58b8e6", "register by adding info as a driver" ]' -p toedri111124@active
executed transaction: 58d44a0797d7ffa16dd3c93eb6fc7356e9e7a48daed447274692d245abe3d5a2  176 bytes  399 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111124","type":"driver","profile_hash":"6aff4d0192db8cc6212004b45994c587b303b6c865f84...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111124","message":"the user details is added."}
#  toedri111124 <= toe1userauth::sendreceipt    {"user":"toedri111124","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 10. Driver - `toedri111125`
Just like driver - `toedri111111` add & check the info.
* `toedri111125` uses `creatifyuser` action to register as a driver
```console
$ cleost push action toe1userauth creatifyuser '["toedri111125", "driver", "7f6487394369dcd326d776e0578daae7ff510988411c01945387c95212d185e5", "register by adding info as a driver" ]' -p toedri111125@active
executed transaction: cb8015f9249ca3198795e4b44adc05654de9e38aa9bf2b054b78971059318fbf  176 bytes  991 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toedri111125","type":"driver","profile_hash":"7f6487394369dcd326d776e0578daae7ff510988411c0...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111125","message":"the user details is added."}
#  toedri111125 <= toe1userauth::sendreceipt    {"user":"toedri111125","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 1. Commuter - `toecom111111`
Just like driver - `toedri111111` add & check the info.
* `toecom111111` uses `creatifyuser` action to register as a commuter
```console
$ cleost push action toe1userauth creatifyuser '["toecom111111", "commuter", "cd61dd65af92e0d9b0a157ac6c13116e5bd446c2b5ec2eb9cb8b7f7a2e79b4be", "register by adding info as a commuter" ]' -p toecom111111@active
executed transaction: c55a077adf359ee829aaade3900646cbc5dfb4ae934460704495e223b27cecfe  184 bytes  916 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toecom111111","type":"commuter","profile_hash":"cd61dd65af92e0d9b0a157ac6c13116e5bd446c2b5e...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toecom111111","message":"the user details is added."}
#  toecom111111 <= toe1userauth::sendreceipt    {"user":"toecom111111","message":"the user details is added."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 2. Commuter - `toecom111112`
Just like driver - `toedri111111` add & check the info.
* `toecom111112` uses `creatifyuser` action to register as a commuter
```console
$ cleost push action toe1userauth creatifyuser '["toecom111112", "commuter", "6498978de380f1050c6a15771deaf3b70b771fcb00408fa6584d297c6f8b9a2a", "register by adding info as a commuter" ]' -p toecom111112@active
executed transaction: 663079caaf1f5e71e2cd83df629918f0f702e05234768c95784f475f28744a56  184 bytes  1115 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toecom111112","type":"commuter","profile_hash":"6498978de380f1050c6a15771deaf3b70b771fcb004...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toecom111112","message":"the user registers by adding as a commuter"}
#  toecom111112 <= toe1userauth::sendreceipt    {"user":"toecom111112","message":"the user registers by adding as a commuter"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 1. Company Validator - `bhubtoeinval`
Just like driver - `toedri111111` add & check the info.
* `bhubtoeinval` uses `creatifyuser` action to register as a validator
```console
$ cleost push action toe1userauth creatifyuser '["bhubtoeinval", "validator", "77c219ee156e40ffb3e05c4084ab3268c07e1658c8c5da48d38546f330a720ac", "register by adding info as a company validator" ]' -p bhubtoeinval@active
executed transaction: d4f822ebef14dc0cb654fb9de4e3a170759746ccc45a204703249b7a077400a5  192 bytes  306 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"bhubtoeinval","type":"validator","profile_hash":"77c219ee156e40ffb3e05c4084ab3268c07e1658c8...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"bhubtoeinval","message":"the user registers by adding as a validator"}
#  bhubtoeinval <= toe1userauth::sendreceipt    {"user":"bhubtoeinval","message":"the user registers by adding as a validator"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 1. Validator - `toepar111111`
Just like driver - `toedri111111` add & check the info.
* `toepar111111` uses `creatifyuser` action to register as a validator
```console
$ cleost push action toe1userauth creatifyuser '["toepar111111", "validator", "2f01723e886ad39bc7cbf330faf131310f82392af082b1b0a0598ce1dd538cf7", "register by adding info as a validator" ]' -p toepar111111@active
executed transaction: 9e1e2a7096d33e753a7202fd450621881c4a0dc9094fa11a2f08e7fc41edeb7f  184 bytes  290 us
#  toe1userauth <= toe1userauth::creatifyuser   {"user":"toepar111111","type":"validator","profile_hash":"2f01723e886ad39bc7cbf330faf131310f82392af0...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toepar111111","message":"the user registers by adding as a validator"}
#  toepar111111 <= toe1userauth::sendreceipt    {"user":"toepar111111","message":"the user registers by adding as a validator"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

### Action - `vbdricom`
* `toepar111111` verifies driver - `toedri111111`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111111", "verified", "verify user"]' -p toepar111111@active
executed transaction: 172797738cf436b25d5eb776c1f66afd8965d48912565c41f18733ad0a431e1e  128 bytes  869 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111111","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table after this operation
```console
$ cleost get table toe1userauth toedri111111 users
{
  "rows": [{
      "user": "toedri111111",
      "type": "driver",
      "profile_hash": "e5deda7cafa7b8c861b352e34d6741461ee17a7c2385a0f86763e9ab3298c385",
      "user_status": "verified",
      "add_timestamp": 1596366825,
      "update_timestamp": 0,
      "verify_timestamp": 1596410116,
      "blist_timestamp": 0,
      "validator_verify": "toepar111111",
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
* `toepar111111` verifies driver - `toedri111112`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111112", "verified", "verify user"]' -p toepar111111@active
executed transaction: 18c54e02157d6a43e4c47d0c6b35c3c52e5378ee4358cdde5170a5612042e322  128 bytes  683 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111112","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111112","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111112 <= toe1userauth::sendalert      {"user":"toedri111112","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111113`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111113", "verified", "verify user"]' -p toepar111111@active
executed transaction: 8e7a15e85371e45384f6425e3b29f19ed6e269c3b1b10527fddcf05723f31205  128 bytes  735 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111113","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111113","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111113 <= toe1userauth::sendalert      {"user":"toedri111113","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111114`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111114", "verified", "verify user"]' -p toepar111111@active
executed transaction: 438fb910994c124a2266b242a6eda7b5a0b9f51bdf930131011272b4d8498586  128 bytes  746 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111114","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111114","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111114 <= toe1userauth::sendalert      {"user":"toedri111114","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111115`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111115", "verified", "verify user"]' -p toepar111111@active
executed transaction: 3ed8ed6a2ef426a0826a2510289bf19768d523b24e3f68f6cc9bb603fd0eca83  128 bytes  275 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111115","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111115","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111115 <= toe1userauth::sendalert      {"user":"toedri111115","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111121`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111121", "verified", "verify user"]
' -p toepar111111@active
executed transaction: b30bf90296d459db06ffdb82129a6a1327d517d9c046bf6846cf6c8a4eb77d04  128 bytes  683 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111121","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111121","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111121 <= toe1userauth::sendalert      {"user":"toedri111121","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111122`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111122", "verified", "verify user"]
' -p toepar111111@active
executed transaction: d8f37163eb623f6f2feb544b3bf83c4dee65def2e0453bf7aa6d7f775450722d  128 bytes  755 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111122","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111122","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111122 <= toe1userauth::sendalert      {"user":"toedri111122","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111123`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111123", "verified", "verify user"]
' -p toepar111111@active
executed transaction: bcc2fa8c5700dfc77d609edf91e67b5bf3457d2090fde8a576a73e4079edbe9f  128 bytes  291 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111123","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111123","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111123 <= toe1userauth::sendalert      {"user":"toedri111123","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111124`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111124", "verified", "verify user"]
' -p toepar111111@active
executed transaction: c45e48659742fba1bf31efa32d347ed215cda7cd1416c1c57198600c54de3b35  128 bytes  318 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111124","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111124","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111124 <= toe1userauth::sendalert      {"user":"toedri111124","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111125`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111125", "verified", "verify user"]
' -p toepar111111@active
executed transaction: dfcd46ed5d3cdcac20f9743b7f8eb81e01ae95726ea484648188179322edc20f  128 bytes  274 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111125","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111125","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111125 <= toe1userauth::sendalert      {"user":"toedri111125","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies commuter - `toecom111111`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toecom111111", "verified", "verify user"]
' -p toepar111111@active
executed transaction: cd3e6c0adba7cd80b9280314b188c5856b9f451269b7f79d43b831b166556d2c  128 bytes  723 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toecom111111","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the user status is updated to verified by toepar111111. Note for t...
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies commuter - `toecom111112`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toecom111112", "verified", "verify user"]
' -p toepar111111@active
executed transaction: 93d04f4b657a93fa964233066e5a84b093aec898b221a87c59c938628a5dece9  128 bytes  709 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toecom111112","dricom_user_status":"verified","memo"...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111112","message":"the user status is updated to verified by toepar111111. Note for t...
#  toecom111112 <= toe1userauth::sendalert      {"user":"toecom111112","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```


### Action - `compvbvdator`
* `bhubtoeinval` verifies itself
```console
$ cleost push action toe1userauth compvbvdator '["bhubtoeinval", "verified", "verify itself as validator"]' -p bhubtoeinval@active
executed transaction: 42e80377aacee1481db32391ec8b5ffa98c66705dc9d441ac436818d81f4bc63  136 bytes  689 us
#  toe1userauth <= toe1userauth::compvbvdator   {"validator_user":"bhubtoeinval","validator_user_status":"verified","memo":"verify itself as validat...
#  toe1userauth <= toe1userauth::sendalert      {"user":"bhubtoeinval","message":"the user status is updated to verified by bhubtoeinval. Note for t...
#  bhubtoeinval <= toe1userauth::sendalert      {"user":"bhubtoeinval","message":"the user status is updated to verified by bhubtoeinval. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table after this operation
```console
$ cleost get table toe1userauth bhubtoeinval users
{
  "rows": [{
      "user": "bhubtoeinval",
      "type": "validator",
      "profile_hash": "77c219ee156e40ffb3e05c4084ab3268c07e1658c8c5da48d38546f330a720ac",
      "user_status": "verified",
      "add_timestamp": 1596407762,
      "update_timestamp": 0,
      "verify_timestamp": 1596407911,
      "blist_timestamp": 0,
      "validator_verify": "bhubtoeinval",
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
* `bhubtoeinval` verifies `toepar111111` validator
```console
$ cleost push action toe1userauth compvbvdator '["toepar111111", "verified", "verify validator"]' -p bhubtoeinval@active
executed transaction: 41f448b6ad880ef7a34280636a0b6c7a42b5f49a161cbf2d3713fc0ea714043b  128 bytes  304 us
#  toe1userauth <= toe1userauth::compvbvdator   {"validator_user":"toepar111111","validator_user_status":"verified","memo":"verify validator"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toepar111111","message":"the user status is updated to verified by bhubtoeinval. Note for t...
#  toepar111111 <= toe1userauth::sendalert      {"user":"toepar111111","message":"the user status is updated to verified by bhubtoeinval. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table after this operation
```console
$ cleost get table toe1userauth toepar111111 users
{
  "rows": [{
      "user": "toepar111111",
      "type": "validator",
      "profile_hash": "2f01723e886ad39bc7cbf330faf131310f82392af082b1b0a0598ce1dd538cf7",
      "user_status": "verified",
      "add_timestamp": 1596408554,
      "update_timestamp": 0,
      "verify_timestamp": 1596408859,
      "blist_timestamp": 0,
      "validator_verify": "bhubtoeinval",
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



### Action - `deluser`
* delete a user `bhubtoeinval` from table
```console
$ cleost push action toe1userauth deluser '["bhubtoeinval", "del user, register as validator"]' -p bhubtoeinval@active
executed transaction: 8d291de375bfdd5724bf6d09f7d0e3af5c4e1e601f12889cfabed4bf4656cafd  136 bytes  372 us
#  toe1userauth <= toe1userauth::deluser        {"user":"bhubtoeinval","memo":"del user, register as validator"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"bhubtoeinval","message":"The user is deleted due to: del user, register as validator"}
#  bhubtoeinval <= toe1userauth::sendalert      {"user":"bhubtoeinval","message":"The user is deleted due to: del user, register as validator"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Utility
* [Calculate profile hash](./utils)
	1. `toedri111111` driver
```cpp
string name = "Ramesh Chakravorty";
string address = "5 Garfa 8th Lane, Jadavpur, Kolkata - 700045";
string country_id = "653278229043"; 
```
```console
f69b120a2e7c9c97edec7fb99e85ca8f7dd8afdbe534a39f4adf89add9df9271
205dab1950a8c8f7e011f72f1cec4e4c23351b99cdb9705a3f84d64661d1c704
```
	2. `toedri111112` driver
```cpp
string name = "Balwinder Dhillon";
string address = "512 Sector 23-A, New Delhi - 110001";
string country_id = "564324529287"; 
```
```console
f7dffa3913c29e9e6e6f527e2954869e3ce0853857bee469d1125eb243ed4d3b
0ade6c23f14bf0dd60c49b001acada74eba412abbe8235af3793e65d35a22be1
```
	3. `toedri111113` driver
```cpp
string name = "Gurwinder Singh";
string address = "390 Sector 76, Mohali, Punjab - 160076";
string country_id = "786709853478"; 
```
```console
1433aa6181d46c7279e2ec57800590ebbe0f5575c6835cc97f9c708cd8dfbd6d
645f2f0d51e9ce0b5986312c4145a2f31748f9b34a4ceadd7242b15713c49db5
```
	4. `toedri111114` driver
```cpp
string name = "Manpreet Singh Mokha";
string address = "90 Sector 89, Bathinda, Punjab - 151001";
string country_id = "894576326741"; 
```
```console
612fe4ee5e71d0535b9001d674da490ce275432ebb1b9405bff0650bfbd7174e
016778d07bdfdcf0b7ee53a9ba57f96c823d98a55af05099ca4c06891bada394
```
	5. `toedri111115` driver
```cpp
string name = "Kunal Kumar Singh";
string address = "36HR+27 Sanderi, Maharashtra 402101";
string country_id = "498327450960"; 
```
```console
ce064a32de68a655945ff2c6a749cb918d3e75492ccf218a586eee27ec0f3030
9dca14f43660e77efef7136ef2f4d8c920f42486a4d1ea7ea6c37d814983067f
```
	6. `toedri111121` driver
```cpp
string name = "Abhilash Das";
string address = "26, 7th A Main Rd, \nKEB Colony, New Gurappana Palya, 1st Stage, BTM Layout \nBengaluru, Karnataka 560029";
string country_id = "549034528921"; 
```
```console
c91e73d1dddaf027454226acb8c5dc7b4a5a782dfd4d80a9a6d3b4cee4cf64a8
27b9e5f20a93c49d371fcb7f41825c60e41075945660c065eace516ac59cb7a4
```

	7. `toedri111122` driver
```cpp
string name = "R. Muthuswamy";
string address = "12th St \nAmbattur Industrial Estate \nChennai, Tamil Nadu 600058";
string country_id = "874384218032"; 
```
```console
3338171aa65e71ce1b07234be3acbb9246b87cdf95e52e4bb7a72b5eae0699b9
dd3462fa7bc06b32301ca76b826552e0c930210e517570fd866c2ee387e24c04
```
	8. `toedri111123` driver
```cpp
string name = "P.L. Radhakrishnan";
string address = "Vivekanand Nagar \nKesavadasapuram \nThiruvananthapuram, Kerala 695004";
string country_id = "435287320341"; 
```
```console
6a7949156f1a83a2d7a366a5f123fc409875302fe7c74c5c9884a2bb954f2e5b
c0c532a60c3c4a9cec652abec7d7a788b4397caa5fb18f5f33583bb77bb3fdb6
```
	9. `toedri111124` driver
```cpp
string name = "Sonam Wangchuk";
string address = "Pynthorumkhrah \nShillong, Meghalaya 793019";
string country_id = "324735303275"; 
```
```console
e34b863464f28cc438cc18669ee691bd2286d0c1d67bf568c7531dfda8947d2c
cd2a1931a592d8ec1bbe36b727cd769213fb0610591ab72f4db60901a7a40fef
```
	10. `toedri111125` driver
```cpp
string name = "Kaushik Jalali";
string address = "Shalamar \nJammu 180001";
string country_id = "348732582382"; 
```
```console
905fafce1cf45a4108838146567b0bb3dc352f59b18c5bf24bb690b2f1e8edbb
c252557a7c526576c45dd7b69858ba06e1b5af7c4efd276b825ae9bc7fad76b6
```
	11. `toecom111111` commuter
```cpp
string name = "Abhijit Banerjee";
string address = "650 (1st floor), Sector-78, Mohali, Punjab - 160078";
string country_id = "732547458343"; 
```
```console
e801f6cf24d4aaa0bcc0f1d68f19851eea23429ebf6ccfb5969736b4cc9d7ebb
5b056d7fcdd6e8e9ff07a187a6fa65d0107eafdf2accfcc68ebd6ac2e251bd40
```
	12. `toecom111112` commuter
```cpp
string name = "Dharmender Singh Dhillon";
string address = "K-52 \nBlock K, Kirti Nagar \nNew Delhi, Delhi 110015";
string country_id = "257425328532"; 
```
```console
e1ee761da5a9c1e65ab33249eab993fc49d70f17e6567183070025f37f398218
1c1450b885470d47aaa470003c00c1cf57a0bc2fcb093a9fed5e3f2918153c54
```
	13. `bhubtoeinval` validator
```cpp
string name = "Block.Hub TOE company validator";
string address = "Mohali, Punjab - 160055";
string country_id = "652439874932"; 
```
```console
3b559a3bd748d0a318f1d4028909b5fec688595c148f9669170689d6849b09ce
fc4bbc8516881bd96416a7215ef57ad35d2e1bde630888dfbc0b34614bcbf7f9
```
	14. `toepar111111` validator
```cpp
string name = "Ramesh Chopra";
string address = "F-24 \nBlock F, Kirti Nagar \nNew Delhi, Delhi 110015";
string country_id = "434731547311"; 
```
```console
c0460fbe31d3e24b6b119869791d04ab1b6ed752c994d4e4b01793d32e8d6f6b
f3637590254f281419698dc62e9058eaaff04bc1e0fba1fe0c5899239509821c
```

## TODO
* [ ] in the userauth table, check if `get_self()` needed in modifying for params: `ride_total`, `ride_rated`, `rating_avg`