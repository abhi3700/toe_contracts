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

## TODO
- [ ] whenever resetting the RIDEX table (either ride type), ensure all the tokens are returned back to the supply

## NOTES
* __rides_no.__ is added by no. of rides finished (i.e. after `toeridetaxi::finish` action) i.e. after each finished ride, `ride_quota += 1` when pay_mode used is __"crypto"__.
* ride price is regulated by Bancor algorithm, which deduces the price automatically w.r.t to available supply of __TOE__ token, __RIDE__ (in no.)  
* Here, before any action is requested, the user has to be checked if it is a verified one.
* Here, `toeridex` contract account is connected via `addridequota` action with `toeridetaxi` contract.
* For different purposes the tokens are sent to the contract by the
	- __token_issuer__ (for initializing RIDEX for __"driver"__ ride type)
	- __token_issuer__ (for initializing RIDEX for __"commuter"__ ride type), 
	- buyer (for buying ride), 