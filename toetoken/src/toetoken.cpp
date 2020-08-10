#include "../include/toetoken.hpp"

// --------------------------------------------------------------------------------------------------------------------
void toetoken::create( const name& issuer,
						const asset& maximum_supply) 
{
	require_auth( get_self() );

	check( is_account(issuer), "Sorry! the issuer name is not valid." );

	auto sym = maximum_supply.symbol;
	check( sym.is_valid(), "invalid symbol name");
	check( maximum_supply.is_valid(), "invalid supply");
	check( maximum_supply.amount > 0, "max-supply must be positive");

	stats_index stats_table( get_self(), sym.code().raw() );
	auto stats_it = stats_table.find(sym.code().raw());

	check(stats_it == stats_table.end(), "token with this symbol already exist");

	stats_table.emplace(get_self(), [&](auto& row) {
		row.supply.symbol = sym;
		row.max_supply = maximum_supply;
		row.issuer = issuer;
	});
}


// --------------------------------------------------------------------------------------------------------------------
void toetoken::issue( const name& to,
						const asset& quantity,
						const string& memo) 
{
	auto sym = quantity.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check(memo.size() <= 256, "memo has more than 256 bytes");
	
	stats_index stats_table( get_self(), sym.code().raw() );
	auto stats_it = stats_table.find(sym.code().raw());

	check( stats_it != stats_table.end(), "token with symbol does not exist, create token before issue" );
	check( to == stats_it->issuer, "tokens can only be issued to issuer account" );

	require_auth( stats_it->issuer );
	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must issue positive quantity" );

	check( quantity.symbol == (stats_it->supply).symbol, "symbol precision mismatch");
	check( quantity.amount <= (stats_it->max_supply).amount - (stats_it->supply).amount, "quantity exceeds available supply" );

	stats_table.modify( stats_it, same_payer, [&](auto& row) {
		row.supply += quantity;
	});

	add_balance( stats_it->issuer, quantity, stats_it->issuer);
}


// --------------------------------------------------------------------------------------------------------------------
void toetoken::retire( const asset& quantity,
						const string& memo )
{
	auto sym = quantity.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check( memo.size() <= 256, "memo has more than 256 bytes");

	stats_index stats_table( get_self(), sym.code().raw() );
	auto stats_it = stats_table.find(sym.code().raw());

	check( stats_it != stats_table.end(), "token with symbol does not exist");

	require_auth( stats_it->issuer );
	check( quantity.is_valid(), "invalid quantity" );
	check(quantity.amount > 0, "must retire positive quantity");

	check(sym == stats_it->supply.symbol, "symbol precision mismatch");

	stats_table.modify( stats_it, same_payer, [&](auto& row){
		row.supply -= quantity;
	});

	sub_balance( stats_it->issuer, quantity );
}

// --------------------------------------------------------------------------------------------------------------------
void toetoken::transfer( const name& from,
							const name& to,
							const asset& quantity,
							const string& memo)
{
	check(from != to, "cannot transfer to self");
	
	require_auth(from);
	
	check(is_account(to), "to account does not exist");

	auto sym = quantity.symbol;
	stats_index stats_table(get_self(), sym.code().raw());
	const auto& st = stats_table.get(sym.code().raw());

	require_recipient(from);
	require_recipient(to);

	check(quantity.is_valid(), "invalid quantity");
	check(quantity.amount > 0, "must transfer positive quantity");
	check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
	check(memo.size() <= 256, "memo has more than 256 bytes");

	auto payer = has_auth(to) ? to : from;

	sub_balance( from, quantity );
	add_balance( to, quantity, payer );
}

// --------------------------------------------------------------------------------------------------------------------
void toetoken::add_balance( const name& owner,
							const asset& value,
							const name& ram_payer) 
{
	accounts_index to_acnts( get_self(), owner.value );
	auto to = to_acnts.find( value.symbol.code().raw() );

	if( to == to_acnts.end() ) {
		to_acnts.emplace( ram_payer, [&](auto& row) {
			row.balance = value;
		});
	} else {
		to_acnts.modify( to, same_payer, [&](auto& row) {
			row.balance += value;
		});
	}

}

// --------------------------------------------------------------------------------------------------------------------
void toetoken::sub_balance( const name& owner,
							const asset& value)
{
	accounts_index from_acnts( get_self(), owner.value);

	const auto& from = from_acnts.get(value.symbol.code().raw(), "no balance object found" );
	check( from.balance.amount >= value.amount, "overdrawn balance" );

	from_acnts.modify( from, owner, [&](auto& row){
		row.balance -= value;
	});
}		

// --------------------------------------------------------------------------------------------------------------------
void toetoken::open( const name& owner, 
						const symbol& symbol,
						const name& ram_payer )
{
	require_auth( ram_payer );
	check( is_account( owner), "owner account does not exist");

	auto sym_code_raw = symbol.code().raw();
	stats_index stats_table( get_self(), sym_code_raw );
	const auto& st = stats_table.get(sym_code_raw, "symbol does not exist");
	check( st.supply.symbol == symbol, "symbol precision mismatach");

	accounts_index acnts( get_self(), owner.value );
	auto acc_it = acnts.find( sym_code_raw);

	if(acc_it == acnts.end()) {
		acnts.emplace(ram_payer, [&](auto& row){
			row.balance = asset{0, symbol};
		});

	}
}

// --------------------------------------------------------------------------------------------------------------------
void toetoken::close( const name& owner,
						const symbol& symbol)
{
	require_auth( owner );
	accounts_index acnts( get_self(), owner.value);
	auto it = acnts.find(symbol.code().raw());

	check(it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect.");
	check(it->balance.amount == 0, "Cannot close because the balance is not zero");	
	acnts.erase(it);
}

// --------------------------------------------------------------------------------------------------------------------
void toetoken::setinflation(const name& issuer, 
					uint64_t year, 
					float inflate_rate_percent)
{
	symbol sym {"TOE",4};
	stats_index stats_table( get_self(), sym.code().raw() );
	auto stats_it = stats_table.find(sym.code().raw());

	check( stats_it != stats_table.end(), "token with symbol does not exist");

	check( issuer == stats_it->issuer, "tokens can only be issued to issuer account" );

	require_auth( stats_it->issuer );

	check(get_year_length(year) != 4, "this is not an year");

	check(inflate_rate_percent != 0, "inflation rate can't be zero.");

	rates_index rates_table(get_self(), sym.code().raw());
	auto rates_it = rates_table.find(year);

	check(rates_it == rates_table.end(), "the inflation_rate for the year already exists. So, can't be modified");

	rates_table.emplace(issuer, [&](auto& row) {
		row.year = year;
		row.inflation_rate_percent = inflate_rate_percent;
	});

}

// --------------------------------------------------------------------------------------------------------------------
void toetoken::inflate( const name& issuer,
				uint64_t year )
{
	symbol sym {"TOE",4};
	stats_index stats_table( get_self(), sym.code().raw() );
	auto stats_it = stats_table.find(sym.code().raw());

	check( stats_it != stats_table.end(), "token with symbol does not exist");

	check( issuer == stats_it->issuer, "tokens can only be issued to issuer account" );

	require_auth( stats_it->issuer );

	rates_index rates_table(get_self(), sym.code().raw());
	auto rates_it = rates_table.find(year);

	check(rates_it != rates_table.end(), "the inflation_rate for the year is not found. Please set using \'setinflation\' action.");

	auto inflated_amount = (rates_it->inflation_rate_percent) * supply_initial_amount;

	stats_table.modify(stats_it, same_payer, [&](auto& row){
		row.supply.amount += inflated_amount;
	});
}

// --------------------------------------------------------------------------------------------------------------------
// void toetoken::deltokenstat(const symbol_code& sym_code, const string& memo) {
// 	require_auth(get_self());

// 	// symbol sym {"EOS",4};
// 	stats_index stats_table( get_self(), sym_code.raw() );
// 	auto stats_it = stats_table.find(sym_code.raw());

// 	check(stats_it != stats_table.end(), "EOS token doesn't exist");
// 	stats_table.erase(stats_it);
// }

