# Wallet contract
## Brief
* It is a contract which is for 
	- [x] send fare via token contract
	- [x] withdraw fare

## About
* contract name - `toeridewallet`
* contract's account name - `toe11rwallet`
* actions
	- `withdrawfare`
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
