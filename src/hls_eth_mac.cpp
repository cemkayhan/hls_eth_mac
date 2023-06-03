#include "hls_eth_mac.h"
#include "crc32.h"

void Hls_Eth_Mac(Gmii_Intf<MAX_MTU_,OCTET_BW_>& Gmii,
                 ap_uint<DEST_ADDR_MSB_BW_> Dest_Addr_Msb,
                 ap_uint<DEST_ADDR_LSB_BW_> Dest_Addr_Lsb,
                 ap_uint<SRC_ADDR_MSB_BW_> Src_Addr_Msb,
                 ap_uint<SRC_ADDR_LSB_BW_> Src_Addr_Lsb,
                 ap_uint<ETHER_TYPE_BW_> Ether_Type,
                 hls::stream<ap_uint<OCTET_BW_> >& Mac_Client_Data,
                 ap_uint<Bit_Width<MAX_MTU_>::Value > Mac_Client_Data_Len,
                 ap_uint<1> Gen_Rand_Mac_Client_Data
) {
#if 1==AP_CTRL_NONE_
#pragma HLS INTERFACE ap_ctrl_none port=return
#endif

#pragma HLS INTERFACE axis port=Mac_Client_Data
#pragma HLS INTERFACE ap_fifo port=Gmii.Ports

#if 1==S_AXILITE_
#pragma HLS INTERFACE s_axilite port=return bundle=Ctrl
#pragma HLS INTERFACE s_axilite offset=0x10 bundle=Ctrl port=Dest_Addr_Msb
#pragma HLS INTERFACE s_axilite offset=0x18 bundle=Ctrl port=Dest_Addr_Lsb
#pragma HLS INTERFACE s_axilite offset=0x20 bundle=Ctrl port=Src_Addr_Msb
#pragma HLS INTERFACE s_axilite offset=0x28 bundle=Ctrl port=Src_Addr_Lsb
#pragma HLS INTERFACE s_axilite offset=0x30 bundle=Ctrl port=Ether_Type
#pragma HLS INTERFACE s_axilite offset=0x38 bundle=Ctrl port=Mac_Client_Data_Len
#pragma HLS INTERFACE s_axilite offset=0x40 bundle=Ctrl port=Gen_Rand_Mac_Client_Data
#else
#pragma HLS INTERFACE ap_none port=Dest_Addr_Msb,Dest_Addr_Lsb
#pragma HLS INTERFACE ap_none port=Src_Addr_Msb,Src_Addr_Lsb
#pragma HLS INTERFACE ap_none port=Ether_Type,Mac_Client_Data_Len
#pragma HLS INTERFACE ap_none port=Gen_Rand_Mac_Client_Data
#pragma HLS STABLE variable=Dest_Addr_Msb,Dest_Addr_Lsb
#pragma HLS STABLE variable=Src_Addr_Msb,Src_Addr_Lsb
#pragma HLS STABLE variable=Ether_Type,Mac_Client_Data_Len
#pragma HLS STABLE variable=Gen_Rand_Mac_Client_Data
#endif

#pragma HLS PIPELINE

  Crc32<OCTET_BW_,CRC32_N_OCTETS_> Crc32_;
  ap_uint<ETHER_TYPE_BW_> Ether_Type_=Ether_Type;
  ap_uint<DEST_ADDR_BW_> Dest_Addr_=(Dest_Addr_Msb,Dest_Addr_Lsb);
  ap_uint<SRC_ADDR_BW_> Src_Addr_=(Src_Addr_Msb,Src_Addr_Lsb);
  ap_uint<Bit_Width<MAX_MTU_>::Value > Mac_Client_Data_Len_=Mac_Client_Data_Len;
  ap_uint<1> Gen_Rand_Mac_Client_Data_=Gen_Rand_Mac_Client_Data;
  unsigned Dummy_Cntr=0;

#if 1==LSB_FIRST_
  loopPreambleLsb: for(auto i=0;i<PREAMBLE_N_OCTETS_;++i){
#endif
#if 1==MSB_FIRST_
  loopPreambleMsb: for(auto i=PREAMBLE_N_OCTETS_-1;i>-1;--i){
#endif
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {PREAMBLE_(i*OCTET_BW_+OCTET_BW_-1,i*OCTET_BW_),0};
  }
  Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {START_FRAME_DELIM_,0};

#if 1==LSB_FIRST_
  loopDestAddrLsb: for(auto i=0;i<DEST_ADDR_N_OCTETS_;++i){
#endif
#if 1==MSB_FIRST_
  loopDestAddrMsb: for(auto i=DEST_ADDR_N_OCTETS_-1;i>-1;--i){
#endif
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {Crc32_.Update_Crc(Dest_Addr_(i*OCTET_BW_+OCTET_BW_-1,i*OCTET_BW_)),0};
  }

#if 1==LSB_FIRST_
  loopSrcAddrLsb: for(auto i=0;i<SRC_ADDR_N_OCTETS_;++i){
#endif
#if 1==MSB_FIRST_
  loopSrcAddrMsb: for(auto i=SRC_ADDR_N_OCTETS_-1;i>-1;--i){
#endif
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {Crc32_.Update_Crc(Src_Addr_(i*OCTET_BW_+OCTET_BW_-1,i*OCTET_BW_)),0};
  }

#if 1==LSB_FIRST_
  loopLengthTypeLsb: for(auto i=0;i<ETHER_TYPE_N_OCTETS_;++i){
#endif
#if 1==MSB_FIRST_
  loopLengthTypeMsb: for(auto i=ETHER_TYPE_N_OCTETS_-1;i>-1;--i){
#endif
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {Crc32_.Update_Crc(Ether_Type_(i*OCTET_BW_+OCTET_BW_-1,i*OCTET_BW_)),0};
  }

  loopMacClientData: for(auto i=0; i<Mac_Client_Data_Len_; ++i){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=MAX_MTU_ max=MAX_MTU_
    if(Gen_Rand_Mac_Client_Data_)
      Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {Crc32_.Update_Crc(i),0};
    else
      Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {Crc32_.Update_Crc(Mac_Client_Data.read()),0};
  }

#if 1==MSB_FIRST_
  loopCrcLsb: for(auto i=0;i<CRC32_N_OCTETS_;++i){
#endif
#if 1==LSB_FIRST_
  loopCrcMsb: for(auto i=CRC32_N_OCTETS_-1;i>-1;--i){
#endif
#pragma HLS UNROLL
    Gmii[Dummy_Cntr++]=Gmii_Ports<OCTET_BW_> {Crc32_.Get_Crc()(i*OCTET_BW_+OCTET_BW_-1,i*OCTET_BW_),0};
  }
}
