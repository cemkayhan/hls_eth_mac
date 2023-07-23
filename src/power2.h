#if !defined(POWER2_H_INCLUDE_GUARD)
#define POWER2_H_INCLUDE_GUARD

template<int N_>
struct Power2
{
  enum {Value=2*Power2<N_-1>::Value};
};

template<>
struct Power2<0>
{
  enum {Value=1};
};       

template<>
struct Power2<1>
{
  enum {Value=2};
};       

#endif
