#include "../include/toetoken.hpp"

void toetoken::create( const name& issuer,
						const asset& maximum_supply) {
	require_auth( get_self() );

	check( is_account(issuer), "Sorry! the issuer name is not valid." );


} 

