#include "hls_eth_mac.h"

static const auto D_PREAMBLE_BW_=D_PREAMBLE_N_OCTETS_*D_OCTET_BW_;
static const ap_uint<D_PREAMBLE_BW_> Preamble = 
  0b10101010101010101010101010101010101010101010101010101010;

static const auto D_SFD_BW_=D_SFD_N_OCTETS_*D_OCTET_BW_;
static const ap_uint<D_SFD_BW_> Start_Frame_Delim = 0b10101011;

static const auto D_DEST_ADDR_N_OCTETS_=D_DEST_ADDR_MSB_N_OCTETS_+D_DEST_ADDR_LSB_N_OCTETS_;
static const auto D_SRC_ADDR_N_OCTETS_=D_SRC_ADDR_MSB_N_OCTETS_+D_SRC_ADDR_LSB_N_OCTETS_;

static const auto D_DEST_ADDR_BW_=D_DEST_ADDR_MSB_BW_+D_DEST_ADDR_LSB_BW_;
static const auto D_SRC_ADDR_BW_=D_SRC_ADDR_MSB_BW_+D_SRC_ADDR_LSB_BW_;

void Hls_Eth_Mac(Gmii_Intf<D_MAX_MTU_,D_OCTET_BW_>& Gmii,
                 hls::stream<ap_uint<D_OCTET_BW_> >& Mac_Client_Data,
                 ap_uint<1> Gen_Rand_Mac_Client_Data,
                 ap_uint<D_LENGTH_TYPE_BW_> Length_Type,
                 ap_uint<D_DEST_ADDR_MSB_BW_> Dest_Addr_Msb,
                 ap_uint<D_DEST_ADDR_LSB_BW_> Dest_Addr_Lsb,
                 ap_uint<D_SRC_ADDR_MSB_BW_> Src_Addr_Msb,
                 ap_uint<D_SRC_ADDR_LSB_BW_> Src_Addr_Lsb
) {
#if defined(D_AP_CTRL_NONE_)
#pragma HLS INTERFACE ap_ctrl_none port=return
#endif

#pragma HLS INTERFACE axis port=Mac_Client_Data
#pragma HLS INTERFACE ap_fifo port=Gmii.Ports

#if defined(D_S_AXILITE_)
#pragma HLS INTERFACE s_axilite port=return bundle=Ctrl
#pragma HLS INTERFACE s_axilite offset=0x10 bundle=Ctrl port=Length_Type
#pragma HLS INTERFACE s_axilite offset=0x18 bundle=Ctrl port=Dest_Addr_Msb
#pragma HLS INTERFACE s_axilite offset=0x20 bundle=Ctrl port=Dest_Addr_Lsb
#pragma HLS INTERFACE s_axilite offset=0x28 bundle=Ctrl port=Src_Addr_Msb
#pragma HLS INTERFACE s_axilite offset=0x30 bundle=Ctrl port=Src_Addr_Lsb
#pragma HLS INTERFACE s_axilite offset=0x38 bundle=Ctrl port=Gen_Rand_Mac_Client_Data
#else
#pragma HLS INTERFACE ap_none port=Gen_Rand_Mac_Client_Data,Length_Type
#pragma HLS INTERFACE ap_none port=Dest_Addr_Msb,Dest_Addr_Lsb
#pragma HLS INTERFACE ap_none port=Src_Addr_Msb,Src_Addr_Lsb
#pragma HLS STABLE variable=Dest_Addr_Msb,Dest_Addr_Lsb
#pragma HLS STABLE variable=Gen_Rand_Mac_Client_Data,Length_Type
#pragma HLS STABLE variable=Src_Addr_Msb,Src_Addr_Lsb
#endif

#pragma HLS PIPELINE

  ap_uint<D_LENGTH_TYPE_BW_> Length_Type_ = Length_Type;
  ap_uint<D_DEST_ADDR_BW_> Dest_Addr_ = (Dest_Addr_Msb,Dest_Addr_Lsb);
  ap_uint<D_SRC_ADDR_BW_> Src_Addr_ = (Src_Addr_Msb,Src_Addr_Lsb);
  ap_uint<1> Gen_Rand_Mac_Client_Data_ = Gen_Rand_Mac_Client_Data;
  unsigned Dummy_Cntr=0;

  loopPreamble: for(auto i=0;i<D_PREAMBLE_N_OCTETS_;++i){
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {Preamble(i*D_OCTET_BW_+D_OCTET_BW_-1,i*D_OCTET_BW_),0};
  }
  Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {Start_Frame_Delim,0};

  loopDestAddr: for(auto i=0;i<D_DEST_ADDR_N_OCTETS_;++i){
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {Dest_Addr_(i*D_OCTET_BW_+D_OCTET_BW_-1,i*D_OCTET_BW_),0};
  }

  loopSrcAddr: for(auto i=0;i<D_SRC_ADDR_N_OCTETS_;++i){
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {Src_Addr_(i*D_OCTET_BW_+D_OCTET_BW_-1,i*D_OCTET_BW_),0};
  }
  Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {Length_Type_(0*D_OCTET_BW_+D_OCTET_BW_-1,0*D_OCTET_BW_),0};
  Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {Length_Type_(1*D_OCTET_BW_+D_OCTET_BW_-1,1*D_OCTET_BW_),0};

  loopMacClientData: for(auto i=0; i<Length_Type_; ++i){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=D_MAX_MTU_ max=D_MAX_MTU_
    if(Gen_Rand_Mac_Client_Data_)
      Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {i,0};
    else
      Gmii[Dummy_Cntr++]=Gmii_Ports<D_OCTET_BW_> {Mac_Client_Data.read(),0};
  }
}
