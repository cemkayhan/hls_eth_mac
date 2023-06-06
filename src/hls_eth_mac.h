#if !defined(HLS_ETH_MAC_H_INCLUDE_GUARD_)
#define HLS_ETH_MAC_H_INCLUDE_GUARD_

//#include "hls_eth_mac_params.h"
#include "bit_width.h"
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

//static const auto PREAMBLE_BW_=PREAMBLE_N_OCTETS_*OCTET_BW_;
static const auto PREAMBLE_=ap_uint<56> {0x55555555555555};
//static const auto SFD_BW_=SFD_N_OCTETS_*OCTET_BW_;
static const auto START_FRAME_DELIM_=ap_uint<8> {0xD5};
//static const auto DEST_ADDR_MSB_BW_=DEST_ADDR_MSB_N_OCTETS_*OCTET_BW_;
//static const auto DEST_ADDR_LSB_BW_=DEST_ADDR_LSB_N_OCTETS_*OCTET_BW_;
//static const auto DEST_ADDR_BW_=DEST_ADDR_MSB_BW_+DEST_ADDR_LSB_BW_;
//static const auto DEST_ADDR_N_OCTETS_=DEST_ADDR_MSB_N_OCTETS_+DEST_ADDR_LSB_N_OCTETS_;
//static const auto SRC_ADDR_MSB_BW_=SRC_ADDR_MSB_N_OCTETS_*OCTET_BW_;
//static const auto SRC_ADDR_LSB_BW_=SRC_ADDR_LSB_N_OCTETS_*OCTET_BW_;
//static const auto SRC_ADDR_BW_=SRC_ADDR_MSB_BW_+SRC_ADDR_LSB_BW_;
//static const auto SRC_ADDR_N_OCTETS_=SRC_ADDR_MSB_N_OCTETS_+SRC_ADDR_LSB_N_OCTETS_;
//static const auto ETHER_TYPE_BW_=ETHER_TYPE_N_OCTETS_*OCTET_BW_; 

void Hls_Eth_Mac(hls::stream<ap_axiu<8,1,1,1> >& Gmii,
                 ap_uint<16> Dest_Addr_Msb,
                 ap_uint<32> Dest_Addr_Lsb,
                 ap_uint<16> Src_Addr_Msb,
                 ap_uint<32> Src_Addr_Lsb,
                 ap_uint<16> Ether_Type,
                 hls::stream<ap_axiu<8,1,1,1> >& Mac_Client_Data
);

#endif
