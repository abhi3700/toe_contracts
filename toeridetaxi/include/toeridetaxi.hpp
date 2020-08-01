#pragma once
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
using eosio::action_wrapper;
using eosio::checksum256;

using std::string;

CONTRACT toeridetaxi : public contract
{

private:
	const symbol ride_token_symbol;

public:
	using contract::contract;

	toeridetaxi(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4) {}
	
	/**
	 * @brief - Add pay mode & status
	 * @details - Add pay mode ("crypto") & status ("paidbycom") into the `ridetaxi` table
	 * 
	 * @param commuter_ac - commuter, transfers money to the contract's token balance & update the amount value in `ridewallet` table & 
	 * 								updates the transferred status into the `ridetaxi` table
	 */
	ACTION addpaymost( const name& commuter_ac );

	/**
	 * @brief - change source location
	 * @details 
	 *      - search by commuter_ac,
	 *      - create only once
	 *      - for modification use `changedes` action
	 * 
	 * @param commuter_ac - commuter eosio account name
	 * @param src_lat - source latitude
	 * @param src_lon - source longitude
	 * @param des_lat - destination latitude
	 * @param des_lon - destination longitude
	 * @param vehicle_type - it has to be among the defined list of taxi names - toex, toexl, toepool, toesuv, toeblack, toeselect, toeexprpool
	 * @param seat_count - to be defined for Pool ride [Optional]. Otherwise, it's value is 2 for other rides
	 * @param pay_mode - either crypto/fiatdigi/fiatcash
	 * @param fare_est - estimated fare (in fiat curr) to be calculated from calling API before calling the action
	 * @param fare_crypto_est - estimated fare (in crypto curr) to be calculated from calling API before calling the action
	 * @param finish_timestamp_est - estimated finish timestamp to be calculated from calling API before calling the action
	 */
	ACTION create( const name& commuter_ac,
					double src_lat, 
					double src_lon, 
					double des_lat, 
					double des_lon,
					const name& vehicle_type,
					const string& pay_mode,
					float fare_est,
					asset fare_crypto_est,
					uint32_t finish_timestamp_est,
					uint32_t seat_count     // define only for Pool rides. passed as default [Optional] parameter
				);


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
	 */
	ACTION assign( const name& driver_ac, 
					const name& commuter_ac,
					uint32_t reachsrc_timestamp_est);

	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by commuter
	 * 
	 * @param commuter_ac - commmuter account
	 */
	ACTION cancelbycom( const name& commuter_ac );

	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by driver
	 * 
	 * @param driver_ac - commmuter account
	 */
	ACTION cancelbydri( const name& driver_ac );

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
	 */
	ACTION changedes( const name& commuter_ac,
						double des_lat, 
						double des_lon,
						float fare_est,
						asset fare_crypto_est,
						const string& pay_mode );

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
						asset fare_crypto_act);

	/**
	 * @brief - send fare to the contract
	 * @details 
	 *      - send the `fare_est` to the contract before ride for automatic deduction.
	 *      - this table `faretaxi` acts as a wallet for commuter
	 * @param commuter_ac - commuter account
	 * @param contract_ac - contract account
	 * @param quantity - the fare_est amount
	 * @param memo - remarks
	 */
/*  [[eosio::on_notify("toe1111token::transfer")]]
	void sendfare( const name& commuter_ac, 
					const name& contract_ac, 
					const asset& quantity, 
					const string& memo );
*/
	/**
	 * @brief - a driver receives fare
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
	 */
	ACTION eraseride( const name& commuter_ac);


	// Action wrappers
	using sendalert_action = action_wrapper<"sendalert"_n, &toeridetaxi::sendalert>;
	using sendreceipt_action = action_wrapper<"sendreceipt"_n, &toeridetaxi::sendreceipt>;
	using addpaymost_action = action_wrapper<"addpaymost"_n, &toeridetaxi::addpaymost>;

private:
// ========TABLES========================================================================================================
	TABLE ridetaxi
	{
		name commuter_ac;
		name ride_status;           // enroute/waiting/ontrip/complete
		name driver_ac;
		double src_lat; 
		double src_lon; 
		double des_lat; 
		double des_lon;
		name vehicle_type;      // list of taxis - toex, toexl, toepool, toesuv, toeblack, toeselect, toeexprpool
		uint32_t seat_count;        // set for pool, else default is 2
		string pay_mode;            // crypto or fiatdigi or fiatcash
		string pay_status;          // paidbycom or paidtodri
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
