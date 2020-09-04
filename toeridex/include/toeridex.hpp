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
	// const float supply_factor;
	const float fees_factor;
	const name ridetaxi_contract_ac;
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
				// supply_factor(0.99),	// 99%
				fees_factor(0.01),		// 1%
				ridetaxi_contract_ac("toe1ridetaxi"_n),
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
	 * @param buyer_type - buyer type (driver/commuter/validator)
	 * @param ride_type - driver/commuter
	 * @param ride_qty - no. of rides to be bought
	 * @param memo - note for buying ride
	 */	
	ACTION buyride( const name& buyer,
					const name& buyer_type,
					const name& ride_type,
					uint64_t ride_qty,
					const string& memo );

	/**
	 * @brief - sell rides from RIDEX
	 * @details - sell rides from RIDEX
	 * 
	 * @param seller - any registered user (driver/commuter/validator)
	 * @param seller_type - buyer type (driver/commuter/validator)
	 * @ride_type - driver/commuter
	 * @param ride_qty - no. of rides to be sold
	 * @param memo - note for selling ride
	 */	
	ACTION sellride( const name& seller,
					const name& seller_type,
					const name& ride_type,
					uint64_t ride_qty,
					const string& memo);


	/**
	 * @brief - use ride
	 * @details - commuter use ride at 2 stages:
	 * 				+ create
	 * 				+ changedes
	 * 			- driver use ride at 1 stage:
	 * 				+ start
	 * 				
	 * @param user - account
	 * @param user_type - commuter or driver
	 * @param ride_type - commuter or driver
	 * @param ride_qty - should be 1
	 */
	ACTION consumeride( const name& user,
						const name& user_type,
						const name& ride_type,
						uint64_t ride_qty );

	/**
	 * @brief - restore ride
	 * @details - commuter use ride at 1 stage:
	 * 				+ changedes
	 * 				+ cancelbycom
	 * 			- driver use ride at 1 stage:
	 * 				+ cancelbycom
	 * @param user - account
	 * @param user_type - commuter
	 * @param ride_type - commuter or driver
	 * @param ride_qty - should be 1
	 */
	ACTION restoreride( const name& user,
						const name& user_type,
						const name& ride_type,
						uint64_t ride_qty );

	/**
	 * @brief - transfer RIDEX asset i.e. rides
	 * @details - transfer RIDEX asset i.e. rides from one account to another. 
	 * 			- only accessed by validators also holding account as commuter.
	 * 
	 * @param sender - sender
	 * @param receiver - receiver
	 * @param ride_type - "driver" or "commuter"
	 * @param ride_qty - can be any no. but <= ride_limit of sender
	 */
/*	ACTION transferidex( const name& sender,
							const name& receiver,
							const name& ride_type,
							uint64_t ride_qty);
*/
	/**
	 * @brief - on finishing a ride with __"crypto"__ pay_mode, the ride quota gets added
	 * @details - the ride quota gets added on finishing a ride from toeridetaxi contract for __"crypto"__ pay_mode
	 * 			- only accessed by 'ridetaxi_contract_ac'
	 * 
	 * @param ride_type - driver/commuter
	 * @param ride_qty - increase ride_quota by ride_qty
	 */
	ACTION addridequota(const name& ride_type, 
						uint64_t ride_qty);

	/**
	 * @brief - withdraw some money
	 * @details - A user wants to withdraw the quantity from wallet [optional]
	 * 
	 * @param user - user account
	 */
	ACTION withdraw( const name& user, 
					const asset& quantity);

	/**
	 * @brief - withdraw entire money
	 * @details - A user wants to withdraw the entire money from wallet [optional]
	 * 
	 * @param user - user account
	 */
	ACTION withdrawfull( const name& user );

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

	// --------------------------------------------------------------------------------
	static void check_buyer_seller( const name& user, const name& user_type, const name& ride_type) {
		check((user_type == "driver"_n) || 
			(user_type == "commuter"_n) || 
			(user_type == "validator"_n)
			, "Other types are not allowed for this action");


		// check the buyer/seller is enlisted in the `toeuserauth` contract table
		user_index user_table("toe1userauth"_n, user_type.value);
		auto user_it = user_table.find(user.value);

		check(user_it != user_table.end(), "Sorry! The user is not registered with us.");
		check(user_it->user_status == "verified"_n, "Sorry! The user is not yet verified.");

		// check for validator can buy/sell only "commuter" type ride
		if(user_type == "validator"_n) {
			check(ride_type == "commuter"_n, "Sorry! A validator can't buy \'driver\' type rides.");
		} 
		// check for commuter can buy/sell only "commuter" type ride
		else if(user_type == "commuter"_n) {
			check(ride_type == "commuter"_n, "Sorry! A commuter can't buy \'driver\' type rides.");
		} 
		// check for driver can buy/sell only "driver" type ride
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

	using consumeride_action  = action_wrapper<"consumeride"_n, &toeridex::consumeride>;
	using restoreride_action  = action_wrapper<"restoreride"_n, &toeridex::restoreride>;
	using addridequota_action  = action_wrapper<"addridequota"_n, &toeridex::addridequota>;

	// ACTION delruwallet(const name& user)
	// {
	// 	rexusrwallet_index rexusrwallet_table(get_self(), user.value);
	// 	auto rexusrwallet_it = rexusrwallet_table.find(ride_token_symbol.raw());	// finding the ("TOE", 4)

	// 	check(rexusrwallet_it != rexusrwallet_table.end(), " no balance.");
	// 	rexusrwallet_table.erase(rexusrwallet_it);	
	// }

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
		checksum256 national_id_hash;	// hash of (Country's ID proof) e.g. Aadhaar Card no.'s hash
		checksum256 profile_hash;		// hash of (full name, address)
		name user_status;				// added/updated/verified/blacklisted
		uint32_t add_timestamp;			// timestamp at which the user details is added
		uint32_t update_timestamp;		// timestamp at which the user details is updated
		uint32_t verify_timestamp;		// timestamp at which the user details is verified
		uint32_t blist_timestamp;		// timestamp at which the user is blacklisted
		name validator_verify;			// validator who verifies the user
		name validator_blacklist;		// validator who blacklist the user
		uint64_t ride_total;
		uint64_t ride_rated;
		float rating_avg;

		auto primary_key() const { return user.value; }
		uint64_t get_secondary_1() const { return user_status.value; }
		checksum256 get_secondary_2() const { return national_id_hash; }
	};

	using user_index = multi_index<"users"_n, user,
						indexed_by<"bystatus"_n, const_mem_fun<user, uint64_t, &user::get_secondary_1>>,
						indexed_by<"bynationalid"_n, const_mem_fun<user, checksum256, &user::get_secondary_2>>
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