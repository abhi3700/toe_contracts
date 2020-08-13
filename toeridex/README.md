# Ride REX contract
## About
* It is a ride resource exchange (REX) contract which is for: 
	- [x] init RIDEX (ride_qty, toe_balance)
	- [x] driver/commuter buy/sell ride(s) from RIDEX
	- [x] send receipt, alert to driver/commuter
	- [x] send/receive supply (99.5%) from/to account - `toeridexsupp` for RIDEX TOE supply
	- [x] send trading fees (0.5%) to account - `toeridexfees` (revenue earned by company)

## Contract
* contract name - `toeridex`
* contract's account name - `toe1111ridex`

## Action
- `initridex`: initialize the asset with rides for driver/commuter.
- `buyride`:  driver/commuter buy ride(s) from RIDEX
- `sellride`: driver/commuter sell ride(s) from RIDEX
- `sendreceipt`: send receipt/copy
- `sendalert`: send alert

## Table
- `ridexaccount`: `cleos get table toe1111ridex cabeos1dri11 ridexaccount`

| type | rides_limit |
|------|-------------|
| driver | 80 |


- `ridex`: `cleos get table toe1111ridex toe1111ridex assets`

| type | rides_quota | toe_balance |
|------|------|----|
| driver | 1000 | 50 TOE |
| commuter | 2000 | 200 TOE |


## Compile
```console
$ eosio-cpp src/toeridex.cpp -o toeridex.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <initridex> does not have a ricardian contract
Warning, action <buyride> does not have a ricardian contract
Warning, action <sellride> does not have a ricardian contract
Warning, action <addridequota> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <initridex> does not have a ricardian contract
Warning, action <buyride> does not have a ricardian contract
Warning, action <sellride> does not have a ricardian contract
Warning, action <addridequota> does not have a ricardian contract
Warning, action <sendalert> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
```

## Deploy
```console
$ cleost set contract toe1111ridex ./
Reading WASM from /mnt/f/Coding/github_repos/toe_contracts/toeridex/toeridex.wasm...
Publishing contract...
executed transaction: 9ee248e14aebb593367c6351938f95a8f69e721de06a2daeb38d989a3c3290cc  13472 bytes  1550 us
#         eosio <= eosio::setcode               {"account":"toe1111ridex","vmtype":0,"vmversion":0,"code":"0061736d0100000001f6012460000060047f7f7f7...
#         eosio <= eosio::setabi                {"account":"toe1111ridex","abi":"0e656f73696f3a3a6162692f312e3100080c6164647269646571756f74610002097...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* Adding eosio.code to permissions (for inline actions)
```
$ cleost set account permission toe1111ridex active --add-code
executed transaction: e518946f5afdef0a6c8943fa3ddb231f102a5dd1d9ed328b4292d6cff9ceb634  184 bytes  434 us
#         eosio <= eosio::updateauth            {"account":"toe1111ridex","permission":"active","parent":"owner","auth":{"threshold":1,"keys":[{"key...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Testing
### Action - `sendridex` (payable action)
* `bhubtoeindia` account tries to transfer money to RIDEX contract account & due to invalid memo (i.e. doesn't contain - "driver", "commuter", "buy") & gets error and the transfer doesn't happen.
```console
$ cleost push action toe1111token transfer '["bhubtoeindia", "toe1111ridex" "1000000.0000 TOE", "dummy"]'
-p bhubtoeindia@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: invalid memo type for RIDEX contract
pending console output:
```
* `bhubtoeindia` account initializing the RIDEX for __"driver"__ ride type
```console
$ cleost push action toe1111token transfer '["bhubtoeindia", "toe1111ridex" "1000000.0000 TOE", "transfer 1 M TOE tokens to RIDEX contract for driver ride type"]' -p bhubtoeindia@active
executed transaction: e20ab330a572e1bddaa7ba1db30192217b5377506df0acd646f88e3d3bcc90b8  192 bytes  399 us
#  toe1111token <= toe1111token::transfer       {"from":"bhubtoeindia","to":"toe1111ridex","quantity":"1000000.0000 TOE","memo":"transfer 1 M TOE to...
#  bhubtoeindia <= toe1111token::transfer       {"from":"bhubtoeindia","to":"toe1111ridex","quantity":"1000000.0000 TOE","memo":"transfer 1 M TOE to...
#  toe1111ridex <= toe1111token::transfer       {"from":"bhubtoeindia","to":"toe1111ridex","quantity":"1000000.0000 TOE","memo":"transfer 1 M TOE to...
#  toe1111ridex <= toe1111ridex::sendreceipt    {"user":"bhubtoeindia","message":"initialized RIDEX with 1000000.0000 TOE & 1000000 RIDE for 'driver...
#  bhubtoeindia <= toe1111ridex::sendreceipt    {"user":"bhubtoeindia","message":"initialized RIDEX with 1000000.0000 TOE & 1000000 RIDE for 'driver...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view `ridex` table for driver ride type
```console
$ cleost get table toe1111ridex toe1111ridex ridex --show-payer
{
  "rows": [{
      "data": {
        "ride_type": "driver",
        "ride_quota": 1000000,
        "toe_balance": "1000000.0000 TOE"
      },
      "payer": "toe1111ridex"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `bhubtoeindia` account retry initializing the RIDEX for __"driver"__ ride type & gets error:
```console
$ cleost push action toe1111token transfer '["bhubtoeindia", "toe1111ridex" "1000000.0000 TOE", "transfer 1 M TOE tokens to RIDEX contract for driver ride type"]' -p bhubtoeindia@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The values for 'driver' ride_type is already initialized.
pending console output:
```
	- And the money is also not transferred. Check the token's account table for `toe1111ridex`
```console
$ cleost get table toe1111token toe1111ridex accounts
{
  "rows": [{
      "balance": "1000000.0000 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- And the `ridex` table for __"driver"__ ride type is same as before, no modification
```console
$ cleost get table toe1111ridex toe1111ridex ridex --show-payer
{
  "rows": [{
      "data": {
        "ride_type": "driver",
        "ride_quota": 1000000,
        "toe_balance": "1000000.0000 TOE"
      },
      "payer": "toe1111ridex"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `bhubtoeindia` account initializing the RIDEX for __"commuter"__ ride type
```console
$ cleost push action toe1111token transfer '["bhubtoeindia", "toe1111ridex" "1000000.0000 TOE", "transfer 1 M TOE tokens to RIDEX contract for commuter ride type"]' -p bhubtoeindia@active
executed transaction: 8342be6c67422810004296a99c99c55f51fb663e1f856b5e32ed740601299fa7  192 bytes  365 us
#  toe1111token <= toe1111token::transfer       {"from":"bhubtoeindia","to":"toe1111ridex","quantity":"1000000.0000 TOE","memo":"transfer 1 M TOE to...
#  bhubtoeindia <= toe1111token::transfer       {"from":"bhubtoeindia","to":"toe1111ridex","quantity":"1000000.0000 TOE","memo":"transfer 1 M TOE to...
#  toe1111ridex <= toe1111token::transfer       {"from":"bhubtoeindia","to":"toe1111ridex","quantity":"1000000.0000 TOE","memo":"transfer 1 M TOE to...
#  toe1111ridex <= toe1111ridex::sendreceipt    {"user":"bhubtoeindia","message":"initialized RIDEX with 1000000.0000 TOE & 1000000 RIDE for 'commut...
#  bhubtoeindia <= toe1111ridex::sendreceipt    {"user":"bhubtoeindia","message":"initialized RIDEX with 1000000.0000 TOE & 1000000 RIDE for 'commut...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view `ridex` table for commuter ride type
```console
$ cleost get table toe1111ridex toe1111ridex ridex --show-payer
{
  "rows": [{
      "data": {
        "ride_type": "commuter",
        "ride_quota": 1000000,
        "toe_balance": "1000000.0000 TOE"
      },
      "payer": "toe1111ridex"
    },{
      "data": {
        "ride_type": "driver",
        "ride_quota": 1000000,
        "toe_balance": "1000000.0000 TOE"
      },
      "payer": "toe1111ridex"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- Now, view the token contract's accounts table for `toe1111ridex` 
```console
$ cleost get table toe1111token toe1111ridex accounts
{
  "rows": [{
      "balance": "2000000.0000 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* Here, `bhubtoeindia` tries to transfer token to `toe1111ridex` but gets error as it contains both __"driver"__ & __"commuter"__ ride_type in the memo
```console
$ cleost push action toe1111token transfer '["bhubtoeindia", "toe1111ridex" "1000000.0000 TOE", "transfer 1 M TOE tokens to RIDEX contract for driver commuter ride type"]' -p bhubtoeindia@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: invalid memo type for RIDEX contract
pending console output:
```
* driver `toedri111111` transfers '9.9999 TOE' to `toe1111ridex` for buying 10 rides
```console
$ cleost push action toe1111token transfer '["toedri111111", "toe1111ridex" "9.9999 TOE", "transfer almost 10 TOE for buy 10 rides"]' -p toedri111111@active
executed transaction: b25b1e39a8bc2edf1d4d9ce459e92153efb6e301c0c33d16e6d837dc2f58b2d4  168 bytes  425 us
#  toe1111token <= toe1111token::transfer       {"from":"toedri111111","to":"toe1111ridex","quantity":"9.9999 TOE","memo":"transfer almost 10 TOE fo...
#  toedri111111 <= toe1111token::transfer       {"from":"toedri111111","to":"toe1111ridex","quantity":"9.9999 TOE","memo":"transfer almost 10 TOE fo...
#  toe1111ridex <= toe1111token::transfer       {"from":"toedri111111","to":"toe1111ridex","quantity":"9.9999 TOE","memo":"transfer almost 10 TOE fo...
#  toe1111ridex <= toe1111ridex::sendreceipt    {"user":"toedri111111","message":"toedri111111 sent '9.9999 TOE' money for purpose: transfer almost ...
#  toedri111111 <= toe1111ridex::sendreceipt    {"user":"toedri111111","message":"toedri111111 sent '9.9999 TOE' money for purpose: transfer almost ...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the '9.9999 TOE' in ridex user wallet table
```console
$ cleost get table toe1111ridex toedri111111 rexusrwallet --show-payer
{
  "rows": [{
      "data": {
        "balance": "9.9999 TOE"
      },
      "payer": "toe1111ridex"
    }
  ],
  "more": false,
  "next_key": ""
}
```

### Action - `buyride`
* driver `toedri111111` buys 10 rides (__"driver"__ ride type) & gets error as no __balance__:
```console
$ cleost push action toe1111ridex buyride '["toedri111111", "driver", "10", "buy 10 rides"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: The RIDEX wallet for buyer has no balance. Please, transfer '9.9999 TOE'
pending console output:
```
* driver `toedri111111` buys 10 rides (__"commuter"__ ride type) & gets error as wrong __ride_type__:
```console
$ cleost push action toe1111ridex buyride '["toedri111111", "commuter", "10", "buy 10 rides"]' -p toedri111111@active
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: Sorry! A driver can't buy 'commuter' type rides.
pending console output:
```
* driver `toedri111111` buys 10 rides (__"driver"__ ride type) successfully
```console
$ cleost push action toe1111ridex buyride '["toedri111111", "driver", "10", "buy 10 rides"]' -p toedri111111@active
executed transaction: 0b021a796eb013fc316e0a2ff6b4d81a120484b1c540646a01b3faaaaf269228  136 bytes  1185 us
#  toe1111ridex <= toe1111ridex::buyride        {"buyer":"toedri111111","ride_type":"driver","ride_qty":10,"memo":"buy 10 rides"}
#  toe1111token <= toe1111token::transfer       {"from":"toe1111ridex","to":"toeridexfees","quantity":"0.0999 TOE","memo":"transfer fees amount for ...
#  toe1111ridex <= toe1111ridex::sendreceipt    {"user":"toedri111111","message":"You bought 10 rides for '9.9999 TOE' amount"}
#  toe1111ridex <= toe1111token::transfer       {"from":"toe1111ridex","to":"toeridexfees","quantity":"0.0999 TOE","memo":"transfer fees amount for ...
>> Either money is not sent to the contract or contract itself is the sender.
#  toeridexfees <= toe1111token::transfer       {"from":"toe1111ridex","to":"toeridexfees","quantity":"0.0999 TOE","memo":"transfer fees amount for ...
#  toedri111111 <= toe1111ridex::sendreceipt    {"user":"toedri111111","message":"You bought 10 rides for '9.9999 TOE' amount"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the token balance for `toe1111ridex`
```console
$ cleost get table toe1111token toe1111ridex accounts
{
  "rows": [{
      "balance": "2000009.9000 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the token balance for `toeridexfees`
```console
$ cleost get table toe1111token toeridexfees accounts
{
  "rows": [{
      "balance": "0.0999 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the ridex user wallet balance for `toedri111111`
```console
{
  "rows": [{
      "balance": "0.0000 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the ridex ride accounts for `toedri111111`
```console
$ cleost get table toe1111ridex toedri111111 rexuseraccnt
{
  "rows": [{
      "ride_type": "driver",
      "rides_limit": 10
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the RIDEX table
```console
$ cleost get table toe1111ridex toe1111ridex ridex
{
  "rows": [{
      "ride_type": "commuter",
      "ride_quota": 1000000,
      "toe_balance": "1000000.0000 TOE"
    },{
      "ride_type": "driver",
      "ride_quota": 999990,
      "toe_balance": "1000009.8999 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```

### Action - `sellride`
* driver `toedri111111` sells '5 rides' successfully
```console
$ cleost push action toe1111ridex sellride '["toedri111111", "driver", "5", "sell 5 rides"]' -p toedri111111@active
executed transaction: ae85313b73cf70a138baa8af48b20d0dc3ae8794df10036e0504160e56a5cbba  136 bytes  422 us
#  toe1111ridex <= toe1111ridex::sellride       {"seller":"toedri111111","ride_type":"driver","ride_qty":5,"memo":"sell 5 rides"}
#  toe1111token <= toe1111token::transfer       {"from":"toe1111ridex","to":"toedri111111","quantity":"4.9500 TOE","memo":"sell 5 ride(s)"}
#  toe1111token <= toe1111token::transfer       {"from":"toe1111ridex","to":"toeridexfees","quantity":"0.0500 TOE","memo":"transfer fees amount for ...
#  toe1111ridex <= toe1111ridex::sendreceipt    {"user":"toedri111111","message":"You sold 5 rides for '5.0000 TOE' amount"}
#  toe1111ridex <= toe1111token::transfer       {"from":"toe1111ridex","to":"toedri111111","quantity":"4.9500 TOE","memo":"sell 5 ride(s)"}
>> Either money is not sent to the contract or contract itself is the sender.
#  toedri111111 <= toe1111token::transfer       {"from":"toe1111ridex","to":"toedri111111","quantity":"4.9500 TOE","memo":"sell 5 ride(s)"}
#  toe1111ridex <= toe1111token::transfer       {"from":"toe1111ridex","to":"toeridexfees","quantity":"0.0500 TOE","memo":"transfer fees amount for ...
>> Either money is not sent to the contract or contract itself is the sender.
#  toeridexfees <= toe1111token::transfer       {"from":"toe1111ridex","to":"toeridexfees","quantity":"0.0500 TOE","memo":"transfer fees amount for ...
#  toedri111111 <= toe1111ridex::sendreceipt    {"user":"toedri111111","message":"You sold 5 rides for '5.0000 TOE' amount"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- view the token table for `toedri111111`. increment from '26.0001 TOE' to '30.9501 TOE'
```console
$ cleost get table toe1111token toedri111111 accounts
{
  "rows": [{
      "balance": "30.9501 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the RIDEX table
```console
$ cleost get table toe1111ridex toe1111ridex ridex
{
  "rows": [{
      "ride_type": "commuter",
      "ride_quota": 1000000,
      "toe_balance": "1000000.0000 TOE"
    },{
      "ride_type": "driver",
      "ride_quota": 999995,
      "toe_balance": "1000004.9499 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the token balance for `toeridexfees`. An increment from '0.0999 TOE' to '0.1499 TOE'
```console
$ cleost get table toe1111token toeridexfees accounts
{
  "rows": [{
      "balance": "0.1499 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
	- view the ridex ride accounts for `toedri111111`. ride_limit decreases from '10' to '5'.
```console
$ cleost get table toe1111ridex toedri111111 rexuseraccnt
{
  "rows": [{
      "ride_type": "driver",
      "rides_limit": 5
    }
  ],
  "more": false,
  "next_key": ""
}
```

## TODO
- [ ] whenever resetting the RIDEX table (either ride type), ensure all the tokens are returned back to the supply

## NOTES
* __rides_no.__ is added by no. of rides finished (i.e. after `toeridetaxi::finish` action) i.e. after each finished ride, `ride_quota += 1` when pay_mode used is __"crypto"__.
* ride price is regulated by Bancor algorithm, which deduces the price automatically w.r.t to available supply of __TOE__ token, __RIDE__ (in no.)  
* Here, before any action is requested, the user has to be checked if it is a verified one.
* Here, `toeridex` contract account is connected via `addridequota` action with `toeridetaxi` contract.
* For 3 different purposes the tokens are sent to the contract by the
	- __token_issuer__ (for initializing RIDEX for __"driver"__ ride type)
	- __token_issuer__ (for initializing RIDEX for __"commuter"__ ride type), 
	- buyer (for buying ride),
* Action - `consumeride` for:
	- Only 1 ride can be consumed.
	- the ride can only be consumed by __commuter__ or __driver__.
	- As the fare is calculated outside SC, so the __commuter__ can consume ride at 2 stages:
		+ before requesting ride,
		+ before change destination
	- __driver__ can consume ride before starting ride.
* Action - `restoreride`:
	- Only 1 ride can be restored.
	- the ride can only be restored by __commuter__.
	- As the fare is calculated outside SC, so the __commuter__ can restore ride at 1 stage:
		+ before change destination (if happens)
