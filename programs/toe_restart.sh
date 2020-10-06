# ================================================================================================
# Inputs
# TODO:
# 1. create a list of commuter, driver
# 2. cleost = cleost -u https://jungle3.cryptolions.io:443 


# #################################################################################################
# Contract - RIDEX
# ----------------------------------------------------------------
# delete RIDEX rides of users (if any)
cleost push action toe1111ridex testdelruacc '["toedri111111", "driver"]' -p toe1111ridex@active
cleost push action toe1111ridex testdelruacc '["toecom111111", "commuter"]' -p toe1111ridex@active

# ----------------------------------------------------------------
# delete RIDEX Wallet balances of users (if any)
cleost push action toe1111ridex testdelruwal '["toedri111111"]' -p toe1111ridex@active
cleost push action toe1111ridex testdelruwal '["toecom111111"]' -p toe1111ridex@active

# ----------------------------------------------------------------
# delete RIDEX spec of TOE
cleost push action toe1111ridex testdelridex '["driver"]' -p toe1111ridex@active
cleost push action toe1111ridex testdelridex '["commuter"]' -p toe1111ridex@active

# ================================================================================================
# Contract - Ride Wallet
cleost push action toe14rwallet testdelrwal '["toecom111111"]' -p toe14rwallet@active
cleost push action toe14rwallet testdelrwal '["toecom111112"]' -p toe14rwallet@active

# ================================================================================================
# Contract - Ride taxi
# ----------------------------------------------------------------
# delete a ride
cleost push action toe1ridetaxi testdelride '["57a941cb6326e205c5cf042f0c4098b568fc40507fa3a80d3e9c15e148406628"]' -p toe1ridetaxi@active

# ================================================================================================
# Contract - Ride token

# ================================================================================================
# Contract - User auth
