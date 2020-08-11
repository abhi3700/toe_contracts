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
### Action - `initridex`
* `bhubtoeindia` account initializing the RIDEX
```console
cleost push action toe1111ridex initridex '["driver", "1000000.0000 TOE", "1000000"]' -p bhubtoeindia@active
```

## NOTES
* __rides_no.__ is added by no. of rides finished (i.e. after `toeridetaxi::finish` action) i.e. after each finished ride, `ride_quota += 1` when pay_mode used is __"crypto"__.
* ride price is regulated by Bancor algorithm, which deduces the price automatically w.r.t to available supply of __TOE__ token, __RIDE__ (in no.)  
* Here, before any action is requested, the user has to be checked if it is a verified one.
* Here, `toeridex` contract account is connected via `addridequota` action with `toeridetaxi` contract.
