# Token contract
## Brief
* It is a token contract which is to 
	- [x] create tokens with issuer & max_supply
	- [x] issue tokens to issuer only
	- [x] retire tokens from supply
	- [x] transfer tokens from one account to another
	- [x] open account with even zero balance
	- [x] close account with zero balance

## About
* contract name - `toetoken`
* contract's account name - `toe1111token`
* action
	- `create`
	- `issue`
	- `retire`
	- `transfer`
	- `open`
	- `close`
* table
	- `stats`
	- `accounts`

## Workflow
* The account `toe1111token` is where the contract is uploaded to.
* “Every token must be issued by an `issuer` account.” [Souce](https://link.medium.com/1G4zitL6t7)
* The contract account `toe1111token` with its permission creates a `issuer` & asset (quantity, symbol)`max_supply` using `create()` action.
	- eg-1: `eosio` & `1000000000.0000 EOS` in EOS mainnet
	- eg-1: `junglefaucet` & `1000000000.0000 EOS` in Jungle testnet
	- eg-2: `bhubindtoeio` & `1000000.0000 TOE` in Jungle Testnet
* Now, any token transferred to a user (driver/commuter) needs permission of the issuer (`bhubindtoeio`). For this, use `transfer()` action.
	- eg-1: `"10.0000 TOE"` is transferred to `toeuser111111` by the permission of `bhubindtoeio`
* Now, amount can be transferred from one account to another.
	- eg-1: `"2.0000 TOE"` is transferred from `toeuser111111` to `toeuser111112`
* `open`/`close` actions are to open/close the zero balance accounts.
	- For more, read [1](https://github.com/EOSIO/eosio.contracts/issues/57), [2](https://github.com/EOSIO/eosio.contracts/issues/61), [3](https://github.com/EOSIO/eosio.contracts/issues/62) 
	- that's why, the RAM is now increased a little bit. Previously, it used to be around 4 KB, but now it is around 5 KB. So, the user has to pay little extra during the account creation for this extra RAM paid by the account creator (by existing account).
* Any amount issued to the issuer is added as supply field in the `stats` table.


## Compile
```console
$ eosio-cpp src/toetoken.cpp -o toetoken.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <create> does not have a ricardian contract
Warning, action <issue> does not have a ricardian contract
Warning, action <retire> does not have a ricardian contract
Warning, action <transfer> does not have a ricardian contract
Warning, action <open> does not have a ricardian contract
Warning, action <close> does not have a ricardian contract
Warning, action <create> does not have a ricardian contract
Warning, action <issue> does not have a ricardian contract
Warning, action <retire> does not have a ricardian contract
Warning, action <transfer> does not have a ricardian contract
Warning, action <open> does not have a ricardian contract
Warning, action <close> does not have a ricardian contract
```
## Deploy

## Testing
* `create`:
```console
$ cleost push action toe1111token create '["bhubindtoeio", "1000000.0000 EOS"]' -p toe1111token@active
```