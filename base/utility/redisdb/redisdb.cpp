#pragma once
#include <eosio/eosio.hpp>
#include <sw/redis++/redis++.h>
// #include <string>

using eosio::contract;
using eosio::check;
using eosio::print;
using std::string;


class [[eosio::contract]] redisdb : public contract
{
public:
	using contract::contract;

	const string redis_uri = "tcp://pd4ecec34154bbca551fdeafb94421d0ec41147dab602a6a878e6509ae49f638a@ec2-54-209-85-193.compute-1.amazonaws.com:11989";

	[[eosio::action]]
	void displayn(const string& key_name) {
		try {
			auto r = Redis(redis_uri);
			auto val = r.hget("person2", key_name);
			if(val) {
				print("person 2's name is: ", *val);
			}
		catch(const std::exception& e) {
			std::cerr << e.what() << '\n';
		}

	}
};