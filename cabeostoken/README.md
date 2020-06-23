## Workflow
* The account `cabeos1token` is where the contract is uploaded to.
* “Every token must be issued by an `issuer` account.” [Souce](https://link.medium.com/1G4zitL6t7)
* The contract account `cabeos1token` with its permission creates a `issuer` & asset (quantity, symbol)`max_supply` using `create()` action.
	- eg-1: `eosio` & `1000000000.0000 EOS` in EOS mainnet
	- eg-1: `junglefaucet` & `1000000000.0000 EOS` in Jungle testnet
	- eg-2: `cabeosoneind` & `1000000.0000 CAB` in Jungle Testnet
* Now, any token issued to a user (driver/commuter) needs permission of the issuer (`cabeosoneind`). For this, use `issue()` action.
	- eg-1: `"10.0000 CAB"` is issued to `cabeos1user1` by the permission of `cabeosoneind`
* Now, amount can be transferred from one account to another.
	- eg-1: `"2.0000 CAB"` is transferred from `cabeos1user1` to `cabeos1user2`
* `open`/`close` actions are to open/close the zero balance accounts.
	- For more, read [1](https://github.com/EOSIO/eosio.contracts/issues/57), [2](https://github.com/EOSIO/eosio.contracts/issues/61), [3](https://github.com/EOSIO/eosio.contracts/issues/62) 
	- that's why, the RAM is now increased a little bit. Previously, it used to be around 4 KB, but now it is around 5 KB. So, the user has to pay little extra during the account creation for this extra RAM paid by the account creator (by existing account).

## Compile
## Deploy
## Push
* `create`:
```console
$ cleost push action cabeos1token create '["cabeosoneind", "1000000.0000 EOS"]' -p cabeos1token@active
```