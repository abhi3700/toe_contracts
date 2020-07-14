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
using eosio::datastream;
using eosio::current_time_point;
using eosio::action;
using eosio::same_payer;

using std::string;

CONTRACT toetaxiride : public contract
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
	 * 		- create only once
	 * 		- for modification use `changedes` action
	 * 
	 * @param commuter_ac - commuter eosio account name
	 * @param src_lat - source latitude
	 * @param src_lon - source longitude
	 * @param des_lat - destination latitude
	 * @param des_lon - destination longitude
	 * @param vehicle_type - it has to be among the defined list
	 * @param seat_count - to be defined for Pool ride [Optional]. Otherwise, it's value is 2 for other rides
	 * @param pay_mode - either crypto/fiatdigi/fiatcash
	 * @param fare_est - estimated fare to be calculated from calling API before calling the action
	 * @param finish_timestamp_est - estimated finish timestamp to be calculated from calling API before calling the action
	 */
	ACTION create( const name& commuter_ac,
								double src_lat, 
								double src_lon, 
								double des_lat, 
								double des_lon,
								string vehicle_type,
								uint32_t seat_count,		// define only for Pool rides. passed as default [Optional] parameter
								string pay_mode,
								double fare_est,
								uint32_t finish_timestamp_est);



	/**
	 * @brief - ride assigned
	 * @details - ride assigned automatically using Ride sharing algorithm
	 * 
	 * @param driver_ac - driver eosio account name
	 * @param commuter_ac - commuter eosio account name
	 */
	ACTION assign( const name& driver_ac, 
					const name& commuter_ac );

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
	 * 		- search by commuter_ac,
	 * 		- change source's latitude & longitude
	 * 
	 * @param commuter_ac
	 * @param src_lat 
	 * @param src_lon
	 */
	// ACTION changesrc(
	// 	const name& commuter_ac,
	// 	double src_lat, 
	// 	double src_lon 
	// 	);

	/**
	 * @brief change destination location
	 * @details 
	 * 		- search by commuter_ac,
	 * 		- change destination's latitude & longitude
	 * 
	 * @param commuter_ac - commuter account
	 * @param des_lat - destination latitude
	 * @param des_lon - destination longitude
	 * @param fare_est - estimated fare
	 */
	ACTION changedes( const name& commuter_ac,
						double des_lat, 
						double des_lon,
						double fare_est,
						string pay_mode );

	/**
	 * @brief - action to timestamp pickup point
	 * @details - action to timestamp pickup point
	 * 
	 * @param driver_ac - driver account
	 */
	ACTION reachsrc( const name& driver_ac )

	/**
	 * @brief start ride
	 * @details
	 * 		- search table by `commuter_ac` 
	 * 		- change ride_status (enroute --> ontrip)
	 * 
	 * @param driver_ac - driver account
	 * @param commuter_ac - commuter account
	 */
	ACTION start( const name& driver_ac/*,
					const name& commuter_ac*/);

	/**
	 * @brief - finish ride
	 * @details
	 * 		- search table by `driver_ac` 
	 * 		- change ride_status (ontrip --> completed)
	 * 
	 * @param driver_ac - driver account
	 */
	ACTION finish( const name& driver_ac );

	/**
	 * @brief - Add actual fare
	 * @details - Add actual fare after the ride is completed
	 * 
	 * @param driver_ac - driver account
	 */
	ACTION addfareact(const name& driver_ac, 
						double fare_act);

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
	 * @brief - a driver receives fare
	 * @details - a driver receives fare after ride is completed, only when the pay_mode chosen as `crypto`
	 * 
	 * @param driver_ac - driver account
	 */
	ACTION recvfare( const name& driver_ac );

	/**
	 * @brief - send alert
	 * @details - send alert after any action is successfully done
	 * 
	 * @param user - driver/commuter
	 * @param message - note depending on the action
	 */
	ACTION sendalert( const name& user,
						const string& message);

	/**
	 * @brief - An external methods to erase ride
	 * @details - erase after the ride is finished & payment is done. 
	 * 
	 * @param commuter_ac erasing by searching commuter_ac
	 */
	ACTION eraseride( const name& commuter_ac);


private:
// ========TABLES========================================================================================================
	TABLE ridetaxi
	{
		name commuter_ac;
		name ride_status;			// enroute/waiting/ontrip/complete
		name driver_ac;
		double src_lat; 
		double src_lon; 
		double des_lat; 
		double des_lon;
		string vehicle_type;		// list of taxis
		uint32_t seat_count;		// set for pool, else default is 2
		string pay_mode;			// crypto or fiatdigi or fiatcash
		string pay_status;			// due or paid
		uint32_t assign_timestamp;	// at which ride is assigned
		uint32_t reachsrc_timestamp;	// at which driver reached source location to pick-up
		uint32_t start_timestamp;		// at which the ride is started
		uint32_t finish_timestamp_act;		// at which the ride is finished
		uint32_t finish_timestamp_est;		// at which the ride is estimated to finish
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
	// `faretaxi` table is for keeping the record of fare_est amount transferred by a commuter before the ride starts.
	TABLE faretaxi
	{
		asset balance;

		auto primary_key() const { return balance.amount; }
	};

	using faretaxi_index = multi_index<"faretaxi"_n, faretaxi>;

// ========Functions========================================================================================================
	// Adding inline action for `sendmsg` action in the same contract	
	void send_alert(const name& user, const string& message);


	// get the current timestamp
	inline uint32_t now() const {
		return current_time_point().sec_since_epoch();
	}



};
