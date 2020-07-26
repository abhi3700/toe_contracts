#include <eosio/eosio.hpp>

using eosio::contract;

using std::string;

CONTRACT toeriderex : public contract 
{
public:
	using contract::contract;

	/**
	 * @brief - init ride assets - ride_quota, rate_perride
	 * @details - init ride assets - ride_quota, rate_perride. Applicable only once.
	 * 
	 * @param type - driver/commuter
	 * @param ride_quota - set initial no. of rides in the ride_quota
	 * @param rate_perride - set the initial price (in TOE) per ride. 
	 */
	ACTION initrasset( const name& type,
						uint64_t ride_quota,
						const asset& rate_perride,
						const string& memo);

	/**
	 * @brief - add ride quota
	 * @details - add ride quota after each ride is finished.
	 * 
	 * @param type - driver/commuter
	 * @param ride_quota - no. of rides getting added in the ride_quota, default value = 1, 
	 */
	ACTION addridequota( const name& type,
							uint64_t ride_quota );

	/**
	 * @brief - set ride's rate
	 * @details - set ride's rate for driver/commuter as per 
	 * 				the fuel rate depending on the ride trading algorithm
	 * 
	 * @param type - driver/commuter
	 * @param rate - price in TOE
	 * @param memo - note for setting ride rate
	 */	
	ACTION setriderate( const name& type,
						const asset& rate,
						const string& memo );


	/**
	 * @brief - buy rides from market
	 * @details - buy rides from market
	 * 
	 * @param buyer - any registered user (driver/commuter/validator)
	 * @param ride_qty - no. of rides to be bought
	 * @param memo - note for buying ride
	 */	
	ACTION buyride( const name& buyer,
					uint64_t ride_qty,
					const string& memo );

	/**
	 * @brief - sell rides from market
	 * @details - sell rides from market
	 * 
	 * @param buyer - any registered user (driver/commuter/validator)
	 * @param ride_qty - no. of rides to be sold
	 * @param memo - note for selling ride
	 */	
	ACTION sellride( const name& seller,
					uint64_t ride_qty,
					const string& memo)

	/**
	 * @brief - send alert
	 * @details - send alert after the action is successfully done. e.g. driver alerting commuter that the vehicle has arrived
	 * 
	 * @param user - driver/commuter
	 * @param message - note depending on the action
	 */
	ACTION sendalert( const name& user,
						const string& message);

	/**
	 * @brief - send receipt or a copy
	 * @details - send receipt after any action is successfully done
	 * 
	 * @param user - driver/commuter
	 * @param message - note depending on the action
	 */
	ACTION sendreceipt( const name& user,
						const string& message);

private:
	// --------------------------------------------------------------------------------
	TABLE account {
		uint64_t rides_limit;
		uint64_t rides_available;
		uint64_t rides_used;

		auto primary_key() const { return rides_limit; }
	};

	using account_index = multi_index<"accounts"_n, account>;
	// --------------------------------------------------------------------------------
	TABLE rasset {
		name type;
		uint64_t ride_quota;
		asset rate_perride;

		auto primary_key() const { return type.value; }
	};

	using rasset_index = multi_index<"rasset"_n, rasset>;
	// --------------------------------------------------------------------------------
	TABLE fuelrate {
		name curr_name;
		string country;
		float price;
		string distance_unit;

		auto primary_key() const { return curr_name.value; }
	};

	using fuelrate_index = multi_index<"fuelrates", fuelrate>;

};