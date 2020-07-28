#include <eosio/eosio.hpp>

using eosio::contract;

using std::string;

CONTRACT toeriderex : public contract 
{
public:
	using contract::contract;

	/**
	 * @brief - initialize RIDEX params
	 * @details - initialize RIDEX params
	 * 				+ toe_owner
	 * 				+ toe_qty
	 * 				+ ride_qty
	 * 
	 * @param toe_owner - company owner - "bhubtoeindia"
	 * @type - driver/commuter
	 * @param toe_qty - quantity in TOE
	 * @param ride_qty - ride quantity
	 *
	 */
	ACTION initridex( const name& toe_owner,
						const name& type,
						const asset& toe_qty,
						uint64_t ride_qty );

	/**
	 * @brief - buy rides from RIDEX
	 * @details - buy rides from RIDEX
	 * 
	 * @param buyer - any registered user (driver/commuter/validator)
	 * @type - driver/commuter
	 * @param ride_qty - no. of rides to be bought
	 * @param memo - note for buying ride
	 */	
	ACTION buyride( const name& buyer,
					const name& type,
					uint64_t ride_qty,
					const string& memo );

	/**
	 * @brief - sell rides from RIDEX
	 * @details - sell rides from RIDEX
	 * 
	 * @param buyer - any registered user (driver/commuter/validator)
	 * @type - driver/commuter
	 * @param ride_qty - no. of rides to be sold
	 * @param memo - note for selling ride
	 */	
	ACTION sellride( const name& seller,
					const name& type,
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
	TABLE ridex {
		name type;
		uint64_t ride_quota;
		asset toe_balance;

		auto primary_key() const { return type.value; }
	};

	using ridex_index = multi_index<"ridex"_n, ridex>;
	// --------------------------------------------------------------------------------

};