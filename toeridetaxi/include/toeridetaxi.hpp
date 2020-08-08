#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
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
using eosio::action_wrapper;
using eosio::checksum256;

using std::string;

CONTRACT toeridetaxi : public contract
{

private:
	const symbol ride_token_symbol;
	const name wallet_contract_ac;
	const name token_contract_ac;

public:
	using contract::contract;

	toeridetaxi(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4),
				wallet_contract_ac("toe14rwallet"_n),
				token_contract_ac("toe1111token"_n) {}
	

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
					float fare_est,
					const asset& fare_crypto_est,
					uint32_t finish_timestamp_est,
					uint32_t seat_count,     // define only for Pool rides. passed as default [Optional] parameter
					const string& memo
				);

	/**
	 * @brief - only valid for non-crypto pay_mode 
	 * @details - set pay_status for 'fiatdigi' & 'fiatcash' pay_mode
	 * 
	 * @param commuter_ac - commuter a/c
	 * @param fiat_paystatus - fiat pay_status
	 * @param memo - the memo string to set fiat payment status e.g. paytm, etc..
	 * 
	 */
	ACTION setfiatpayst( const name& commuter_ac,
							const name& fiat_paystatus,
							const string& memo );

	/**
	 * @brief - ride assigned
	 * @details - ride assigned automatically using Ride sharing algorithm
	 * 
	 * @param driver_ac - driver eosio account name
	 * @param commuter_ac - commuter eosio account name
	 * @param reachsrc_timestamp_est - estimated time to reach the pick-up point
	 * 
	 * @pre driver must be a verified user
	 * @pre driver_status must be online from `dridestatus` table
	 * 
	 */
	ACTION assign( const name& driver_ac, 
					const name& commuter_ac,
					uint32_t reachsrc_timestamp_est);

	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by commuter
	 * 
	 * @param commuter_ac - commmuter account
	 * @param memo - the memo string to cancel a ride
	 */
	ACTION cancelbycom( const name& commuter_ac,
						const string& memo );

	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by driver
	 * 
	 * @param driver_ac - commmuter account
	 * @param memo - the memo string to cancel a ride
	 */
	ACTION cancelbydri( const name& driver_ac,
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
	 * @param fare_est - estimated fare (in fiat curr)
	 * @param fare_crypto_est - estimated fare (in crypto curr)
	 * @param memo - the memo string to change destination
	 */
	ACTION changedes( const name& commuter_ac,
						checksum256 des_lat_hash, 
						checksum256 des_lon_hash,
						float fare_est,
						const asset& fare_crypto_est,
						const name& pay_mode,
						const string& memo );

	/**
	 * @brief - action to timestamp pickup point
	 * @details - action to timestamp pickup point
	 * 
	 * @param driver_ac - driver account
	*/	 
	ACTION reachsrc( const name& driver_ac );

	/**
	 * @brief start ride
	 * @details
	 *      - search table by `commuter_ac` 
	 *      - change ride_status (enroute --> ontrip)
	 * 
	 * @param driver_ac - driver account
	 * @param commuter_ac - commuter account
	 */
	ACTION start( const name& driver_ac/*,
					const name& commuter_ac*/);

	/**
	 * @brief - finish ride
	 * @details
	 *      - search table by `driver_ac` 
	 *      - change ride_status (ontrip --> completed)
	 * 
	 * @param driver_ac - driver account
	 */
	ACTION finish( const name& driver_ac );

	/**
	 * @brief - Add actual fare
	 * @details - Add actual fare after the ride is completed
	 * 
	 * @param driver_ac - driver account
	 * @param fare_act - actual fare (in fiat curr)
	 * @param fare_crypto_act - actual fare (in crypto curr)
	 */
	ACTION addfareact(const name& driver_ac, 
						float fare_act,
						const asset& fare_crypto_act);


	/**
	 * @brief - valid for 'crypto' pay_mode
	 * @details - a driver receives fare after ride is completed, only when the pay_mode chosen as `crypto`
	 * 
	 * @param driver_ac - driver account
	 */
	ACTION recvfare( const name& driver_ac );

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

	/**
	 * @brief - An external methods to erase ride
	 * @details - erase after the ride is finished & payment is done. 
	 * 
	 * @param commuter_ac erasing by searching commuter_ac
	 * @param memo - the memo string to erase a ride
	 */
	ACTION eraseride( const name& commuter_ac,
						const string& memo);

	static void check_userauth( const name& user, const name& type) {
		// check whether the `user` is a verified driver by reading the `auth` table
		user_index user_table("toe1userauth"_n, user.value);
		auto user_it = user_table.find(user.value);

		if(type == "driver"_n) {
			check( user_it != user_table.end(), "The driver is not added in the Auth Table.");
			check( user_it->type == "driver"_n, "The given user is not a driver");
			check( user_it->user_status == "verified"_n, "The driver is not verified yet.");
		}
		if (type == "commuter"_n) {
			check( user_it != user_table.end(), "The commuter is not added in the Auth Table.");
			check( user_it->type == "commuter"_n, "The given user is not a commuter");
			check( user_it->user_status == "verified"_n, "The commuter is not verified yet.");
		}
	}

	static void check_dridestatus( const name& driver_ac ) {
		//instantiate the `dridestatus` table
		dridestatus_index dridestatus_table("toe1ridetaxi"_n, driver_ac.value);
		auto dridestatus_it = dridestatus_table.find("online"_n.value);

		// check the driver is online
		check(dridestatus_it != dridestatus_table.end(), "driver's status row is not present in the table.");
		check(dridestatus_it->status == "online"_n, "driver is not online.");
	}

private:
// ========TABLES========================================================================================================
	TABLE ridetaxi
	{
		name commuter_ac;
		name ride_status;           // enroute/waiting/ontrip/complete
		name driver_ac;
		checksum256 src_lat_hash; 
		checksum256 src_lon_hash; 
		checksum256 des_lat_hash; 
		checksum256 des_lon_hash;
		name vehicle_type;      // list of taxis - toeauto, toemoto, toego, toegoexec, toepremier, toepremexec, toexl, toegointcity, toexlintcity
		uint32_t seat_count;        // set for pool, else default is 2
		name pay_mode;            // crypto or fiatdigi or fiatcash
		name crypto_paystatus;          // paidbycom or paidtodri for "crypto"
		name fiat_paystatus;          // paidbycom or paidtodri	for "fiatdigi"
		uint32_t assign_timestamp;  // at which ride is assigned
		uint32_t reachsrc_timestamp_est;    // at which driver is estimated to reach source location to pick-up
		uint32_t reachsrc_timestamp_act;    // at which driver actually reached source location to pick-up
		uint32_t start_timestamp;       // at which the ride is started
		uint32_t finish_timestamp_act;      // at which the ride is finished
		uint32_t finish_timestamp_est;      // at which the ride is estimated to finish
		float fare_est;			// estimated fare (in national curr)
		float fare_act;			// actual fare (in national curr)
		asset fare_crypto_est;			// estimated fare (in national curr) converted (outside blockchain interaction) to fare (in crypto) based on the market rate.
		asset fare_crypto_act;			// actual fare (in national curr) converted (outside blockchain interaction) to fare (in crypto) based on the market rate.


		auto primary_key() const { return commuter_ac.value; }
		uint64_t get_secondary_1() const { return driver_ac.value; }
		uint64_t get_secondary_2() const { return ride_status.value; }
		uint64_t get_secondary_3() const { return vehicle_type.value; }
	};

	using ridetaxi_index = multi_index<"ridestaxi"_n, ridetaxi, 
									indexed_by<"bydriver"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_1>>,
									indexed_by<"byridestatus"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_2>>,
									indexed_by<"byvehicltype"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_3>>
									>;

// -----------------------------------------------------------------------------------------------------------------------
	TABLE dridestatus
	{
		name status;	// online or offline

		auto primary_key() const { return status.value; };
	};

	using dridestatus_index = multi_index<"dridestatus"_n, dridestatus>;

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
// ========Functions========================================================================================================
	// Adding inline action for `sendalert` action in the same contract 
	void send_alert(const name& user, const string& message);

	// Adding inline action for `sendreceipt` action in the same contract   
	void send_receipt(const name& user, const string& message);

	// get the current timestamp
	inline uint32_t now() const {
		return current_time_point().sec_since_epoch();
	}



};
