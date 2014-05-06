#include "bitset.hh"

int main(void)
{
	// non thread-safe bitsets
	bitset<> my_bitset1(42);
	bitset<false> my_bitseté(42);

	// thread-safe bitset
	bitset<true> my_bitset3(42);


	return 0;
}
