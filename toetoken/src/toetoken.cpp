#include "../include/toetoken.hpp"

void toetoken::create( const name& issuer,
						const asset& maximum_supply) {
	require_auth( get_self() );

	check( is_account(issuer), "Sorry! the issuer name is not valid." );

	auto sym = maximum_supply.symbol;
	check( sym.is_valid(), "invalid symbol name");
	check( maximum_supply.is_valid(), "invalid supply");
	check( maximum_supply.amount > 0, "max-supply must be positive");

	stats stats_table( get_self(), sym.code().raw() );
	auto stats_it = stats_table.find(sym.code().raw());

	check(stats_it == stats_table.end(), "token with this symbol already exist");

	stats_table.emplace(get_self(), [&](auto& row) {
		row.issuer = issuer;
		row.max_supply = maximum_supply;
	});



} 

