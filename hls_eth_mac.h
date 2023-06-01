#if !defined(HLS_ETH_MAC_INCLUDE_GUARD_H_)
#define HLS_ETH_MAC_INCLUDE_GUARD_H_

#include "ap_int.h"
#include "hls_stream.h"

#if defined(D_HLS_CSIM_)
#include<ostream>
#endif

template<int OCTET_BW_>
struct Gmii_Ports {
  ap_uint<OCTET_BW_> Tx_D;
  ap_uint<1> Tx_Er;
};

#if defined(D_HLS_CSIM_)
template<int OCTET_BW_>
inline std::ostream& operator<<(std::ostream& os, const Gmii_Ports<OCTET_BW_>& Ports){
   os << "Tx_D: " << std::hex << Ports.Tx_D << ", " 
      << "Tx_Er: " << std::dec << Ports.Tx_Er;
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

static const auto D_LENGTH_TYPE_BW_ = D_LENGTH_TYPE_N_OCTETS_*D_OCTET_BW_; 
static const auto D_DEST_ADDR_MSB_BW_ = D_DEST_ADDR_MSB_N_OCTETS_*D_OCTET_BW_;
static const auto D_DEST_ADDR_LSB_BW_ = D_DEST_ADDR_LSB_N_OCTETS_*D_OCTET_BW_;
static const auto D_SRC_ADDR_MSB_BW_ = D_SRC_ADDR_MSB_N_OCTETS_*D_OCTET_BW_;
static const auto D_SRC_ADDR_LSB_BW_ = D_SRC_ADDR_LSB_N_OCTETS_*D_OCTET_BW_;

void Hls_Eth_Mac(Gmii_Intf<D_MAX_MTU_,D_OCTET_BW_>& Gmii,
                 hls::stream<ap_uint<D_OCTET_BW_> >& Mac_Client_Data,
                 ap_uint<1> Gen_Rand_Mac_Client_Data,
                 ap_uint<D_LENGTH_TYPE_BW_> Length_Type,
                 ap_uint<D_DEST_ADDR_MSB_BW_> Dest_Addr_Msb,
                 ap_uint<D_DEST_ADDR_LSB_BW_> Dest_Addr_Lsb,
                 ap_uint<D_SRC_ADDR_MSB_BW_> Src_Addr_Msb,
                 ap_uint<D_SRC_ADDR_LSB_BW_> Src_Addr_Lsb);

#endif
