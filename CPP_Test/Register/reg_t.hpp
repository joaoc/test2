#include <generate_mask.hpp>
#include <rw_t.hpp>
#include <ro_t.hpp>
#include <wo_t.hpp>

template<typename mut_t, uint32_t addr, int offset, int width>
struct reg_t
{
   static_assert(width > 0, "nonpositive width");
   static_assert(offset >= 0, "negative offset");
   static_assert(width + offset <= std::numeric_limits<uint32_t>::digits, "width overflow");

   static uint32_t read()
   {
      mut_t::read(
         reinterpret_cast<volatile uint32_t*>(addr), offset,
         generate_mask(offset, width));
   }

   static void write(uint32_t val)
   {
      mut_t::write(
         reinterpret_cast<volatile uint32_t*>(addr), offset,
         generate_mask(offset, width), val);
   }
};


#include <climits>

using namespace std;


template <typename type, uint32_t n_bits, uint32_t offset>
static constexpr type generate_bitmask()
{
	static_assert(n_bits+offset<=(sizeof(type)*8),"Offset+n_bits is out of bounds");
	return ( ( (1UL << n_bits)-1 ) << offset);
	//return bitmask<type>(n_bits)<<offset;
}
