#ifndef HASH_COMPARE_H
#define HASH_COMPARE_H

namespace __gnu_cxx {
        template<> struct hash<std::string>
        {
                size_t operator()(const std::string& __s) const
                { return __stl_hash_string(__s.c_str()); }
        };
#ifdef DARWIN
	template<class T> struct hash<T*> {
		size_t operator()(T* ptr) const {
			return hash<size_t>()((size_t)ptr);
		}
	};
#endif	//DARWIN
}
template<class T> class hash_compare : public hash<T> {};

#endif	//HASH_COMPARE_H
