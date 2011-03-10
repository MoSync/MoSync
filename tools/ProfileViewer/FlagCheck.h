
template<class T> class FlagCheck {
private:
	int flags;
public:
	FlagCheck() : flags(0) {}

	// set a flag.
	void set(typename T::Flags f) {
		flags |= 1 << f;
	}

	// returns true if all flags are set.
	bool check() {
		return flags == ((1 << T::_flags_end) - 1);
	}
};
