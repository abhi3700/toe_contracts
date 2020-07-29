TOE Smart Contracts
===================
v0.0.7 - `30 Jul 2020 02:54`
----
* added "string length check" in message param of `sendalert`, `sendreceipt` actions for all the contracts.
* __RIDEX__ implemented using Bancor Algorithm.
* `toeuserauth` contract added & successfully compiled.
* `reachsrc_timestamp_est`, `reachsrc_timestamp_act` added in the `ridetaxi` table of `toeridetaxi` contract.
* added __RIDEX__ related notes (for technical whitepaper) in the repo README file.
* `toeridetaxi` contract linked to `toeuserauth` contract.
* `#pragma once` line added in all the contract header files to avoid header file repetition during the compilation process.

v0.0.6 - `23 Jul 2020 10:30`
----
* `toeridetaxi` contract README updated with TODO tasks
* `toetoken` contract added & successfully compiled

v0.0.5 - `22 Jul 2020 01:38`
----
* NOTES added for technical whitepaper in the root __README.md__ file.
* `toetoken` contract added & successfully compiled

v0.0.4 - `21 Jul 2020 21:52`
----
* type of `fare_est` & `fare_act` changed from `double` (8 bytes) to `float` (4 bytes) due to high memory usage earlier. Moreover, high precision using `double` (upto 15 decimal digits) is not needed here in `fare`, rather `float` (upto 7 decimal digits) is enough.
* @param `fare_crypto` of `asset` type added in the `toeridetaxi` table -- `ridetaxi`.
* `cabeostoken` updated to `toetoken`

v0.0.3 - `19 Jul 2020 18:25`
----
* `toeridewallet` contract created to isolate the ride wallet from `toeridetaxi` contract. This is to ensure that wallet is independently in communication with taxi, bus, metro contracts later.
* `toeridetaxi` contract modified to reading data from other table of `toeridewallet` contract

v0.0.2 - `21 Jul 2020 21:52`
----
* `toeridetaxi` contract is written & released after successful compilation.


v0.0.1 - `17 Jun 2020 20:16`
----
* All the contract folders were created:
	- `cabeos.ride`
	- `cabeos.stake`
	- `cabeos.status`
	- `cabeos.token`