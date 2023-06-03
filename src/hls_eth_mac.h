#if !defined(HLS_ETH_MAC_H_INCLUDE_GUARD_)
#define HLS_ETH_MAC_H_INCLUDE_GUARD_

#include "hls_eth_mac_params.h"
#include "bit_width.h"
#include "ap_int.h"
#include "hls_stream.h"

#if 1==HLS_CSIM_
#include<ostream>
#endif

template<int OCTET_BW_>
struct Gmii_Ports {
  ap_uint<OCTET_BW_> Tx_D;
  ap_uint<1> Tx_Er;
};

#if 1==HLS_CSIM_
template<int OCTET_BW_>
inline std::ostream& operator<<(std::ostream& os, const Gmii_Ports<OCTET_BW_>& Ports){
   os << "Tx_D: " << std::hex << Ports.Tx_D << ", " 
      << "Tx_Er: " << std::dec << Ports.Tx_Er;
   return os;
}
#endif

template<int N_OCTETS_,int OCTET_BW_>
struct Gmii_Intf {
  Gmii_Ports<OCTET_BW_> Ports[N_OCTETS_];
  Gmii_Ports<OCTET_BW_>& operator[](int index){
#pragma HLS INLINE
    return Ports[index];
  }
};

static const auto PREAMBLE_BW_=PREAMBLE_N_OCTETS_*OCTET_BW_;
static const auto PREAMBLE_=ap_uint<PREAMBLE_BW_> {0x55555555555555};
static const auto SFD_BW_=SFD_N_OCTETS_*OCTET_BW_;
static const auto START_FRAME_DELIM_=ap_uint<SFD_BW_> {0xD5};
static const auto DEST_ADDR_MSB_BW_=DEST_ADDR_MSB_N_OCTETS_*OCTET_BW_;
static const auto DEST_ADDR_LSB_BW_=DEST_ADDR_LSB_N_OCTETS_*OCTET_BW_;
static const auto DEST_ADDR_BW_=DEST_ADDR_MSB_BW_+DEST_ADDR_LSB_BW_;
static const auto DEST_ADDR_N_OCTETS_=DEST_ADDR_MSB_N_OCTETS_+DEST_ADDR_LSB_N_OCTETS_;
static const auto SRC_ADDR_MSB_BW_=SRC_ADDR_MSB_N_OCTETS_*OCTET_BW_;
static const auto SRC_ADDR_LSB_BW_=SRC_ADDR_LSB_N_OCTETS_*OCTET_BW_;
static const auto SRC_ADDR_BW_=SRC_ADDR_MSB_BW_+SRC_ADDR_LSB_BW_;
static const auto SRC_ADDR_N_OCTETS_=SRC_ADDR_MSB_N_OCTETS_+SRC_ADDR_LSB_N_OCTETS_;
static const auto ETHER_TYPE_BW_=ETHER_TYPE_N_OCTETS_*OCTET_BW_; 

void Hls_Eth_Mac(Gmii_Intf<MAX_MTU_,OCTET_BW_>& Gmii,
                 ap_uint<DEST_ADDR_MSB_BW_> Dest_Addr_Msb,
                 ap_uint<DEST_ADDR_LSB_BW_> Dest_Addr_Lsb,
                 ap_uint<SRC_ADDR_MSB_BW_> Src_Addr_Msb,
                 ap_uint<SRC_ADDR_LSB_BW_> Src_Addr_Lsb,
                 ap_uint<ETHER_TYPE_BW_> Ether_Type,
                 hls::stream<ap_uint<OCTET_BW_> >& Mac_Client_Data,
                 ap_uint<Bit_Width<MAX_MTU_>::Value > Mac_Client_Data_Len,
                 ap_uint<1> Gen_Rand_Mac_Client_Data);

#endif
