#if !defined(HLS_ETH_MAC_H_INCLUDE_GUARD_)
#define HLS_ETH_MAC_H_INCLUDE_GUARD_

#include "bit_width.h"
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

static const auto PREAMBLE_=ap_uint<56> {0x55555555555555};
static const auto START_FRAME_DELIM_=ap_uint<8> {0xD5};

void D_TOP_(
  hls::stream<ap_axiu<8,1,1,1> >& Gmii,
  ap_uint<16> Dest_Addr_Msb,
  ap_uint<32> Dest_Addr_Lsb,
  ap_uint<16> Src_Addr_Msb,
  ap_uint<32> Src_Addr_Lsb,
  ap_uint<16> Ether_Type,
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Client_Data
);

#endif
