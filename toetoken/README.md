# Token contract
## Brief
* It is a token contract which is to 
	- [x] create tokens with issuer & max_supply
	- [x] issue tokens to issuer only
	- [x] retire tokens from supply
	- [x] transfer tokens from one account to another
	- [x] open account with even zero balance
	- [x] close account with zero balance
	- [x] set inflation rate
	- [x] inflate total_supply, not max_supply.
	- [ ] burn

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
	- `setinflation`
	- `inflate`
* table
	- `stats`
	- `accounts`
	- `rates`

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
Warning, action <setinflation> does not have a ricardian contract
Warning, action <inflate> does not have a ricardian contract
Warning, action <create> does not have a ricardian contract
Warning, action <issue> does not have a ricardian contract
Warning, action <retire> does not have a ricardian contract
Warning, action <transfer> does not have a ricardian contract
Warning, action <open> does not have a ricardian contract
Warning, action <close> does not have a ricardian contract
Warning, action <setinflation> does not have a ricardian contract
Warning, action <inflate> does not have a ricardian contract
```
## Deploy
* deploy contract
```console
/toetoken
$ cleost set contract toe1111token ./ -p toe1111token@active
Reading WASM from /mnt/f/Coding/github_repos/toe_contracts/toetoken/toetoken.wasm...
Publishing contract...
executed transaction: 30afb374166fc9cd928648f5701ffbe38de6741854566a3b48797231758b1933  8288 bytes  3182 us
#         eosio <= eosio::setcode               {"account":"toe1111token","vmtype":0,"vmversion":0,"code":"0061736d0100000001ad011d60000060017e00600...
#         eosio <= eosio::setabi                {"account":"toe1111token","abi":"0e656f73696f3a3a6162692f312e31000b076163636f756e7400010762616c616e6...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Testing
* `create`:
```console
$ cleost push action toe1111token create '["bhubtoeindia", "100000000000.0000 TOE"]' -p toe1111token@active
executed transaction: 4bbdddad0230aee2c6669674f50670b83174117ca08b2e0f72c7ed590c26d18d  120 bytes  658 us
#  toe1111token <= toe1111token::create         {"issuer":"bhubtoeindia","maximum_supply":"100000000000.0000 TOE"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* get table after `create` tokens operation
```console
$ cleost get table toe1111token TOE stat
{
  "rows": [{
      "supply": "0.0000 TOE",
      "max_supply": "100000000000.0000 TOE",
      "issuer": "bhubtoeindia"
    }
  ],
  "more": false,
  "next_key": ""
}
```
* `issue`:
```console
$ cleost push action toe1111token issue '["bhubtoeindia", "1000000000.0000 TOE", "issue 1 B TOE tokens"]' -p bhubtoeindia@active
executed transaction: 7e988d203ee809d8361b92f3f6517b95dd2172ef85b22182e6f3dfe182c7cba8  144 bytes  310 us
#  toe1111token <= toe1111token::issue          {"to":"bhubtoeindia","quantity":"1000000000.0000 TOE","memo":"issue 1 B TOE tokens"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```