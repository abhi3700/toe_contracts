# Wallet contract
## Brief
* It is a contract which is for 
	- [x] send fare via token contract
	- [x] withdraw fare

## About
* contract name - `toeridewallet`
* contract's account name - `toe14rwallet`
* actions
	- `sendfare` (on_notify)
	- `withdrawfare`
	- `withdrawfull`
	- `sendreceipt`

* table
	- `ridewallet`

## Compile
```console
/cabeos_contracts/toeridewallet$ eosio-cpp src/toeridewallet.cpp -o toeridewallet.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <withdraw> does not have a ricardian contract
Warning, action <withdrawfull> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
Warning, action <withdraw> does not have a ricardian contract
Warning, action <withdrawfull> does not have a ricardian contract
Warning, action <sendreceipt> does not have a ricardian contract
```

## Deploy
```console
$ cleost set contract toe14rwallet ./
Reading WASM from /mnt/f/Coding/github_repos/toe_contracts/toeridewallet/toeridewallet.wasm...
Publishing contract...
executed transaction: 2a02b0e04e336bf559e71ed1cbad16c1d10c83eff310e9beb63a57f2a292572e  9152 bytes  3370 us
#         eosio <= eosio::setcode               {"account":"toe14rwallet","vmtype":0,"vmversion":0,"code":"0061736d0100000001be011e60000060017f00600...
#         eosio <= eosio::setabi                {"account":"toe14rwallet","abi":"0e656f73696f3a3a6162692f312e3100040a7269646577616c6c657400010762616...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* Adding eosio.code to permissions (for inline actions)
```
$ cleost set account permission toe14rwallet active --add-code
executed transaction: e0617a317b186b0c027fae7e8878a59d09e0bd838cbce8ca99a2e0360295dcea  184 bytes  211 us
#         eosio <= eosio::updateauth            {"account":"toe14rwallet","permission":"active","parent":"owner","auth":{"threshold":1,"keys":[{"key...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Testing
### Action - `sendfare` (payable action)
* `toecom111111` use `sendfare` to transfer fare amount using `crypto` pay_mode before ride to the contract - `toe11rwallet`
```console
$ cleost push action toe1111token transfer '["toecom111111", "toe14rwallet" "15.8000 TOE", "transfer 15.8 TOE to ride wallet"]' -p toecom111111@active
executed transaction: d263836768605d7305a822d2098c77039271a7769ac8f5f0846323eb212dd048  160 bytes  1111 us
#  toe1111token <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"15.8000 TOE","memo":"transfer 15.8 TOE to rid...
#  toecom111111 <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"15.8000 TOE","memo":"transfer 15.8 TOE to rid...
#  toe14rwallet <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"15.8000 TOE","memo":"transfer 15.8 TOE to rid...
#  toe14rwallet <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 transfers an amount of 15.8000 TOE to the contract ->...
#  toecom111111 <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 transfers an amount of 15.8000 TOE to the contract ->...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toecom111111` transfers more amount to wallet
```console
$ cleost push action toe1111token transfer '["toecom111111", "toe14rwallet" "0.0001 TOE", "transfer 0.0001 TOE to ride wallet"]' -p toecom111111@active
executed transaction: 89e9b4ca82d489a254b1366e0f674a321244415aa904cc0661b9365fdcb5fcb8  160 bytes  947 us
#  toe1111token <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"0.0001 TOE","memo":"transfer 0.0001 TOE to ri...
#  toecom111111 <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"0.0001 TOE","memo":"transfer 0.0001 TOE to ri...
#  toe14rwallet <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"0.0001 TOE","memo":"transfer 0.0001 TOE to ri...
#  toe14rwallet <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 transfers an amount of 0.0001 TOE to the contract -> ...
#  toecom111111 <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 transfers an amount of 0.0001 TOE to the contract -> ...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* `toecom111111` transfers more amount to wallet
```console
$ cleost push action toe1111token transfer '["toecom111111", "toe14rwallet" "0.0001 TOE", "transfer 0.0001 TOE to ride wallet"]' -p toecom111111@active
executed transaction: 6708b0cd157db996eb1985dfc895bc2628e3c5a7c1598058a6928c1dc2e56a04  160 bytes  778 us
#  toe1111token <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"0.0001 TOE","memo":"transfer 0.0001 TOE to ri...
#  toecom111111 <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"0.0001 TOE","memo":"transfer 0.0001 TOE to ri...
#  toe14rwallet <= toe1111token::transfer       {"from":"toecom111111","to":"toe14rwallet","quantity":"0.0001 TOE","memo":"transfer 0.0001 TOE to ri...
#  toe14rwallet <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 transfers an amount of 0.0001 TOE to the contract -> ...
#  toecom111111 <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 transfers an amount of 0.0001 TOE to the contract -> ...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* view the `ridewallet` table after 3 transfers by `toecom111111`
```console
$ cleost get table toe14rwallet toecom111111 ridewallet
{
  "rows": [{
      "balance": "15.8002 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```

### Action - `withdraw`
* `toecom111111` withdraws __"10.0001 TOE"__ money from __"15.8000 TOE"__ balance `ridewallet` table stored in `toe14rwallet` contract
```console
$ cleost push action toe14rwallet withdraw '["toecom111111", "10.0001 TOE"]' -p toecom111111@active
executed transaction: 09ae90d707282f5f1359aa8d481263b4f52ba9e2200cb74da0d3e78626d67d89  120 bytes  777 us
#  toe14rwallet <= toe14rwallet::withdraw       {"commuter_ac":"toecom111111","quantity":"10.0001 TOE"}
#  toe1111token <= toe1111token::transfer       {"from":"toe14rwallet","to":"toecom111111","quantity":"10.0001 TOE","memo":"commuter withdraws 10.00...
#  toe14rwallet <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 withdraws 10.0001 TOE amount."}
#  toe14rwallet <= toe1111token::transfer       {"from":"toe14rwallet","to":"toecom111111","quantity":"10.0001 TOE","memo":"commuter withdraws 10.00...
>> Either money is not sent to the contract or contract itself is the commuter.
#  toecom111111 <= toe1111token::transfer       {"from":"toe14rwallet","to":"toecom111111","quantity":"10.0001 TOE","memo":"commuter withdraws 10.00...
#  toecom111111 <= toe14rwallet::sendreceipt    {"user":"toecom111111","message":"toecom111111 withdraws 10.0001 TOE amount."}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* check if the withdrawn balance is added to token contract table for commuter - `toecom111111`
```console
$ cleost get table toe1111token toecom111111 accounts
{
  "rows": [{
      "balance": "14.2001 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* check if the withdrawn balance is substracted from __"15.8000 TOE"__ balance `ridewallet` table for `toecom111111` commuter
```console
$ cleost get table toe14rwallet toecom111111 ridewallet
{
  "rows": [{
      "balance": "5.7999 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
```


### Action - `withdrawfull`
* `toecom111111` withdraws all the money from `ridewallet` table stored in `toe14rwallet` contract
```console
$ cleost push action toe14rwallet withdrawfull '['toecom111111']' -p toecom111111@active
executed transaction: 3a96efbb98b5df0396dd6d87e7cde9dd4f0816cafb178af01d084f3ae3f4da9b  104 bytes  901 us
#  toe14rwallet <= toe14rwallet::withdrawfull   {"commuter_ac":"toecom111111"}
#  toe1111token <= toe1111token::transfer       {"from":"toe14rwallet","to":"toecom111111","quantity":"15.8002 TOE","memo":"commuter withdraws 15.80...
#  toe14rwallet <= toe1111token::transfer       {"from":"toe14rwallet","to":"toecom111111","quantity":"15.8002 TOE","memo":"commuter withdraws 15.80...
>> Either money is not sent to the contract or contract itself is the commuter.
#  toecom111111 <= toe1111token::transfer       {"from":"toe14rwallet","to":"toecom111111","quantity":"15.8002 TOE","memo":"commuter withdraws 15.80...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* check the table, if the data is erased after successful transfer back to commuter
```console
$ cleost get table toe14rwallet toecom111111 ridewallet
{
  "rows": [],
  "more": false,
  "next_key": ""
}
```
* Also, check the balance of `toecom111111` is equal to the initial balance i.e. __"20.0000 TOE"__ in token contract
```console
$ cleost get table toe1111token toecom111111 accounts
{
  "rows": [{
      "balance": "20.0000 TOE"
    }
  ],
  "more": false,
  "next_key": ""
}
``` 
