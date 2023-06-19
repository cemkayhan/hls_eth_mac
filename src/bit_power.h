#if !defined(BIT_POWER_H_INCLUDE_GUARD)
#define BIT_POWER_H_INCLUDE_GUARD

#include "bit_width.h"

template<int N_>
struct Bit_Power
{
  enum {Value=2*Bit_Power<N_-1>::Value};
};

template<int N_>
struct Bit_Power_Num
{
  enum {Value=Bit_Power<Bit_Width<N_>::Value>::Value};
};

template<>
struct Bit_Power<0>
{
  enum {Value=1};
};       

template<>
struct Bit_Power<1>
{
  enum {Value=2};
};       

#endif
