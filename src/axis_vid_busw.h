#if !defined(AXIS_VID_BUSW_H_INCLUDE_GUARD_)
#define AXIS_VID_BUSW_H_INCLUDE_GUARD_

template<int DEPTH_,int PPC_>
struct Axis_Vid_Busw
{
  enum {Value=8*((3*DEPTH_*PPC_)/8+(((3*DEPTH_*PPC_)%8)>0))};
};

#endif
