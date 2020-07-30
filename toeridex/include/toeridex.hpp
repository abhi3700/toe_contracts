#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <iterator>

using eosio::contract;
using eosio::print;
using eosio::name;
using eosio::multi_index;
using eosio::const_mem_fun;
using eosio::indexed_by;
using eosio::asset;
using eosio::check;
using eosio::permission_level;
using eosio::datastream;
using eosio::action;
using eosio::same_payer;
using eosio::symbol;
using eosio::require_recipient;
using eosio::action_wrapper;
using eosio::checksum256;

using std::string;

CONTRACT toeridex : public contract 
{
private:
	const name token_issuer;
	const symbol ride_token_symbol;
	const float supply_factor;
	const float fees_factor;


public:
	using contract::contract;

	toeridex(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds),
				token_issuer("bhubtoeindia"_n),
				ride_token_symbol("TOE", 4),
				supply_factor(0.995),
				fees_factor(0.005) {}

	/**
	 * @brief - initialize RIDEX params
	 * @details - initialize RIDEX params
	 * 				+ toe_balance
	 * 				+ ride_quota
	 * 
	 * @type - driver/commuter
	 * @param toe_qty - quantity in TOE
	 * @param ride_qty - ride quantity
	 *
	 */
	ACTION initridex( const name& type,
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
					const string& memo);

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
	TABLE ridexaccount {
		name type;
		uint64_t rides_limit;

		auto primary_key() const { return type.value; }
	};

	using ridexaccount_index = multi_index<"ridexaccount"_n, ridexaccount>;
	// --------------------------------------------------------------------------------
	TABLE ridex {
		name type;
		uint64_t ride_quota;
		asset toe_balance;

		auto primary_key() const { return type.value; }
	};

	using ridex_index = multi_index<"ridex"_n, ridex>;
	
	// --------------------------------------------------------------------------------
	struct user {
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
	
	// --------------------------------------------------------------------------------
	struct currency_stats
	{
		asset supply;
		asset max_supply;
		name issuer;
		
		uint64_t primary_key() const { return supply.symbol.code().raw(); }
	};

	using stats_index = eosio::multi_index< "stats"_n, currency_stats >;

	// ========Functions========================================================================================================
	// Adding inline action for `sendalert` action in the same contract 
	void send_alert(const name& user, const string& message);

	// Adding inline action for `sendreceipt` action in the same contract   
	void send_receipt(const name& user, const string& message);
};