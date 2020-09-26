// #pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
// #include <eosio/print.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <algorithm>
#include <vector>
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
using eosio::action_wrapper;

using std::string;
using std::vector;
using std::pair;

CONTRACT toeridewallet : public contract
{
private:
	const symbol ride_token_symbol;
	const name token_contract_ac;
	const name auth_contract_ac;
	const name ridetaxi_contract_ac;
	const name rides_fees_ac;
	const float ride_commission_percent;


public:
	using contract::contract;

	toeridewallet(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4),
				token_contract_ac("toe1111token"_n),
				auth_contract_ac("toe1userauth"_n),
				ridetaxi_contract_ac("toe1ridetaxi"_n),
				rides_fees_ac("toeridesfees"_n),
				ride_commission_percent(0.25) {}


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
	 * @brief - only accessed by ride contract ac(s) - taxi,..
	 * @details - "toe1ridetaxi"  disburse from "toe14rwallet" contract to a person (driver or any)
	 * 
	 * @param receiver_ac - account to whom money is disbursed
	 * @param wallet_holder - account who has the wallet, from where the money is to be deducted.
	 * @param quantity - disburse amount
	 * @param memo - purpose of disbursing money
	 */
	ACTION disburse( const name& receiver_ac,
					const name& wallet_holder,
					const asset& quantity,
					const string& memo );

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



	using disburse_action  = action_wrapper<"disburse"_n, &toeridewallet::disburse>;

	static void check_quantity( const asset& quantity ) {
		check(quantity.is_valid(), "invalid quantity");
		check(quantity.amount > 0, "must withdraw positive quantity");
		check(quantity.symbol == symbol("TOE", 4), "symbol precision mismatch");
	}

private:
	// -----------------------------------------------------------------------------------------------------------------------
	// `ridewallet` table is for keeping the record of fare_est amount transferred by a commuter before the ride starts.
	// scope - user (commuter) name
	TABLE ridewallet
	{
		asset balance;

		auto primary_key() const { return balance.symbol.raw(); }
	};

	using ridewallet_index = multi_index<"ridewallet"_n, ridewallet>;

	// -----------------------------------------------------------------------------------------------------------------------
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

	// -----------------------------------------------------------------------------------------------------------------------
	struct ridetaxi
	{
		name commuter_ac;
		name ride_status;           // /requested/enroute/waiting/ontrip/complete
		name driver_ac;
		checksum256 ride_id;		// a unique id of the ride. Used for rating in `userauth` table in toeuserauth contract
		checksum256 src_lat_hash; 
		checksum256 src_lon_hash; 
		checksum256 des_lat_hash; 
		checksum256 des_lon_hash;
		name vehicle_type;      // list of taxis - toeauto, toemoto, toego, toegoexec, toepremier, toepremexec, toexl, toegointcity, toexlintcity
		uint32_t seat_count;        // set for pool, else default is 2
		name pay_mode;            // crypto or fiatdigi or fiatcash
		name crypto_paystatus;          // paidbycom or paidtodri for "crypto"
		name fiat_paystatus;          // paidbycom or paidtodri	for "fiatdigi"
		vector<pair<name, checksum256>> action_txnid_vector;		// vector of pairs (action_name, txn_id)
		uint32_t create_timestamp;			// at which ride is created
		uint32_t assign_timestamp;  		// at which ride is assigned
		uint32_t reachsrc_timestamp_est;    // at which driver is estimated to reach source location to pick-up
		uint32_t reachsrc_timestamp_act;    // at which driver actually reached source location to pick-up
		uint32_t cancel_timestamp;
		uint32_t start_timestamp;       // at which the ride is started
		uint32_t changedes_timestamp;		// at which commuter changes destination(s). It can be multiple. So, if > 1, then shown timestamp will be the last changedes.
		uint32_t finish_timestamp_act;      // at which the ride is finished
		uint32_t finish_timestamp_est;      // at which the ride is estimated to finish
		uint32_t addfareact_timestamp;		// at which the fare_act is added
		name ridex_usagestatus_com;					// "y"_n or "n"_n, only 1 ride is used by default.
		name ridex_usagestatus_dri;					// "y"_n or "n"_n, only 1 ride is used by default.
		float fare_est;			// estimated fare (in national curr)
		float fare_act;			// actual fare (in national curr)
		float market_price;		// market price during ride request
		asset fare_crypto_est;			// estimated fare (in national curr) converted (outside blockchain interaction) to fare (in crypto) based on the market rate.
		asset fare_crypto_act;			// actual fare (in national curr) converted (outside blockchain interaction) to fare (in crypto) based on the market rate.
		name rating_status_dri;			// rating status of doer - driver | "done"
		float rating_dri;				// rating of driver set by commuter | < 5.0
		name rating_status_com;			// rating status of doer - commuter | "done"
		float rating_com;				// rating of commuter set by driver | < 5.0

		auto primary_key() const { return commuter_ac.value; }
		uint64_t get_secondary_1() const { return driver_ac.value; }
		checksum256 get_secondary_2() const { return ride_id; }
		uint64_t get_secondary_3() const { return ride_status.value; }
		uint64_t get_secondary_4() const { return vehicle_type.value; }
		// uint128_t get_secondary_5_driver_ridestatus() const { return combine_ids(driver_ac.value, ride_status.value); }

	};

	using ridetaxi_index = multi_index<"ridestaxi"_n, ridetaxi, 
									indexed_by<"bydriver"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_1>>,
									indexed_by<"byrideid"_n, const_mem_fun<ridetaxi, checksum256, &ridetaxi::get_secondary_2>>,
									indexed_by<"byridestatus"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_3>>,
									indexed_by<"byvehicltype"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_4>>
									// indexed_by<"bydrirstatus"_n, const_mem_fun<ridetaxi, uint128_t, &ridetaxi::get_secondary_5_driver_ridestatus>>
									>;

	// -----------------------------------------------------------------------------------------------------------------------
	// Adding inline action for `sendalert` action in the same contract 
	void send_alert(const name& user, const string& message);

	// Adding inline action for `sendreceipt` action in the same contract	
	void send_receipt(const name& user, const string& message);

};


