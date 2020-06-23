#include <eosio/eosio.hpp>

using eosio::contract;
using eosio::print;
using std::string;
using eosio::multi_index;

class [[eosio::contract]] cabeosride : public contract
{
public:
	using contract::contract;
	
	[[eosio::action]]
	void create(name dummy_ac) {
		require_auth( get_self() );

		ride_index rides(get_self(), )

	}


private:
	struct [[eosio::table]] ride
	{
		name driver_ac;
		string ride_status,
	};

	using ride_index = multi_index<"ridetable"_n, ride>
};
