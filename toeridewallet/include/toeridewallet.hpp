// #pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
// #include <eosio/print.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
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
using eosio::current_time_point;
using eosio::action;
using eosio::same_payer;
using eosio::symbol;
using eosio::require_recipient;
using eosio::checksum256;

using std::string;


CONTRACT toeridewallet : public contract
{
private:
	const symbol ride_token_symbol;
	const name ride_contract_ac;

public:
	using contract::contract;

	toeridewallet(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4),
				ride_contract_ac("toe1ridetaxi"_n) {}


	/**
	 * @brief - send fare to the contract
	 * @details 
	 * 		- send the `fare_est` to the contract before ride for automatic deduction.
	 * 		- this table `faretaxi` acts as a wallet for commuter
	 * @param commuter_ac - commuter account
	 * @param contract_ac - contract account
	 * @param quantity - the fare_est amount
	 * @param memo - remarks
	 */
	[[eosio::on_notify("toe1111token::transfer")]]
	void sendfare( const name& commuter_ac, 
					const name& contract_ac, 
					const asset& quantity, 
					const string& memo );


	/**
	 * @brief - withdraw some money
	 * @details - A commuter wants to withdraw the quantity from wallet [optional]
	 * 
	 * @param commuter_ac - commuter account
	 */
	ACTION withdraw( const name& commuter_ac,
						const asset& quantity );

	/**
	 * @brief - withdraw entire money
	 * @details - A commuter wants to withdraw the entire money from wallet [optional]
	 * 
	 * @param commuter_ac - commuter account
	 */
	ACTION withdrawfull( const name& commuter_ac );

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
	// `ridewallet` table is for keeping the record of fare_est amount transferred by a commuter before the ride starts.
	TABLE ridewallet
	{
		asset balance;

		auto primary_key() const { return balance.amount; }
	};

	using ridewallet_index = multi_index<"ridewallet"_n, ridewallet>;

	// -----------------------------------------------------------------------------------------------------------------------
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

	// -----------------------------------------------------------------------------------------------------------------------
	// Adding inline action for `sendreceipt` action in the same contract	
	void send_receipt(const name& user, const string& message);

	// Adding inline action for `addpaymost` action in the "toeridewallet" contract
	void addpay_mode_status(const name& commuter_ac );
};


