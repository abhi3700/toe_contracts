#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <iterator>

#include <string>

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
using std::size_t;

CONTRACT toeridex : public contract 
{
private:
	// const name token_issuer;
	const symbol ride_token_symbol;
	const float supply_factor;
	const float fees_factor;
	const name ride_contract_ac;
	const name token_contract_ac;
	const name ridex_supply_ac;
	const name ridex_fees_ac;
	const name auth_contract_ac;
	const uint64_t initial_ride_qty;


public:
	using contract::contract;

	toeridex(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds),
				// token_issuer("bhubtoeindia"_n),
				ride_token_symbol("TOE", 4),
				supply_factor(0.99),	// 99%
				fees_factor(0.01),		// 1%
				ride_contract_ac("toe1ridetaxi"_n),
				token_contract_ac("toe1111token"_n),
				ridex_supply_ac("toeridexsupp"_n),
				ridex_fees_ac("toeridexfees"_n),
				auth_contract_ac("toe1userauth"_n),
				initial_ride_qty(1000000) {}

	/**
	 * @brief - send money for 3 purposes: init RIDEX (for driver, commuter ride types)
	 * @details - send money for 3 purposes:
	 * 				+ init RIDEX (for driver, commuter ride types)
	 * 				+ buying ride
	 * 
	 * @sender - sender i.e. bhubtoeindia/ buyer/ seller
	 * @contract_ac - this contract
	 * @param quantity - initialize RIDEX supply quantity in TOE
	 * @param memo - remarks
	 * 
	 * @pre memo must contain either "driver" or "commuter" or "buy" or "sell"
	 *
	 */
	[[eosio::on_notify("toe1111token::transfer")]]
	void sendridex( const name& sender,
						const name& contract_ac,
						const asset& quantity,
						const string& memo);





	/**
	 * @brief - buy rides from RIDEX
	 * @details - buy rides from RIDEX
	 * 
	 * @param buyer - any registered user (driver/commuter/validator)
	 * @ride_type - driver/commuter
	 * @param ride_qty - no. of rides to be bought
	 * @param memo - note for buying ride
	 */	
	ACTION buyride( const name& buyer,
					const name& ride_type,
					uint64_t ride_qty,
					const string& memo );

	/**
	 * @brief - sell rides from RIDEX
	 * @details - sell rides from RIDEX
	 * 
	 * @param buyer - any registered user (driver/commuter/validator)
	 * @ride_type - driver/commuter
	 * @param ride_qty - no. of rides to be sold
	 * @param memo - note for selling ride
	 */	
	ACTION sellride( const name& seller,
					const name& ride_type,
					uint64_t ride_qty,
					const string& memo);

	/**
	 * @brief - on finishing a ride with __"crypto"__ pay_mode, the ride quota gets added
	 * @details - the ride quota gets added on finishing a ride from toeridetaxi contract for __"crypto"__ pay_mode
	 * 			- only accessed by 'ride_contract_ac'
	 * 
	 * @param ride_type - driver/commuter
	 * @param ride_qty - increase ride_quota by ride_qty
	 */
	ACTION addridequota(const name& ride_type, 
						uint64_t ride_qty);

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

	static void check_buyer_seller( const name& user, const name& ride_type) {
		// check the buyer/seller is enlisted in the `toeuserauth` contract table
		user_index user_table("toe1userauth"_n, user.value);
		auto user_it = user_table.find(user.value);

		check(user_it != user_table.end(), "Sorry! The user is not registered with us.");
		check(user_it->user_status == "verified"_n, "Sorry! The user is not yet verified.");
		check((user_it->type == "driver"_n) || 
			(user_it->type == "commuter"_n) || 
			(user_it->type == "validator"_n)
			, "Other types are not allowed for this action");

		// check for validator can buy/sell only "commuter" type ride
		if(user_it->type == "validator"_n) {
			check(ride_type == "commuter"_n, "Sorry! A validator can't buy \'driver\' type rides.");
		} 
		// check for commuter can buy/sell only "commuter" type ride
		else if(user_it->type == "commuter"_n) {
			check(ride_type == "commuter"_n, "Sorry! A commuter can't buy \'driver\' type rides.");
		} 
		// check for driver can buy/sell only "commuter" type ride
		else {
			check(ride_type == "driver"_n, "Sorry! A driver can't buy \'commuter\' type rides.");
		}
	}

	// --------------------------------------------------------------------------------
	static void check_quantity( const asset& quantity ) {
		check(quantity.is_valid(), "invalid quantity");
		check(quantity.amount > 0, "must withdraw positive quantity");
		check(quantity.symbol == symbol("TOE", 4), "symbol precision mismatch");
	}

	using addridequota_action  = action_wrapper<"addridequota"_n, &toeridex::addridequota>;

private:

	// --------------------------------------------------------------------------------
	// RIDEX
	TABLE ridex {
		name ride_type;
		uint64_t ride_quota;
		asset toe_balance;

		auto primary_key() const { return ride_type.value; }
	};

	using ridex_index = multi_index<"ridex"_n, ridex>;
	
	// --------------------------------------------------------------------------------
	// RIDEX User wallet
	TABLE rexusrwallet {
		asset balance;

		auto primary_key() const { return balance.symbol.raw(); }
	};

	using rexusrwallet_index = multi_index<"rexusrwallet"_n, rexusrwallet>;

	// --------------------------------------------------------------------------------
	// RIDEX Fee wallet
	TABLE rexfeewallet {
		asset balance;

		auto primary_key() const { return balance.symbol.raw(); }
	};

	using rexfeewallet_index = multi_index<"rexfeewallet"_n, rexfeewallet>;
	// --------------------------------------------------------------------------------
	// RIDEX User account
	TABLE rexuseraccnt {
		name ride_type;
		uint64_t rides_limit;

		auto primary_key() const { return ride_type.value; }
	};

	using rexuseraccnt_index = multi_index<"rexuseraccnt"_n, rexuseraccnt>;

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
	
	// ------------------------------------------------------------------
	struct account
	{
		asset balance;

		uint64_t primary_key() const { return balance.symbol.code().raw(); }
	};

	using accounts_index = eosio::multi_index< "accounts"_n, account >;

	// --------------------------------------------------------------------------------
	struct currency_stats
	{
		asset supply;
		asset max_supply;
		name issuer;
		
		uint64_t primary_key() const { return supply.symbol.code().raw(); }
	};

	using stats_index = eosio::multi_index< "stat"_n, currency_stats >;

	// ========Functions========================================================================================================
	// Adding inline action for `sendalert` action in the same contract 
	void send_alert(const name& user, const string& message);

	// Adding inline action for `sendreceipt` action in the same contract   
	void send_receipt(const name& user, const string& message);
};