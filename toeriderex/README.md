# Ride REX contract
## About
* It is a ride resource exchange (REX) contract which is for: 
	- [ ] init ride asset (no. & rate)
	- [ ] set ride rate based on the RTA.
	- [ ] add ride_no to the asset
	- [ ] buy/sell ride(s) by driver/commuter
	- [ ] send receipt, alert to driver/commuter
	- [ ] send trading fees (0.5%) to account - `bhtoerexearn` (owned by company)
* __rides_no.__ is added by no. of rides finished (i.e. after `toeridetaxi::finish` action) i.e. after each finished ride, `ride_quota += 1`.
* __ride_rate__ is controlled by Ride Trading Algorithm (RTA), which takes __fuel price__ into count.

## Contract
* contract name - `toeriderex`
* contract's account name - `toe11riderex`

## Action
- `initrasset`: initialize the asset with rides for driver/commuter.
- `setriderate`: set ride rate for driver/commuter type
- `addridequota`: add ride(s) for each finished ride for driver/commuter type.
- `buyride`: buy ride(s) by driver/commuter
- `sellride`: sell ride(s) by driver/commuter
- `sendreceipt`: send receipt/copy
- `sendalert`: send alert

## Table
- `accounts`: `cleos get table toe11riderex cabeos1dri11 accounts`

| rides_limit | rides_available | rides_used |
|-------------|-----------------|------------|
| 100 | 80 | 20 | 


- `rasset`: `cleos get table toe11riderex toe11riderex assets`

| type | rides_quota | rate_perride |
|------|------|----|
| driver | 1000 | 50 TOE |
| commuter | 2000 | 200 TOE |

- `fuelrates`: `cleos get table toe11riderex toe11riderex fuelrates`

| curr_name | country | price | distance_unit |
|---|---|---|---|
| INR | India | 76 | km

## Compile
## Deploy
## Testing
