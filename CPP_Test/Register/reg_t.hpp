#pragma once

#include <limits>


template <typename type, type addr>
struct reg_t
{
	static type read()
	{
		return *reinterpret_cast<volatile type*>(addr);
	}

	static void write(type val, type bitmask)
	{
		*reinterpret_cast<volatile type*>(addr) ^= ((*reinterpret_cast<volatile type*>(addr) ^ val) & bitmask);
	}

	static void set(type val)
	{
		*reinterpret_cast<volatile type*>(addr) |=val;
	}

	static void clear(type val)
	{
		*reinterpret_cast<volatile type*>(addr) &=~val;
	}
};



template <typename type, type n_bits, type offset>
static constexpr type generate_bitmask()
{
	static_assert(n_bits+offset<=std::numeric_limits<type>::digits,"Offset+n_bits is out of bounds");
	return ( ( (1UL << n_bits)-1 ) << offset);
	//return bitmask<type>(n_bits)<<offset;
}
