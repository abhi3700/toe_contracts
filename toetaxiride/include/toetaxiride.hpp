#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using eosio::contract;
using eosio::print;
using eosio::name;
using eosio::multi_index;
using eosio::indexed_by;
using eosio::asset;
using eosio::check;
using eosio::permission_level;

using std::string;

class [[eosio::contract]] toetaxiride : public contract
{

private:
	const symbol ride_token_symbol;

public:
	using contract::contract;

	toetaxiride(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds), 
				ride_token_symbol("TOE", 4) {}
	
	/**
	 * @brief - change source location
	 * @details 
	 * 		- search by commuter_ac,
	 * 		- change source's latitude & longitude
	 * 
	 * @param commuter_ac
	 * @param src_lat 
	 * @param src_lon
	 */
	[[eosio::action]]
	void create(
		const name& commuter_ac,
		double src_lat, 
		double src_lon, 
		double des_lat, 
		double des_lon,
		string vehicle_type 
		);


	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by commuter
	 * 
	 * @param commuter_ac - commmuter account
	 */
	[[eosio::action]]
	void cancelbycom(
		const name& commuter_ac
		);

	/**
	 * @brief - cancel ride
	 * @details - cancel ride (if any created) by driver
	 * 
	 * @param driver_ac - commmuter account
	 */
	[[eosio::action]]
	void cancelbydri(
		const name& driver_ac
		);

	/**
	 * @brief - change source location
	 * @details 
	 * 		- search by commuter_ac,
	 * 		- change source's latitude & longitude
	 * 
	 * @param commuter_ac
	 * @param src_lat 
	 * @param src_lon
	 */
	[[eosio::action]]
	void changesrc(
		const name& commuter_ac,
		double src_lat, 
		double src_lon 
		);

	/**
	 * @brief change destination location
	 * @details 
	 * 		- search by commuter_ac,
	 * 		- change destination's latitude & longitude
	 * 
	 * @param commuter_ac - commuter account
	 * @param des_lat - destination latitude
	 * @param des_lon - destination longitude
	 */
	[[eosio::action]]
	void changedes(
		const name& commuter_ac,
		double des_lat, 
		double des_lon 
		);

	[[eosio::action]]
	void alert() {

	}

	/**
	 * @brief start ride
	 * @details
	 * 		- search table by `commuter_ac` 
	 * 		- change ride_status (enroute --> ontrip)
	 * 
	 * @param driver_ac - driver account
	 * @param commuter_ac - commuter account
	 */
	[[eosio::action]]
	void start(
		const name& driver_ac,
		const name& commuter_ac
		);

	/**
	 * @brief - finish ride
	 * @details
	 * 		- search table by `driver_ac` 
	 * 		- change ride_status (ontrip --> completed)
	 * 
	 * @param driver_ac - driver account
	 */
	[[eosio::action]]
	void finish(
		const name& driver_ac,
		);


	/**
	 * @brief - send fare to the contract
	 * @details - send the `fare_est` to the contract before ride for automatic deduction.
	 * 
	 * @param commuter_ac - commuter account
	 * @param contract_ac - contract account
	 * @param quantity - the fare_est amount
	 * @param memo - remarks
	 */
	[[eosio::on_notify("toe1111token::transfer")]]
	void sendfare(
		name commuter_ac, 
		name contract_ac, 
		asset quantity, 
		string memo
		);




private:
	struct [[eosio::table]] ridetaxi
	{
		name commuter_ac;
		name ride_status;		// enroute, ontrip, finished
		name driver_ac;
		double src_lat; 
		double src_lon; 
		double des_lat; 
		double des_lon;
		string pay_mode;			// crypto or fiat (includes cash)
		string pay_status;			// preride or unpaid or postride
		string assign_timestamp;	// at which ride is assigned
		string pickup_point_timestamp;	// at which driver reached source location to pick-up
		string start_timestamp;		// at which the ride is started
		string finish_timestamp		// at which the ride is finished
		double fare_est;			// estimated fare
		double fare_act;			// actual fare



		auto primary_key() const { return commuter_ac.value; }
		auto get_secondary_1() const { return driver_ac.value; }
		auto get_secondary_2() const { return ride_status.value; }
	};

	using ridetaxi_index = multi_index<"rides"_n, ridetaxi, 
									indexed_by<"bydriver"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_1>>
									indexed_by<"byridestatus"_n, const_mem_fun<ridetaxi, uint64_t, &ridetaxi::get_secondary_2>>
									>;


// -----------------------------------------------------------------------------------------------------------------------
	struct [[eosio::table]] fareamount
	{
		asset balance;

		auto primary_key() const { return balance.amount; }
	};

	using fareamount_index = multi_index<"fare"_n, fareamount>;
};
