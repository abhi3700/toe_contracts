#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
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

using std::string;

CONTRACT toeridex : public contract 
{
private:
	const symbol ride_token_symbol;

public:
	using contract::contract;

	toeridex(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4) {}

	/**
	 * @brief - initialize RIDEX params
	 * @details - initialize RIDEX params
	 * 				+ toe_issuer
	 * 				+ toe_qty
	 * 				+ ride_qty
	 * 
	 * @param toe_issuer - company owner - "bhubtoeindia"
	 * @type - driver/commuter
	 * @param toe_qty - quantity in TOE
	 * @param ride_qty - ride quantity
	 *
	 */
	ACTION initridex( const name& toe_issuer,
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
	
	// ========Functions========================================================================================================
	// Adding inline action for `sendalert` action in the same contract 
	void send_alert(const name& user, const string& message);

	// Adding inline action for `sendreceipt` action in the same contract   
	void send_receipt(const name& user, const string& message);
};