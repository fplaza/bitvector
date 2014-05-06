#ifndef BITSET_HH
#define BITSET_HH

#include <stdint.h>
#include <vector>

template <bool thread_safe = false>
class bitset
{
	public:
		typedef uint64_t size_type;

		bitset(size_type n);
		size_type size() const;
		void set(size_type n);
		void unset(size_type n);
		void flip(size_type n);
		bool get(size_type n) const;
		bool any() const;
		bool none() const;
		size_type count() const;
	private:
		typedef uint32_t word_t;
		static const size_type WORD_SIZE =  sizeof(word_t)*8;

		size_type index(size_type i) const;
		word_t mask(size_type i) const; 

		size_type size_;
		std::vector<word_t> data_;

};

template <bool thread_safe>
inline bitset<thread_safe>::bitset(size_type n)
	: size_(n), data_((n+WORD_SIZE-1)/WORD_SIZE)
{};

template <bool thread_safe>
typename bitset<thread_safe>::size_type bitset<thread_safe>::size() const
{
	return size_;
}

template <bool thread_safe>
inline typename bitset<thread_safe>::size_type bitset<thread_safe>::index(size_type i) const
{
	return i/WORD_SIZE;
}

template <bool thread_safe>
inline typename bitset<thread_safe>::word_t bitset<thread_safe>::mask(size_type i) const
{
	return 1 << (i%WORD_SIZE);
}

template <>
inline void bitset<false>::set(size_type n)
{
	data_[index(n)] |= mask(n);
};

template <>
inline void bitset<true>::set(size_type n)
{
	__sync_or_and_fetch(&data_[index(n)], mask(n));
};


template <>
inline void bitset<false>::unset(size_type n)
{
	data_[index(n)] &= ~mask(n);
};

template <>
inline void bitset<true>::unset(size_type n)
{
	__sync_and_and_fetch(&data_[index(n)], ~mask(n));
};


template <>
inline void bitset<false>::flip(size_type n)
{
	data_[index(n)] ^= mask(n);
};

template <>
inline void bitset<true>::flip(size_type n)
{
	__sync_xor_and_fetch(&data_[index(n)], ~mask(n));
};


template <bool thread_safe>
inline bool bitset<thread_safe>::get(size_type n) const
{
	return data_[index(n)] & mask(n);
};

template <bool thread_safe>
inline bool bitset<thread_safe>::any() const
{
	for (size_type i = 0; i < data_.size(); i++)
	{
		if (data_[i])
			return true;
	}

	return false;
}

template <bool thread_safe>
inline bool bitset<thread_safe>::none() const
{
	return !any();
}

template <bool thread_safe>
inline typename bitset<thread_safe>::size_type bitset<thread_safe>::count() const
{
	size_type popcount = 0;

	for (size_type i = 0; i < data_.size(); i++)
	{
		popcount += __builtin_popcount(data_[i]);
	}

	return popcount;
}
#endif //BITSET_HH
