#include <eosio/eosio.hpp>

using eosio::contract;
using eosio::print;
using eosio::name;


class [[eosio::contract]] hello : public contract {
public:
	using contract::contract;

	[[eosio::action]]
	void hi(name user) {
		print("Hello, ", user);
	}
};

EOSIO_DISPATCH(hello, (hi))