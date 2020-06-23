## Compile
* Compile the contract
```console
....../base/utility/otp
$ eosio-cpp -I=/usr/local/include -I=/usr/include/c++/7.5.0 -l=/usr/local/lib/libhl++.a otp.cpp -o otp.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <putval> does not have a ricardian contract
```

## Deploy
* Deploy the contract
	- Here, you always need the permission of contract account
```console
....../base/utility/otp
$ cleost set contract cabeos1test1 ./ -p cabeos1test1@active
Reading WASM from /mnt/f/Coding/github_repos/eosio-playground/base/hello/hello.wasm...
Skipping set abi because the new abi is the same as the existing abi
Publishing contract...
executed transaction: 03786ad57bdefa0fc58b10cce0befd391d75e4da7e3904d4ee7e0958e912c5a8  2120 bytes  1534 us
#         eosio <= eosio::setcode               {"account":"cabeos1test1","vmtype":0,"vmversion":0,"code":"0061736d010000000163126000006000017f60027...
warn  2020-06-19T13:12:37.250 cleos     main.cpp:506                  print_result   warning: transaction executed locally, but may not be confirmed by the network yet
```

## Push
* `hiany`: Push the action
	- Here, you need the permission of any account
```console
....../base/utility/otp
$ cleost push action cabeos1test1 hiany '["\"dalvath3700@gmail.com\""]' -p cabeos1user1@active
executed transaction: 5174e79bb3701024157a76e881f3bcb13d83c3c094e7043ee3d41d598f9d8a8f  120 bytes  505 us
#  cabeos1test1 <= cabeos1test1::hiany          {"a":"\"dalvath3700@gmail.com\""}
>> Hello, "dalvath3700@gmail.com"
warn  2020-06-19T13:14:42.052 cleos     main.cpp:506                  print_result   warning: transaction executed locally, but may not be confirmed by the network yet
```
* `hiname`: Push the action
	- Here, you need the permission of contract as it includes require_auth(<eos-username>)
	
		+ Error!
```console
....../base/utility/otp
$ cleost push action cabeos1test1 hiname '["cabeos1test1"]' -p cabeos1user1@active
Error 3090004: Missing required authority
Ensure that you have the related authority inside your transaction!;
If you are currently using 'cleos push action' command, try to add the relevant authority using -p option.
Error Details:
missing authority of cabeos1test1
pending console output:
```

		+ Ok!
```console
....../base/utility/otp
$ cleost push action cabeos1test1 hiname '["cabeos1test1"]' -p cabeos1test1@active
executed transaction: 34bd98ff1b4f052dd01dfa5cd019633e48d190351660804c572057e3eeef2142  104 bytes  545 us
#  cabeos1test1 <= cabeos1test1::hiname         {"user":"cabeos1test1"}
>> Hello, your EOS name is, cabeos1test1
warn  2020-06-19T13:15:53.356 cleos     main.cpp:506                  print_result   warning: transaction executed locally, but may not be confirmed by the network yet
```


