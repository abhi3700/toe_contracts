## Contracts Security (for CabEOS)
* If the ride is booked & the action is repeatedly pushed with the same data, it should not consume RAM or CPU, or NET (either of these). It could be a technical glitch.
	- Sol: use check() func to ensure that the ride info data (src, des loc) if found in table then contract should not be executed.

* To use the contract actions, a user (driver/commuter) has to be among the users list (found in 'users' table of  ride contract)
	- make 2 additional tables:
		+ tabdriver
		+ tabcommuter