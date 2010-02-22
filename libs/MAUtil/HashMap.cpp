#include "HashMap.h"

//******************************************************************************
// THashFunction
//******************************************************************************

template<> hash_val_t MAUtil::THashFunction<MAUtil::String>(const MAUtil::String& s) {
	return hash_fun_default(s.c_str());
}

template<> hash_val_t MAUtil::THashFunction<int>(const int& data) {
	int key = data;
	key = key + ~(key << 15); // key = key - (key << 15) - 1;
	key = key ^ (key >> 10);
	key = key + (key << 3);
	key = key ^ (key >> 6);
	key = key * 16389; // key = (key + (key << 2)) + (key << 14);
	key = key ^ (key >> 16);
	return key;
}
