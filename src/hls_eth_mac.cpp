#include "hls_eth_mac.h"

void D_TOP_(
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Data,
  hls::stream<ap_uint<8> >& Gmii,
  ap_uint<16> Dest_Addr_Msb,
  ap_uint<32> Dest_Addr_Lsb,
  ap_uint<16> Src_Addr_Msb,
  ap_uint<32> Src_Addr_Lsb,
  ap_uint<16> Ether_Type
){
#pragma HLS INTERFACE axis port=Mac_Data
#pragma HLS INTERFACE ap_fifo port=Gmii

#if 1==D_RETURN_S_AXILITE_
#pragma HLS INTERFACE s_axilite port=return bundle=Ctrl
#pragma HLS INTERFACE s_axilite port=Dest_Addr_Msb bundle=Ctrl offset=0x10
#pragma HLS INTERFACE s_axilite port=Dest_Addr_Lsb bundle=Ctrl offset=0x18
#pragma HLS INTERFACE s_axilite port=Src_Addr_Msb bundle=Ctrl offset=0x20
#pragma HLS INTERFACE s_axilite port=Src_Addr_Lsb bundle=Ctrl offset=0x28
#pragma HLS INTERFACE s_axilite port=Ether_Type bundle=Ctrl offset=0x30
#pragma HLS STABLE variable=Dest_Addr_Msb,Dest_Addr_Lsb
#pragma HLS STABLE variable=Src_Addr_Msb,Src_Addr_Lsb
#pragma HLS STABLE variable=Ether_Type
#endif

#if 1==D_RETURN_AP_CTRL_HLS_
#pragma HLS INTERFACE ap_none port=Dest_Addr_Msb,Dest_Addr_Lsb
#pragma HLS INTERFACE ap_none port=Src_Addr_Msb,Src_Addr_Lsb
#pragma HLS INTERFACE ap_none port=Ether_Type
#pragma HLS STABLE variable=Dest_Addr_Msb,Dest_Addr_Lsb
#pragma HLS STABLE variable=Src_Addr_Msb,Src_Addr_Lsb
#pragma HLS STABLE variable=Ether_Type
#endif

  ap_uint<16> Dest_Addr_Msb_=Dest_Addr_Msb;
  ap_uint<32> Dest_Addr_Lsb_=Dest_Addr_Lsb;
  ap_uint<16> Src_Addr_Msb_=Src_Addr_Msb;
  ap_uint<32> Src_Addr_Lsb_=Src_Addr_Lsb;
  ap_uint<16> Ether_Type_=Ether_Type;

#pragma HLS DATAFLOW

  std::array<ap_uint<8>, Power2<Bit_Width<D_MAX_MTU_>::Value>::Value> Buf1;
#pragma HLS BIND_STORAGE variable=Buf1 type=RAM_T2P impl=BRAM

  std::array<ap_uint<8>, Power2<Bit_Width<D_MAX_MTU_>::Value>::Value> Buf2;
#pragma HLS BIND_STORAGE variable=Buf2 type=RAM_T2P impl=BRAM

  ap_uint<16> Buf1_Len;
  ap_uint<16> Buf2_Len;

  Fill_Bufs<D_MIN_MTU_,D_MAX_MTU_>(
    Mac_Data,
    Buf1,Buf1_Len,
    Buf2,Buf2_Len
  );
  Packetize_Bufs<D_MIN_MTU_,D_MAX_MTU_>(
    Buf1,Buf1_Len,
    Buf2,Buf2_Len,
    Gmii,
    Dest_Addr_Msb_,Dest_Addr_Lsb_,
    Src_Addr_Msb_,Src_Addr_Lsb_,
    Ether_Type_
  );
}
