#include "../include/toeridex.hpp"

// --------------------------------------------------------------------------------
void initridex( const name& toe_owner,
					const name& type,
					const asset& toe_qty,
					uint64_t ride_qty ) {
	// require the authority of toe_owner ac - "bhubtoeindia"
	require_auth(toe_owner);

	// check the type is "driver" or "commuter"
	check( (type == "driver"_n) || (type == "commuter"_n), "invalid type");

	// check the amount is non-zero
	check( toe_qty.amount > 0, "the amount must be positive.");

	// check the symbol is 'TOE'

}
