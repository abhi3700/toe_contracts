# Ride REX contract
## About
* It is a ride resource exchange (REX) contract which is for: 
	- [ ] init RIDEX (ride_qty, toe_balance)
	- [ ] driver/commuter buy/sell ride(s) from RIDEX
	- [ ] send receipt, alert to driver/commuter
	- [ ] send trading fees (0.5%) to account - `toeridexfees` (owned by company)
* __rides_no.__ is added by no. of rides finished (i.e. after `toeridetaxi::finish` action) i.e. after each finished ride, `ride_quota += 1`.
* __ride_rate__ is controlled by Ride Trading Algorithm (RTA), which takes __fuel price__ into count.

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
- `accounts`: `cleos get table toe1111ridex cabeos1dri11 accounts`

| rides_limit | rides_outstanding |
|-------------|-----------------|
| 100 | 80 |


- `ridex`: `cleos get table toe1111ridex toe1111ridex assets`

| type | rides_quota | toe_balance |
|------|------|----|
| driver | 1000 | 50 TOE |
| commuter | 2000 | 200 TOE |


## Compile
## Deploy
## Testing
