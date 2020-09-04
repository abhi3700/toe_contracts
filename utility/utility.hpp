template<typename CharT>
static std::string to_hex(const CharT* d, uint32_t s) {
  std::string r;
  const char* to_hex="0123456789abcdef";
  uint8_t* c = (uint8_t*)d;
  for( uint32_t i = 0; i < s; ++i ) {
    (r += to_hex[(c[i] >> 4)]) += to_hex[(c[i] & 0x0f)];
  }
  return r;
}

// get the sha256 hash digest/checksum in string
inline string hash_str_256(const name& commuter_ac,
									uint32_t create_timestamp) const {
	string data_str_cpp = commuter_ac.to_string() + std::to_string(create_timestamp);
	const char * data_str_c = data_str_cpp.c_str(); 

	auto hash_digest = sha256(data_str_c, strlen(data_str_c));

	string hash_digest_str = to_hex(&hash_digest, sizeof(hash_digest));

	return hash_digest_str;
}

// get the sha256 hash digest/checksum
inline checksum256 hash_digest_256(const name& commuter_ac,
									uint32_t create_timestamp) const {
	string data_str_cpp = commuter_ac.to_string() + std::to_string(create_timestamp);
	const char * data_str_c = data_str_cpp.c_str(); 

	auto hash_digest = sha256(data_str_c, strlen(data_str_c));

	return hash_digest;
}


/*
	TODO: to use uint256_t, click here - https://github.com/ckormanyos/wide-integer
		- Apply this code line: 
			+ const uint256_t a("0xF4DF741DE58BCB2F37F18372026EF9CBCFC456CB80AF54D53BDEED78410065DE");
*/
// extract integers from string
inline uint256_t ext_int_string(const string& s ) const {
    // string input = "1ee5ad0d074cfffbaa1598f0d236c09537d9c28b3b85aff206e20398f5e032dd";//I added some more extra characters to prove my point.
    string numbers = "0123456789";
    std::size_t found = s.find_first_of(numbers.c_str());
    string output_str = "";

    while (found != string::npos) {
        output_str += s[found];
        found = s.find_first_of(numbers.c_str(), found+1);
    }

    // std::cout << output_str << "\n";
    // std::cout << "length" << output_str.size() << "\n";
    uint256_t output_int = = (uint256_t)output_str;
    // std::cout << output_int << "\n";

    return output_int;
}

/**
 * @brief - get transaction id. Also, possible in string by using `to_hex()` function
 * 
 */
inline checksum256 get_trxid()
{
  auto trxsize = transaction_size();
  char trxbuf[trxsize];
  uint32_t trxread = read_transaction( trxbuf, trxsize );
  check( trxsize == trxread, "read_transaction failed");
  return sha256(trxbuf, trxsize);
}

// NOTE: vector arg can't be const as emplace_back is non-const method
inline void creatify_vector_pair( vector<pair<string, checksum256>>& v, const string& s, const checksum256& val ) {
  auto s_it = std::find_if(v.begin(), v.end(), [&](auto& vs){ return vs.first == s; });
  if(s_it != v.end()) {   // key found
    s_it->second = val;
  }
  else {            // key NOT found
    v.emplace_back(make_pair(s, val));
  }
}


inline void read_vector_pair( const vector<pair<string, checksum256>>& v, const string& s ) {
  auto s_it = std::find_if(v.begin(), v.end(), [&](auto& vs){ return vs.first == s; });

  if(s_it != v.end()) {     // key found
    print("The value(s): ");

    while(s_it != v.end()) {
      print(s_it->second, " | ");
      ++s_it;
    }
  } else {            // key NOT found
    print("No item found with this key");
  }
}
