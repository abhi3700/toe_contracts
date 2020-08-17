#pragma once
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
using eosio::datastream;

using std::string;

CONTRACT toeuserauth : public contract {
private:
	const name company_validator_ac;
public:
	using contract::contract;

	toeuserauth(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				company_validator_ac("bhubtoeinval"_n) {}
	

	/**
	 * @brief - add/update user details
	 * @details - add/update user details to the `users` table
	 * 
	 * @param user - driver/commuter name
	 * @param type - driver/commuter
	 * @param profile_hash - add sha256 hash of user identity details - ID proof, address, etc.(more strict for drivers)
	 * @param memo - notes like "add info" "update info"
	 * 
	 * @pre ensure that the name is not in `blacklist` table
	 */
	ACTION creatifyuser( const name& user,
					const name& type,
					const checksum256& profile_hash,
					const string& memo);

	/**
	 * @brief - verify/blacklist dricom_user
	 * @details - verify/blacklist dricom_user as verified/blacklisted by the TOE CARES validators
	 * 
	 * @param validator_user - validator account name
	 * @param dricom_user - driver/commuter account name
	 * @param dricom_user_status - to be verified/blacklisted by TOE CARES validators
	 * @param memo - a note (reason) for the corresponding status, visible 
	 * 					in the user's action via send_receipt() or send_alert() inline actions
	 */
	ACTION vbdricom( const name& validator_user,
						const name& dricom_user,
						const name& dricom_user_status,
						const string& memo );


	/**
	 * @brief - company verify/blacklist validator_user
	 * @details - company verify/blacklist validator_user as verified/blacklisted
	 * 
	 * @param validator_user - validator account name
	 * @param validator_user_status - to be verified/blacklisted by Company validator account i.e. "bhubtoeinval"
	 * @param memo - a note (reason) for the corresponding status, visible 
	 * 					in the user's action via send_receipt() or send_alert() inline actions
	 */
	ACTION compvbvdator( const name& validator_user,
						const name& validator_user_status,
						const string& memo);

	ACTION addrating( const name& user,
						checksum256 ride_id,
						uint32_t 
						uint32_t addfareact_timestamp,
						float rating)

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
		name type;						// driver/commuter/validator
		checksum256 profile_hash;
		name user_status;				// added/updated/verified/blacklisted
		uint32_t add_timestamp;			// timestamp at which the user details is added
		uint32_t update_timestamp;		// timestamp at which the user details is updated
		uint32_t verify_timestamp;		// timestamp at which the user details is verified
		uint32_t blist_timestamp;		// timestamp at which the user is blacklisted
		name validator_verify;			// validator who verifies the user
		name validator_blacklist;		// validator who blacklist the user
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