#include <eosio/eosio.hpp>
#include <eosio/crypto.hpp>
#include <eosio/system.hpp>

#include <string>

using eosio::contract;
using eosio::check;
using eosio::multi_index;
using eosio::name;
using eosio::indexed_by;
using eosio::const_mem_fun;
using eosio::checksum256;
using eosio::has_auth;
using eosio::current_time_point;
using eosio::same_payer;
using eosio::action;
using eosio::permission_level;

using std::string;

CONTRACT toeuserauth : public contract {
public:
	using contract::contract;
	/**
	 * @brief - add user
	 * @details - add user to the `users` table
	 * 
	 * @param user - driver/commuter name
	 * @param type - driver/commuter
	 * @param profile_hash - add sha256 hash of user identity details - ID proof, address, etc.(more strict for drivers)
	 * 
	 * @pre ensure that the name is not in `blacklist` table
	 */
	ACTION adduser( const name& user,
					const name& type,
					const checksum256& profile_hash);

	/**
	 * @brief - add/edit user status
	 * @details - add/edit user status (added/verified/blacklisted) by the TOE CARES
	 * 
	 * @param user - driver/commuter account
	 * @param user_status - added/verified/blacklisted
	 * @param memo - a note (reason) for the corresponding status, visible 
	 * 					in the user's action via send_receipt() or send_alert() inline actions
	 */
	ACTION addusrstatus( const name& user,
						 const name& user_status,
						 const string& memo );

	/**
	 * @brief - delete user
	 * @details - delete user from the `user` table
	 * 
	 * @param user - driver/commuter
	 * @param memo - a note (reason) for the corresponding status, visible 
	 * 					in the user's action via send_receipt() or send_alert() inline actions
	 */
	ACTION deluser( const name& user,
					const string& memo);

	/**
	 * @brief - send alert
	 * @details - send alert after the action is successfully done. e.g. a driver is alerted for getting blacklisted
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
	// ------------------------------------------------------------------------------------------------
	TABLE user {
		name user;
		name type;
		checksum256 profile_hash;
		name user_status;				// added or verified or blacklisted
		uint32_t add_timestamp;			// timestamp at which the user data is added
		uint32_t auth_timestamp;		// timestamp at which the user data is authenticated/validated
		uint32_t blist_timestamp;		// timestamp at which the user is blacklisted
		float rating;
		uint64_t ride_total;
		uint64_t ride_rated;

		auto primary_key() const { return user.value; }
		uint64_t get_secondary_1() const { return type.value; }
		uint64_t get_secondary_2() const { return user_status.value; }
	};

	using user_index = multi_index<"users"_n, user,
						indexed_by<"bytype"_n, const_mem_fun<user, uint64_t, &user::get_secondary_1>>,
						indexed_by<"bystatus"_n, const_mem_fun<user, uint64_t, &user::get_secondary_2>>
						>;

	// =======Functions========================================================================================================
	// Adding inline action for `sendalert` action in the same contract 
	void send_alert(const name& user, const string& message);

	// Adding inline action for `sendreceipt` action in the same contract   
	void send_receipt(const name& user, const string& message);

	// get the current timestamp
	inline uint32_t now() const {
		return current_time_point().sec_since_epoch();
	}


};