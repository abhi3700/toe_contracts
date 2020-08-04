# Wallet contract
## Brief
* It is a contract which is for 
	- [x] send fare via token contract
	- [x] withdraw fare

## About
* contract name - `toeridewallet`
* contract's account name - `toe11rwallet`
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
$ cleost set contract toe11rwallet ./
Reading WASM from /mnt/f/Coding/github_repos/toe_contracts/toeridewallet/toeridewallet.wasm...
Skipping set abi because the new abi is the same as the existing abi
Publishing contract...
executed transaction: 45063e35bf65319a6aee54853e0fe6c087cf587f9f40c21bf1597c49b482e785  8456 bytes  9078 us
#         eosio <= eosio::setcode               {"account":"toe11rwallet","vmtype":0,"vmversion":0,"code":"0061736d0100000001b7011d60000060027f7f006...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* Adding eosio.code to permissions (for inline actions)
```
$ cleost set account permission toe11rwallet active --add-code
executed transaction: 7fe84034ebc26e5f981d26163f1c3d03581b06fa4df88a6dbec7397cf8a5f8f1  184 bytes  181 us
#         eosio <= eosio::updateauth            {"account":"toe11rwallet","permission":"active","parent":"owner","auth":{"threshold":1,"keys":[{"key...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Testing
