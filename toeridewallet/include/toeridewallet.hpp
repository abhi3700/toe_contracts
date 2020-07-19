#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
// #include <eosio/print.hpp>
#include <eosio/system.hpp>
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

using std::string;


CONTRACT toeridewallet : public contract
{
private:
	const symbol ride_token_symbol;

public:
	using contract::contract;

	toeridewallet(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4) {}


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
	// table defined for reading data
	struct ridetaxi
	{
		name commuter_ac;
		name ride_status;			// enroute/waiting/ontrip/complete
		name driver_ac;
		double src_lat; 
		double src_lon; 
		double des_lat; 
		double des_lon;
		name vehicle_type;		// list of taxis - toex, toexl, toepool, toesuv, toeblack, toeselect, toeexprpool
		uint32_t seat_count;		// set for pool, else default is 2
		string pay_mode;			// crypto or fiatdigi or fiatcash
		string pay_status;			// paidbycom or paidbydri
		uint32_t assign_timestamp;	// at which ride is assigned
		uint32_t reachsrc_timestamp;	// at which driver reached source location to pick-up
		uint32_t start_timestamp;		// at which the ride is started
		uint32_t finish_timestamp_act;		// at which the ride is finished
		uint32_t finish_timestamp_est;		// at which the ride is estimated to finish
		double fare_est;			// estimated fare
		double fare_act;			// actual fare



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
	// Adding inline action for `sendreceipt` action in the same contract	
	void send_receipt(const name& user, const string& message);

	// Adding inline action for `addpay` action in the "toeridewallet" contract
	void addpay_mode_status(const name& commuter_ac );
};

