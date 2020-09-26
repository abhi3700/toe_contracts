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
* `signup`
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
Warning, action <signup> does not have a ricardian contract
Warning, action <vbdricom> does not have a ricardian contract
Warning, action <compvbvdator> does not have a ricardian contract
Warning, action <deluser> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <signup> does not have a ricardian contract
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
$ cleost set contract toe1userauth ./
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
### Action - `signup`
#### 1. Driver - `toedri111111`
* `toedri111111` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111111", "driver", "205dab1950a8c8f7e011f72f1cec4e4c23351b99cdb9705a3f84d64661d1c704", "f69b120a2e7c9c97edec7fb99e85ca8f7dd8afdbe534a39f4adf89add9df9271", "register by adding info as a driver" ]' -p toedri111111@active
executed transaction: 676e368f40eddc82a0eb9ecb6dfc6799945d02492dd90fe9c6cd5f86459bfbbc  208 bytes  304 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111111","type":"driver","national_id_hash":"205dab1950a8c8f7e011f72f1cec4e4c23351b99c...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111111","message":"the user registers by adding as a driver"}
#  toedri111111 <= toe1userauth::sendreceipt    {"user":"toedri111111","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table after `toedri111111` registers as a __driver__
```console
$ cleost get table toe1userauth driver user_status
{
  "rows": [{
      "user": "toedri111111",
      "national_id_hash": "205dab1950a8c8f7e011f72f1cec4e4c23351b99cdb9705a3f84d64661d1c704",
      "profile_hash": "f69b120a2e7c9c97edec7fb99e85ca8f7dd8afdbe534a39f4adf89add9df9271",
      "user_status": "added",
      "add_timestamp": 1598040647,
      "update_timestamp": 0,
      "verify_timestamp": 0,
      "blist_timestamp": 0,
      "validator_verify": "",
      "validator_blacklist": "",
      "ride_total": 0,
      "ride_rated": 0,
      "rating_avg": "0.00000000000000000"
    }
  ],
  "more": false,
  "next_key": ""
}
```

* You can check that the `toedri111111` driver is notified, by clicking the [link](https://jungle.eosn.io/v2/history/get_actions?account=toedri111111) for Jungle 3 Testnet in the browser. Also, view on [Block explorer](https://jungle3.bloks.io/account/toedri111111):
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
				"name":"signup",
				"authorization":[{"actor":"toedri111111","permission":"active"}],
				"data":{
					"user":"toedri111111",
					"type":"driver",
					"national_id_hash": "205DAB1950A8C8F7E011F72F1CEC4E4C23351B99CDB9705A3F84D64661D1C704",
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
		+ The main action `signup` is shown in both the contract account - `toe1userauth` & driver account - `toedri111111` .
		+ The inline action `sendreceipt` is shown in both the contract (`toe1userauth`) & user (`toedri111111`) accounts.
		+ both the actions are stored in same block no. __31,775,245__ with same txn_id __676e368f40eddc82a0eb9ecb6dfc6799945d02492dd90fe9c6cd5f86459bfbbc__
		+ for this action, resources used:
			- RAM: __0.739 KB__ of `toedri111111` (mentioned in `emplace`, `modify` methods of table) 
			- NET: __208 bytes__ of `toedri111111` (mentioned in `require_auth()`)
			- CPU: __304 us__ of `toedri111111` (mentioned in `require_auth()`)
		+ <u>Observation:</u> The __RAM__, __NET__ are constant for different execution of the `signup` action. But, the __CPU__ is different i.e. range: __326-991 us__

* driver tries to send same info again, & get error:
```console
$ cleost push action toe1userauth signup '["toedri111111", "driver", "205dab1950a8c8f7e011f72f1cec4e4c23351b99cdb9705a3f84d64661d1c704", "f69b120a2e7c9c97edec7fb99e85ca8f7dd8afdbe534a39f4adf89add9df9271", "register by adding info as a driver" ]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: at least profile or national id hash should be different.
pending console output:
```

#### 2. Driver - `toedri111112`
Just like driver - `toedri111111` add & check the info.
* `toedri111112` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111112", "driver", "0ade6c23f14bf0dd60c49b001acada74eba412abbe8235af3793e65d35a22be1", "f7dffa3913c29e9e6e6f527e2954869e3ce0853857bee469d1125eb243ed4d3b", "register by adding info as a driver" ]' -p toedri111112@active
executed transaction: 56fa33543e33f658be9edf5d252b2dd11dce6d7591974ddd8f4b394f0a241c20  208 bytes  272 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111112","type":"driver","national_id_hash":"0ade6c23f14bf0dd60c49b001acada74eba412abb...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111112","message":"the user registers by adding as a driver"}
#  toedri111112 <= toe1userauth::sendreceipt    {"user":"toedri111112","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 3. Driver - `toedri111113`
Just like driver - `toedri111111` add & check the info.
* `toedri111113` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111113", "driver", "645f2f0d51e9ce0b5986312c4145a2f31748f9b34a4ceadd7242b15713c49db5", "1433aa6181d46c7279e2ec57800590ebbe0f5575c6835cc97f9c708cd8dfbd6d", "register by adding info as a driver" ]' -p toedri111113@active
executed transaction: b1482931d130d053f5626b01b6d79e751d18db1541615883aab42d788c9325a2  208 bytes  284 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111113","type":"driver","national_id_hash":"645f2f0d51e9ce0b5986312c4145a2f31748f9b34...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111113","message":"the user registers by adding as a driver"}
#  toedri111113 <= toe1userauth::sendreceipt    {"user":"toedri111113","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 4. Driver - `toedri111114`
Just like driver - `toedri111111` add & check the info.
* `toedri111114` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111114", "driver", "016778d07bdfdcf0b7ee53a9ba57f96c823d98a55af05099ca4c06891bada394", "612fe4ee5e71d0535b9001d674da490ce275432ebb1b9405bff0650bfbd7174e", "register by adding info as a driver" ]' -p toedri111114@active
executed transaction: c8ab165e0c4f31b8ab636cad733fa2ee41716ed8086a93f9aaee4c0b2e9d83bb  208 bytes  269 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111114","type":"driver","national_id_hash":"016778d07bdfdcf0b7ee53a9ba57f96c823d98a55...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111114","message":"the user registers by adding as a driver"}
#  toedri111114 <= toe1userauth::sendreceipt    {"user":"toedri111114","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 5. Driver - `toedri111115`
Just like driver - `toedri111111` add & check the info.
* `toedri111115` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111115", "driver", "9dca14f43660e77efef7136ef2f4d8c920f42486a4d1ea7ea6c37d814983067f", "ce064a32de68a655945ff2c6a749cb918d3e75492ccf218a586eee27ec0f3030", "register by adding info as a driver" ]' -p toedri111115@active
executed transaction: 4d0adb4f6bd4e44055f4b508b9f91d96c1efc25a7f74de3b675e654b35205334  208 bytes  291 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111115","type":"driver","national_id_hash":"9dca14f43660e77efef7136ef2f4d8c920f42486a...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111115","message":"the user registers by adding as a driver"}
#  toedri111115 <= toe1userauth::sendreceipt    {"user":"toedri111115","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 6. Driver - `toedri111121`
Just like driver - `toedri111111` add & check the info.
* `toedri111121` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111121", "driver", "27b9e5f20a93c49d371fcb7f41825c60e41075945660c065eace516ac59cb7a4", "c91e73d1dddaf027454226acb8c5dc7b4a5a782dfd4d80a9a6d3b4cee4cf64a8", "register by adding info as a driver" ]' -p toedri111121@active
executed transaction: 0efa5fbcceb1a05e649ee2a124fb92a9295e092f53ac15981197be539e5cb5e4  208 bytes  296 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111121","type":"driver","national_id_hash":"27b9e5f20a93c49d371fcb7f41825c60e41075945...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111121","message":"the user registers by adding as a driver"}
#  toedri111121 <= toe1userauth::sendreceipt    {"user":"toedri111121","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 7. Driver - `toedri111122`
Just like driver - `toedri111111` add & check the info.
* `toedri111122` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111122", "driver", "dd3462fa7bc06b32301ca76b826552e0c930210e517570fd866c2ee387e24c04", "3338171aa65e71ce1b07234be3acbb9246b87cdf95e52e4bb7a72b5eae0699b9", "register by adding info as a driver" ]' -p toedri111122@active
executed transaction: 0560df156c548a73bc170d9e6883f82f3df416162c86ed58141de3f9168bb991  208 bytes  249 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111122","type":"driver","national_id_hash":"dd3462fa7bc06b32301ca76b826552e0c930210e5...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111122","message":"the user registers by adding as a driver"}
#  toedri111122 <= toe1userauth::sendreceipt    {"user":"toedri111122","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 8. Driver - `toedri111123`
Just like driver - `toedri111111` add & check the info.
* `toedri111123` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111123", "driver", "c0c532a60c3c4a9cec652abec7d7a788b4397caa5fb18f5f33583bb77bb3fdb6", "6a7949156f1a83a2d7a366a5f123fc409875302fe7c74c5c9884a2bb954f2e5b", "register by adding info as a driver" ]' -p toedri111123@active
executed transaction: f200927d4612e93930e17f7aa093e826c8b705ff0ad63a692334ce1fc34edb6d  208 bytes  276 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111123","type":"driver","national_id_hash":"c0c532a60c3c4a9cec652abec7d7a788b4397caa5...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111123","message":"the user registers by adding as a driver"}
#  toedri111123 <= toe1userauth::sendreceipt    {"user":"toedri111123","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 9. Driver - `toedri111124`
Just like driver - `toedri111111` add & check the info.
* `toedri111124` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111124", "driver", "cd2a1931a592d8ec1bbe36b727cd769213fb0610591ab72f4db60901a7a40fef", "e34b863464f28cc438cc18669ee691bd2286d0c1d67bf568c7531dfda8947d2c", "register by adding info as a driver" ]' -p toedri111124@active
executed transaction: 2bdd59e5a6d3b18c8a85241f0e60ca24cafc72c34cd3ffb9bd887ef68b4d6b52  208 bytes  313 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111124","type":"driver","national_id_hash":"cd2a1931a592d8ec1bbe36b727cd769213fb06105...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111124","message":"the user registers by adding as a driver"}
#  toedri111124 <= toe1userauth::sendreceipt    {"user":"toedri111124","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 10. Driver - `toedri111125`
Just like driver - `toedri111111` add & check the info.
* `toedri111125` uses `signup` action to register as a driver
```console
$ cleost push action toe1userauth signup '["toedri111125", "driver", "c252557a7c526576c45dd7b69858ba06e1b5af7c4efd276b825ae9bc7fad76b6", "905fafce1cf45a4108838146567b0bb3dc352f59b18c5bf24bb690b2f1e8edbb", "register by adding info as a driver" ]' -p toedri111125@active
executed transaction: 4f8a5e207041b22da14a61491c8ebb89d917ad0c734015fb8ead621b9d900782  208 bytes  253 us
#  toe1userauth <= toe1userauth::signup         {"user":"toedri111125","type":"driver","national_id_hash":"c252557a7c526576c45dd7b69858ba06e1b5af7c4...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toedri111125","message":"the user registers by adding as a driver"}
#  toedri111125 <= toe1userauth::sendreceipt    {"user":"toedri111125","message":"the user registers by adding as a driver"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 1. Commuter - `toecom111111`
Just like driver - `toedri111111` add & check the info.
* `toecom111111` uses `signup` action to register as a commuter
```console
$ cleost push action toe1userauth signup '["toecom111111", "commuter", "5b056d7fcdd6e8e9ff07a187a6fa65d0107eafdf2accfcc68ebd6ac2e251bd40", "e801f6cf24d4aaa0bcc0f1d68f19851eea23429ebf6ccfb5969736b4cc9d7ebb", "register by adding info as a commuter" ]' -p toecom111111@active
executed transaction: c6c38ce6252ef72d15c9e024293f9a5b50b337a828b1a29d362867f82df34b72  208 bytes  253 us
#  toe1userauth <= toe1userauth::signup         {"user":"toecom111111","type":"commuter","national_id_hash":"5b056d7fcdd6e8e9ff07a187a6fa65d0107eafdf2...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toecom111111","message":"the user registers by adding as a commuter"}
#  toecom111111 <= toe1userauth::sendreceipt    {"user":"toecom111111","message":"the user registers by adding as a commuter"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 2. Commuter - `toecom111112`
Just like driver - `toedri111111` add & check the info.
* `toecom111112` uses `signup` action to register as a commuter
```console
$ cleost push action toe1userauth signup '["toecom111112", "commuter", "1c1450b885470d47aaa470003c00c1cf57a0bc2fcb093a9fed5e3f2918153c54", "e1ee761da5a9c1e65ab33249eab993fc49d70f17e6567183070025f37f398218", "register by adding info as a commuter" ]' -p toecom111112@active
executed transaction: d89c3d68928fb6cefe34c31bb049e4aefb71ae04299f646c2153bd8c91e234b2  216 bytes  232 us
#  toe1userauth <= toe1userauth::signup         {"user":"toecom111112","type":"commuter","national_id_hash":"1c1450b885470d47aaa470003c00c1cf57a0bc2...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toecom111112","message":"the user registers by adding as a commuter"}
#  toecom111112 <= toe1userauth::sendreceipt    {"user":"toecom111112","message":"the user registers by adding as a commuter"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 1. Validator - `toepar111111`
* `toepar111111` uses `signup` action to register as a validator
```console
$ cleost push action toe1userauth signup '["toepar111111", "validator", "f3637590254f281419698dc62e9058eaaff04bc1e0fba1fe0c5899239509821c", "c0460fbe31d3e24b6b119869791d04ab1b6ed752c994d4e4b01793d32e8d6f6b", "register by adding info as a validator" ]' -p toepar111111@active
executed transaction: efcf9be2d36d8f095475fa1f3d243c672155db3fcc6e3f4eb33498f1c7a73e89  216 bytes  381 us
#  toe1userauth <= toe1userauth::signup         {"user":"toepar111111","type":"validator","national_id_hash":"f3637590254f281419698dc62e9058eaaff04b...
#  toe1userauth <= toe1userauth::sendreceipt    {"user":"toepar111111","message":"the user registers by adding as a validator"}
#  toepar111111 <= toe1userauth::sendreceipt    {"user":"toepar111111","message":"the user registers by adding as a validator"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

### Action - `compvbvdator`
* `bhubtoeinval` verifies `toepar111111` validator
```console
$ cleost push action toe1userauth compvbvdator '["toepar111111", "verified", "verify validator"]' -p bhubtoeinval@active
executed transaction: b9bdfc0b975488dc427f25106f9880f2462e52db70758098310811fb975abed8  128 bytes  279 us
#  toe1userauth <= toe1userauth::compvbvdator   {"validator_user":"toepar111111","validator_user_status":"verified","memo":"verify validator"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toepar111111","message":"the user status is updated to verified by bhubtoeinval. Note for t...
#  toepar111111 <= toe1userauth::sendalert      {"user":"toepar111111","message":"the user status is updated to verified by bhubtoeinval. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- View the table after this operation
```console
$ cleost get table toe1userauth validator users --show-payer
{
  "rows": [{
      "data": {
        "user": "toepar111111",
        "national_id_hash": "f3637590254f281419698dc62e9058eaaff04bc1e0fba1fe0c5899239509821c",
        "profile_hash": "c0460fbe31d3e24b6b119869791d04ab1b6ed752c994d4e4b01793d32e8d6f6b",
        "user_status": "verified",
        "add_timestamp": 1598042527,
        "update_timestamp": 0,
        "verify_timestamp": 1598042927,
        "blist_timestamp": 0,
        "validator_verify": "bhubtoeinval",
        "validator_blacklist": "",
        "ride_total": 0,
        "ride_rated": 0,
        "rating_avg": "0.00000000000000000"
      },
      "payer": "toepar111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```

### Action - `vbdricom`
* `toepar111111` tries to verify driver, but gets error:
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111111", "driver", "verified", "verify user"]' -p toepar111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: validator is not verified
pending console output:
``` 
* `toepar111111` verifies driver - `toedri111111`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111111", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: ee18dfc344aaaae2f5c608d3d25608de899c3aac84eb837748cfbf435a64a02d  136 bytes  205 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111111","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111111","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111111 <= toe1userauth::sendalert      {"user":"toedri111111","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the table after this operation
```console
$ cleost get table toe1userauth driver users --show-payer --lower toedri111111 --limit 1
{
  "rows": [{
      "data": {
        "user": "toedri111111",
        "national_id_hash": "205dab1950a8c8f7e011f72f1cec4e4c23351b99cdb9705a3f84d64661d1c704",
        "profile_hash": "f69b120a2e7c9c97edec7fb99e85ca8f7dd8afdbe534a39f4adf89add9df9271",
        "user_status": "verified",
        "add_timestamp": 1598040647,
        "update_timestamp": 1598041839,
        "verify_timestamp": 1598043053,
        "blist_timestamp": 0,
        "validator_verify": "toepar111111",
        "validator_blacklist": "",
        "ride_total": 0,
        "ride_rated": 0,
        "rating_avg": "0.00000000000000000"
      },
      "payer": "toedri111111"
    }
  ],
  "more": true,
  "next_key": "14777607492439917088"
}
```
* `toepar111111` verifies driver - `toedri111112`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111112", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: 463ca5fc699cf374332f2e9588c3e0862908d2ee03f4667552ad5e6cac668021  136 bytes  315 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111112","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111112","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111112 <= toe1userauth::sendalert      {"user":"toedri111112","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111113`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111113", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: eb70feb7f2e7a9c2007b2bdc16ff25ae8c3efaecf8fa3667bfc409bf3094ed2a  136 bytes  191 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111113","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111113","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111113 <= toe1userauth::sendalert      {"user":"toedri111113","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111114`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111114", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: ff15eb11a1e875d11e00c4e2e6a5003b05d5b391aab796c7707e7355125ddd6a  136 bytes  308 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111114","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111114","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111114 <= toe1userauth::sendalert      {"user":"toedri111114","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111115`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111115", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: 46651ad064f1bf463f862bb040f8bfc2f91193ebe4991da2d3a5bca743b96f9e  136 bytes  228 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111115","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111115","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111115 <= toe1userauth::sendalert      {"user":"toedri111115","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111121`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111121", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: 533b0c5a7ec11b055bc89cae89bfcc53f57f8a18cfa56badfa64c9713767bffa  136 bytes  263 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111121","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111121","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111121 <= toe1userauth::sendalert      {"user":"toedri111121","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111122`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111122", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: 51b5c5b4be9550f221af92f9f9cc63f4c0a739aaa5b116666fc81ef6172859c9  136 bytes  253 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111122","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111122","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111122 <= toe1userauth::sendalert      {"user":"toedri111122","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111123`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111123", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: 0a1f2d3accba69bb5c69d0a2867e7fa60e8ae2eb4d73f7da0a16a3e747351d9a  136 bytes  301 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111123","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111123","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111123 <= toe1userauth::sendalert      {"user":"toedri111123","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111124`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111124", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: 1ea8c498a85731eff6e46c292890ccfb8ee1e1654ea723cb343e11bd364accac  136 bytes  191 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111124","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111124","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111124 <= toe1userauth::sendalert      {"user":"toedri111124","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies driver - `toedri111125`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toedri111125", "driver", "verified", "verify user"]' -p toepar111111@active
executed transaction: c9011790e9066db1d1bcac279a08c10522fe06360b33e38870c879cb3fe1094b  136 bytes  237 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toedri111125","dricom_user_type":"driver","dricom_us...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toedri111125","message":"the user status is updated to verified by toepar111111. Note for t...
#  toedri111125 <= toe1userauth::sendalert      {"user":"toedri111125","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies commuter - `toecom111111`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toecom111111", "commuter", "verified", "verify user as commuter"]' -p toepar111111@active
executed transaction: 5163bc01d6a44d3ac590a39bfcd1d302ecbabf8d4f6ddab026e6f925b036a946  152 bytes  280 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toecom111111","dricom_user_type":"commuter","dricom_...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"the user status is updated to verified by toepar111111. Note for t...
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toepar111111` verifies commuter - `toecom111112`
```console
$ cleost push action toe1userauth vbdricom '["toepar111111", "toecom111112", "commuter", "verified", "verify user as commuter"]' -p toepar111111@active
executed transaction: 177455c452351819931131708589c6137f3474a6147d2857edf5bf6f818d0934  152 bytes  233 us
#  toe1userauth <= toe1userauth::vbdricom       {"validator_user":"toepar111111","dricom_user":"toecom111112","dricom_user_type":"commuter","dricom_...
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111112","message":"the user status is updated to verified by toepar111111. Note for t...
#  toecom111112 <= toe1userauth::sendalert      {"user":"toecom111112","message":"the user status is updated to verified by toepar111111. Note for t...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```


### Action - `compvbvdator`
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
$ cleost get table toe1userauth validator users --show-payer --lower toepar111111 --limit 1
{
  "rows": [{
      "data": {
        "user": "toepar111111",
        "national_id_hash": "f3637590254f281419698dc62e9058eaaff04bc1e0fba1fe0c5899239509821c",
        "profile_hash": "c0460fbe31d3e24b6b119869791d04ab1b6ed752c994d4e4b01793d32e8d6f6b",
        "user_status": "verified",
        "add_timestamp": 1598042527,
        "update_timestamp": 0,
        "verify_timestamp": 1598042927,
        "blist_timestamp": 0,
        "validator_verify": "bhubtoeinval",
        "validator_blacklist": "",
        "ride_total": 0,
        "ride_rated": 0,
        "rating_avg": "0.00000000000000000"
      },
      "payer": "toepar111111"
    }
  ],
  "more": false,
  "next_key": ""
}
```


## Extra
### Action - `testdeluser`
* delete a user `toecom111111` from table
```console
$ cleost push action toe1userauth testdeluser '["toecom111111", "driver", "del user"]' -p toe1userauth@active
executed transaction: cbc843a9dc8be1dc00dd684c719c5141fc7b65d5914e39e7a5979ca2be150b98  120 bytes  341 us
#  toe1userauth <= toe1userauth::testdeluser    {"user":"toecom111111","user_type":"driver","memo":"del user"}
#  toe1userauth <= toe1userauth::sendalert      {"user":"toecom111111","message":"The user is deleted due to: del user"}
#  toecom111111 <= toe1userauth::sendalert      {"user":"toecom111111","message":"The user is deleted due to: del user"}
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
205dab1950a8c8f7e011f72f1cec4e4c23351b99cdb9705a3f84d64661d1c704
f69b120a2e7c9c97edec7fb99e85ca8f7dd8afdbe534a39f4adf89add9df9271
```
	2. `toedri111112` driver
```cpp
string name = "Balwinder Dhillon";
string address = "512 Sector 23-A, New Delhi - 110001";
string country_id = "564324529287"; 
```
```console
0ade6c23f14bf0dd60c49b001acada74eba412abbe8235af3793e65d35a22be1
f7dffa3913c29e9e6e6f527e2954869e3ce0853857bee469d1125eb243ed4d3b
```
	3. `toedri111113` driver
```cpp
string name = "Gurwinder Singh";
string address = "390 Sector 76, Mohali, Punjab - 160076";
string country_id = "786709853478"; 
```
```console
645f2f0d51e9ce0b5986312c4145a2f31748f9b34a4ceadd7242b15713c49db5
1433aa6181d46c7279e2ec57800590ebbe0f5575c6835cc97f9c708cd8dfbd6d
```
	4. `toedri111114` driver
```cpp
string name = "Manpreet Singh Mokha";
string address = "90 Sector 89, Bathinda, Punjab - 151001";
string country_id = "894576326741"; 
```
```console
016778d07bdfdcf0b7ee53a9ba57f96c823d98a55af05099ca4c06891bada394
612fe4ee5e71d0535b9001d674da490ce275432ebb1b9405bff0650bfbd7174e
```
	5. `toedri111115` driver
```cpp
string name = "Kunal Kumar Singh";
string address = "36HR+27 Sanderi, Maharashtra 402101";
string country_id = "498327450960"; 
```
```console
9dca14f43660e77efef7136ef2f4d8c920f42486a4d1ea7ea6c37d814983067f
ce064a32de68a655945ff2c6a749cb918d3e75492ccf218a586eee27ec0f3030
```
	6. `toedri111121` driver
```cpp
string name = "Abhilash Das";
string address = "26, 7th A Main Rd, \nKEB Colony, New Gurappana Palya, 1st Stage, BTM Layout \nBengaluru, Karnataka 560029";
string country_id = "549034528921"; 
```
```console
27b9e5f20a93c49d371fcb7f41825c60e41075945660c065eace516ac59cb7a4
c91e73d1dddaf027454226acb8c5dc7b4a5a782dfd4d80a9a6d3b4cee4cf64a8
```

	7. `toedri111122` driver
```cpp
string name = "R. Muthuswamy";
string address = "12th St \nAmbattur Industrial Estate \nChennai, Tamil Nadu 600058";
string country_id = "874384218032"; 
```
```console
dd3462fa7bc06b32301ca76b826552e0c930210e517570fd866c2ee387e24c04
3338171aa65e71ce1b07234be3acbb9246b87cdf95e52e4bb7a72b5eae0699b9
```
	8. `toedri111123` driver
```cpp
string name = "P.L. Radhakrishnan";
string address = "Vivekanand Nagar \nKesavadasapuram \nThiruvananthapuram, Kerala 695004";
string country_id = "435287320341"; 
```
```console
c0c532a60c3c4a9cec652abec7d7a788b4397caa5fb18f5f33583bb77bb3fdb6
6a7949156f1a83a2d7a366a5f123fc409875302fe7c74c5c9884a2bb954f2e5b
```
	9. `toedri111124` driver
```cpp
string name = "Sonam Wangchuk";
string address = "Pynthorumkhrah \nShillong, Meghalaya 793019";
string country_id = "324735303275"; 
```
```console
cd2a1931a592d8ec1bbe36b727cd769213fb0610591ab72f4db60901a7a40fef
e34b863464f28cc438cc18669ee691bd2286d0c1d67bf568c7531dfda8947d2c
```
	10. `toedri111125` driver
```cpp
string name = "Kaushik Jalali";
string address = "Shalamar \nJammu 180001";
string country_id = "348732582382"; 
```
```console
c252557a7c526576c45dd7b69858ba06e1b5af7c4efd276b825ae9bc7fad76b6
905fafce1cf45a4108838146567b0bb3dc352f59b18c5bf24bb690b2f1e8edbb
```
	11. `toecom111111` commuter
```cpp
string name = "Abhijit Banerjee";
string address = "650 (1st floor), Sector-78, Mohali, Punjab - 160078";
string country_id = "732547458343"; 
```
```console
5b056d7fcdd6e8e9ff07a187a6fa65d0107eafdf2accfcc68ebd6ac2e251bd40
e801f6cf24d4aaa0bcc0f1d68f19851eea23429ebf6ccfb5969736b4cc9d7ebb
```
	12. `toecom111112` commuter
```cpp
string name = "Dharmender Singh Dhillon";
string address = "K-52 \nBlock K, Kirti Nagar \nNew Delhi, Delhi 110015";
string country_id = "257425328532"; 
```
```console
1c1450b885470d47aaa470003c00c1cf57a0bc2fcb093a9fed5e3f2918153c54
e1ee761da5a9c1e65ab33249eab993fc49d70f17e6567183070025f37f398218
```
	13. `bhubtoeinval` validator
```cpp
string name = "Block.Hub TOE company validator";
string address = "Mohali, Punjab - 160055";
string country_id = "652439874932"; 
```
```console
fc4bbc8516881bd96416a7215ef57ad35d2e1bde630888dfbc0b34614bcbf7f9
3b559a3bd748d0a318f1d4028909b5fec688595c148f9669170689d6849b09ce
```
	14. `toepar111111` validator
```cpp
string name = "Ramesh Chopra";
string address = "F-24 \nBlock F, Kirti Nagar \nNew Delhi, Delhi 110015";
string country_id = "434731547311"; 
```
```console
f3637590254f281419698dc62e9058eaaff04bc1e0fba1fe0c5899239509821c
c0460fbe31d3e24b6b119869791d04ab1b6ed752c994d4e4b01793d32e8d6f6b
```

## TODO
* [ ] in the userauth table, check if `get_self()` needed in modifying for params: `ride_total`, `ride_rated`, `rating_avg`