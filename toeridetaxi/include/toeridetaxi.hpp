#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
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
using eosio::action_wrapper;
using eosio::checksum256;
using eosio::sha256;
using eosio::transaction_size;
using eosio::read_transaction;


using std::string;
using std::vector;
using std::pair;
using std::make_pair;

CONTRACT toeridetaxi : public contract
{

private:
	const symbol ride_token_symbol;
	const name wallet_contract_ac;
	const name token_contract_ac;
	const name ridex_contract_ac;
	const name auth_contract_ac;

public:
	using contract::contract;

	toeridetaxi(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4),
				wallet_contract_ac("toe14rwallet"_n),
				token_contract_ac("toe1111token"_n),
				ridex_contract_ac("toe1111ridex"_n),
				auth_contract_ac("toe1userauth"_n) {}
	

	/**
	 * @brief - change source location
	 * @details 
	 *      - search by commuter_ac,
	 *      - create only once
	 *      - for modification use `changedes` action
	 * 
	 * @param commuter_ac - commuter eosio account name
	 * @param src_lat_hash - source latitude hash
	 * @param src_lon_hash - source longitude hash
	 * @param des_lat_hash - destination latitude hash
	 * @param des_lon_hash - destination longitude hash
	 * @param vehicle_type - it has to be among the defined list of taxi names - toex, toexl, toepool, toesuv, toeblack, toeselect, toeexprpool
	 * @param pay_mode - either crypto/fiatdigi/fiatcash
	 * @param ridex_usagestatus_com - 'y' or 'n' 
	 * @param fare_est - estimated fare (in fiat curr) to be calculated from calling API before calling the action
	 * @param fare_crypto_est - estimated fare (in crypto curr) to be calculated from calling API before calling the action
	 * @param finish_timestamp_est - estimated finish timestamp to be calculated from calling API before calling the action
	 * @param seat_count - to be defined for Pool ride [Optional]. Otherwise, it's value is 2 for other rides
	 * @param memo - the memo string to create a ride
	 */
	ACTION create( const name& commuter_ac,
					checksum256 src_lat_hash, 
					checksum256 src_lon_hash, 
					checksum256 des_lat_hash, 
					checksum256 des_lon_hash,
					const name& vehicle_type,
					const name& pay_mode,
					const name& ridex_usage_status,
					float fare_est,
					float market_price,
					const asset& fare_crypto_est,
					uint32_t finish_timestamp_est,
					uint32_t seat_count,     // define only for Pool rides. passed as default [Optional] parameter
					const string& memo
				);

	/**
	 * @brief - only valid for non-crypto pay_mode. set pay_mode & pay_status
	 * @details - set pay_status for 'fiatdigi' & 'fiatcash' pay_mode, only after ensuring sufficient balance in wallet in cases:
	 * 				- before creating a ride request
	 * 				- before changing destination
	 * 
	 * @param commuter_ac - commuter a/c
	 * @param pay_mode - pay_mode - "fiatdigi" or "fiatcash"
	 * @param memo - the memo string to set fiat payment status e.g. paytm, etc..
	 * 
	 */
	ACTION setfipaymost( const name& commuter_ac,
							const name& pay_mode,
							const string& memo );

	/**
	 * @brief - ride assigned
	 * @details - ride assigned automatically using Ride sharing algorithm
	 * 
	 * @param driver_ac - driver eosio account name
	 * @param ride_id - ride_id
	 * @param reachsrc_timestamp_est - estimated time to reach the pick-up point
	 * 
	 * @pre driver must be a verified user
	 * @pre driver_status must be online from `dridestatus` table
	 * 
	 */
	ACTION assign( const name& driver_ac, 
					// const name& commuter_ac,
					const checksum256& ride_id,
					uint32_t reachsrc_timestamp_est);

	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by commuter
	 * 
	 * @param commuter_ac - commmuter account
	 * @param memo - reason to cancel a ride
	 */
	ACTION cancelbycom( const name& commuter_ac,
						const string& memo );

	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by driver
	 * 
	 * @param driver_ac - commmuter account
	 * @param ride_id - ride_id
	 * @param memo - reason to cancel a ride
	 */
	ACTION cancelbydri( const name& driver_ac,
						const checksum256& ride_id,
						const string& memo );

	/**
	 * @brief - change source location
	 * @details 
	 *      - search by commuter_ac,
	 *      - change source's latitude & longitude
	 * 
	 * @param commuter_ac
	 * @param src_lat 
	 * @param src_lon
	 */
	// ACTION changesrc(
	//  const name& commuter_ac,
	//  double src_lat, 
	//  double src_lon 
	//  );

	/**
	 * @brief change destination location
	 * @details 
	 *      - search by commuter_ac,
	 *      - change destination's latitude & longitude
	 * 
	 * @param commuter_ac - commuter account
	 * @param des_lat - destination latitude
	 * @param des_lon - destination longitude
	 * @param ridex_usagestatus_com - 'y' or 'n' 
	 * @param fare_est - estimated fare (in fiat curr)
	 * @param fare_crypto_est - estimated fare (in crypto curr)
	 * @param memo - the memo string to change destination
	 */
	ACTION changedes( const name& commuter_ac,
						checksum256 des_lat_hash, 
						checksum256 des_lon_hash,
						const name& ridex_usagestatus_com,
						float fare_est,
						const asset& fare_crypto_est,
						const name& pay_mode,
						const string& memo );

	/**
	 * @brief - action to timestamp pickup point
	 * @details - action to timestamp pickup point
	 * 
	 * @param driver_ac - driver account
	 * @param ride_id - ride_id
	*/	 
	ACTION reachsrc( const name& driver_ac,
						const checksum256& ride_id);

	/**
	 * @brief start ride
	 * @details
	 *      - search table by `commuter_ac` 
	 *      - change ride_status (enroute --> ontrip)
	 * 
	 * @param driver_ac - driver account
	 * @param ride_id - ride_id
	 * @param ridex_usagestatus_dri - 'y' or 'n'
	 */
	ACTION start( const name& driver_ac, 
					const checksum256& ride_id,
					const name& ridex_usagestatus_dri );

	/**
	 * @brief - finish ride
	 * @details
	 *      - search table by `driver_ac` 
	 *      - change ride_status (ontrip --> completed)
	 * 
	 * @param driver_ac - driver account
	 * @param ride_id - ride_id
	 */
	ACTION finish( const name& driver_ac,
					const checksum256& ride_id );

	/**
	 * @brief - Add actual fare
	 * @details - Add actual fare after the ride is completed
	 * 
	 * @param driver_ac - driver account
	 * @param ride_id - ride_id
	 * @param fare_act - actual fare (in fiat curr)
	 * @param fare_crypto_act - actual fare (in crypto curr)
	 */
	ACTION addfareact(const name& driver_ac, 
						const checksum256& ride_id,
						float fare_act,
						const asset& fare_crypto_act);


	/**
	 * @brief - valid for 'crypto' pay_mode
	 * @details - a driver receives fare after ride is completed, only when the pay_mode chosen as `crypto`
	 * 
	 * @param driver_ac - driver account
	 * @param ride_id - ride_id
	 * @memo - note for receiving fare to driver
	 */
	ACTION recvfare( const name& driver_ac,
					const checksum256& ride_id,
					const string& memo);

	/**
	 * @brief - driver add rating to commuter
	 * @details - driver add rating to commuter corresponding to ride_id
	 * 
	 * @param driver_ac - driver
	 * @param ride_id - ride_id
	 * @param rating_com - rating of commuter
	 * 
	 * @pre - check the `ride_id->driver_ac == driver_ac`
	 * @pre - check the `ride_id->ride_status == "complete"`
	 * @pre - check the `ride_id->crypto_paystatus == "paytodri"`, if the `ride_it->pay_mode =="crypto"`
	 * @pre - check the `ride_id->rating_status_dri != "done"` i.e. not done previously
	 * 
	 */
	ACTION driaddrating( const name& driver_ac,
							const checksum256& ride_id,
							float rating_com );

	/**
	 * @brief - driver add rating to commuter
	 * @details - driver add rating to commuter corresponding to ride_id
	 * 
	 * @param driver_ac - driver
	 * @param ride_id - ride_id
	 * @param rating_com - rating of commuter
	 * 
	 * @pre - check the `ride_id->driver_ac == driver_ac`
	 * @pre - check the `ride_id->ride_status == "complete"`
	 * @pre - check the `ride_id->crypto_paystatus == "paytodri"`, if the `ride_it->pay_mode =="crypto"`
	 * @pre - check the `ride_id->rating_status_com != "done"` i.e. not done previously
	 * 
	 */
	ACTION comaddrating( const name& commuter_ac,
							const checksum256& ride_id,
							float rating_dri );

	/**
	 * @brief - driver add status - online/offline
	 * @details - driver add status - online/offline
	 * 
	 * @param driver_ac - driver's account
	 * @param status - online/offline
	 * 
	 * @pre driver must be a verified user
	 */
	ACTION addristatus( const name& driver_ac, const name& status );


	/**
	 * @brief - set ridetaxi specs wait timestamp for erase action
	 * @details - set ridetaxi specs wait timestamp for erase action
	 * 
	 * @param action - action
	 * @param wait_time - wait timestamp
	 * 
	 */
	ACTION setrtststamp( const name& action, 
						uint32_t wait_time );

	/**
	 * @brief - set fuel price of petrol & diesel
	 * @details - set fuel price of petrol & diesel in fiat currency
	 * 
	 * @param currency - currency e.g. "inr"
	 * @param fuel_price_petrol - fuel price of petrol
	 * @param fuel_price_diesel - fuel price of diesel
	 */
	ACTION setrtsfuelpr( const name& currency, 
						const name& fuel_unit, 
						float fuel_price_petrol,
						float fuel_price_diesel );
	/**
	 * @brief - An external methods to erase ride
	 * @details - erase after the ride is finished & payment is done. 
	 * 
	 * @param commuter_ac erasing by searching commuter_ac
	 * @param memo - the memo string to erase a ride
	 */
	ACTION erase( const name& commuter_ac,
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


	// --------------------------------------------------------------------------------------------------------------------
	static void check_userauth( const name& user, const name& type) {
		if(type == "driver"_n) {
			// check whether the `user` is a verified driver by reading the `auth` table
			user_index user_table("toe1userauth"_n, "driver"_n.value);
			auto user_it = user_table.find(user.value);

			check( user_it != user_table.end(), "The driver is not added in the Auth Table.");
			check( user_it->user_status == "verified"_n, "The driver is not verified yet.");
		} else if (type == "commuter"_n) {
			// check whether the `user` is a verified commuter by reading the `auth` table
			user_index user_table("toe1userauth"_n, "commuter"_n.value);
			auto user_it = user_table.find(user.value);

			check( user_it != user_table.end(), "The commuter is not added in the Auth Table.");
			check( user_it->user_status == "verified"_n, "The commuter is not verified yet.");
		}
	}

	// --------------------------------------------------------------------------------------------------------------------
	static void check_fareamount( const asset& quantity ) {
		check(quantity.is_valid(), "invalid quantity");
		check(quantity.amount > 0, "must withdraw positive quantity");
		check(quantity.symbol == symbol("TOE", 4), "symbol precision mismatch");
	}

	// --------------------------------------------------------------------------------------------------------------------
	// inline void read_vector_pair( const vector<pair<name, checksum256>>& v, const name& s ) {
	// 	auto s_it = std::find_if(v.begin(), v.end(), [&](auto& vs){ return vs.first == s; });

	// 	if(s_it != v.end()) {			// key found
	// 		print("The value(s): ");

	// 		while(s_it != v.end()) {
	// 			print(s_it->second, " | ");
	// 			++s_it;
	// 		}
	// 	} else {						// key NOT found
	// 		print("No item found with this key");
	// 	}
	// }


private:
// ========TABLES========================================================================================================
	TABLE ridetaxi
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
	TABLE dridestatus
	{
		name user_type;		// driver
		name status;	// online or offline or assigned

		auto primary_key() const { return user_type.value; };
	};

	using dridestatus_index = multi_index<"dridestatus"_n, dridestatus>;

// -----------------------------------------------------------------------------------------------------------------------
	// RideTaxiSpecs timestamp 
	TABLE rtststamp
	{
		name action;	// "erase"
		uint32_t wait_time;	// wait time (in secs) to erase the ride, when no rating done by commuter.

		auto primary_key() const { return action.value; };
	};

	using rtststamp_index = multi_index<"rtststamp"_n, rtststamp>;

// -----------------------------------------------------------------------------------------------------------------------
	// RideTaxiSpecs fuel price
	TABLE rtsfuelprice
	{
		name fiat_currency;				// e.g. "INR"
		name fuel_unit;				// e.g. litre, gallon
		float fuel_price_petrol;	// fuel_price of petrol
		float fuel_price_diesel;	// fuel_price of diesel

		auto primary_key() const { return fiat_currency.value; };
	};

	using rtsfuelprice_index = multi_index<"rtsfuelprice"_n, rtsfuelprice>;

// -----------------------------------------------------------------------------------------------------------------------
	struct ridewallet
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
// ========Functions========================================================================================================
	// Adding inline action for `sendalert` action in the same contract 
	void send_alert(const name& user, const string& message);

	// Adding inline action for `sendreceipt` action in the same contract   
	void send_receipt(const name& user, const string& message);

	// get the current timestamp
	inline uint32_t now() const {
		return current_time_point().sec_since_epoch();
	}

	template<typename CharT>
	static std::string to_hex(const CharT* d, uint32_t s) {
	  std::string r;
	  const char* to_hex="0123456789abcdef";
	  uint8_t* c = (uint8_t*)d;
	  for( uint32_t i = 0; i < s; ++i ) {
	    (r += to_hex[(c[i] >> 4)]) += to_hex[(c[i] & 0x0f)];
	  }
	  return r;
	}

	// get the sha256 hash digest/checksum
	inline checksum256 hash_digest_256(const name& commuter_ac,
										uint32_t create_timestamp) const {
		string data_str_cpp = commuter_ac.to_string() + std::to_string(create_timestamp);
		const char * data_str_c = data_str_cpp.c_str(); 

		auto hash_digest = sha256(data_str_c, strlen(data_str_c));

		return hash_digest;
	}

	// concatenation of ids
	// static uint128_t combine_ids(const uint64_t &x, const uint64_t &y) {
	//     return (uint128_t{x} << 64) | y;
	// }

	// calculate the current_rating_avg
	inline float current_rating_avg( float last_rating_avg,
										float current_rating,
										uint64_t last_ride_rated ) const {
		return ( (last_rating_avg * last_ride_rated) + current_rating ) / (last_ride_rated + 1);
	} 

	// get the transaction id
	inline checksum256 get_trxid()
	{
	  auto trxsize = transaction_size();
	  char trxbuf[trxsize];
	  uint32_t trxread = read_transaction( trxbuf, trxsize );
	  check( trxsize == trxread, "read_transaction failed");
	  return sha256(trxbuf, trxsize);
	}

	// Adding inline action for `disburse` action in the ridewallet contract   
	void disburse_fare(const name& receiver_ac,
						const name& wallet_holder,
						const asset& quantity,
						const string& memo );

	// Adding inline action for `consumeride` action in the ridex contract   
	void consume_ride( const name& user,
						const name& user_type,
						const name& ride_type,
						uint64_t ride_qty );

	// Adding inline action for `restoreride` action in the ridex contract   
	void restore_ride( const name& user,
						const name& user_type,
						const name& ride_type,
						uint64_t ride_qty );

	// Adding inline action for `addridequota` action in the ridex contract   
	void add_ridequota(const name& type, 
						uint64_t ride_qty );

	// Adding inline action for `setridetotal` action in the user auth contract   
	void set_ride_total(const name& user,
						const name& user_type,
						uint64_t ride_total);

	// Adding inline action for `setriderated` action in the user auth contract   
	void set_ride_rated(const name& user,
						const name& user_type,
						uint64_t ride_rated);

	// Adding inline action for `setratingavg` action in the user auth contract   
	void set_rating_avg(const name& user,
						const name& user_type,
						float rating_avg);


};
