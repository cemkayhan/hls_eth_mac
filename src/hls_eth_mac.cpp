#include "hls_eth_mac.h"
#include "crc32.h"

template<int MIN_MTU_,int MAX_MTU_>
static void Main_Loop(
  hls::stream<ap_axiu<8,1,1,1> >& Gmii,
  ap_uint<16> Dest_Addr_Msb,
  ap_uint<32> Dest_Addr_Lsb,
  ap_uint<16> Src_Addr_Msb,
  ap_uint<32> Src_Addr_Lsb,
  ap_uint<16> Ether_Type,
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Client_Data,
  ap_uint<1>& Packet_Done
){
  Gmii<< ap_axiu<8,1,1,1>{PREAMBLE_(6*8+7-1,6*8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{PREAMBLE_(5*8+7-1,5*8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{PREAMBLE_(4*8+7-1,4*8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{PREAMBLE_(3*8+7-1,3*8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{PREAMBLE_(2*8+7-1,2*8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{PREAMBLE_(1*8+7-1,1*8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{PREAMBLE_(0*8+7-1,0*8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{START_FRAME_DELIM_,-1,-1,0,0,0,0};

  ap_uint<32> Crc_Result=-1;
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Dest_Addr_Msb(15,8),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Dest_Addr_Msb(7,0),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Dest_Addr_Lsb(31,24),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Dest_Addr_Lsb(23,16),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Dest_Addr_Lsb(15,8),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Dest_Addr_Lsb(7,0),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Src_Addr_Msb(15,8),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Src_Addr_Msb(7,0),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Src_Addr_Lsb(31,24),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Src_Addr_Lsb(23,16),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Src_Addr_Lsb(15,8),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Src_Addr_Lsb(7,0),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Ether_Type(15,8),Crc_Result),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Ether_Type(7,0),Crc_Result),-1,-1,0,0,0,0};

  ap_axiu<8,1,1,1> Mac_Client_Data_;
  ap_uint<Bit_Width<MAX_MTU_>::Value > Data_Cntr_=0;
  ap_uint<1> Last_Data_=0;

  loopMacClientData: do{
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MAX_MTU_ max=MAX_MTU_
    Mac_Client_Data_=Mac_Client_Data.read();
    Last_Data_=Mac_Client_Data_.last;
    Gmii<< ap_axiu<8,1,1,1>{Update_Crc(Mac_Client_Data_.data,Crc_Result),-1,-1,0,0,0,0};
    ++Data_Cntr_;
  }while(!Last_Data_&&Data_Cntr_<MAX_MTU_);

  loopMacClientPad: while(Data_Cntr_<MIN_MTU_){
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MIN_MTU_ max=MIN_MTU_
    Gmii<< ap_axiu<8,1,1,1> {Update_Crc(0,Crc_Result),-1,-1,0,0,0,0};
    ++Data_Cntr_;
  }

  Gmii<< ap_axiu<8,1,1,1>{~Crc_Result(7,0),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{~Crc_Result(15,8),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{~Crc_Result(23,16),-1,-1,0,0,0,0};
  Gmii<< ap_axiu<8,1,1,1>{~Crc_Result(31,24),-1,-1,0,1,0,0};

  if(Last_Data_){
    Packet_Done=1;
  }else if(Data_Cntr_<MAX_MTU_){
    Packet_Done=0;
  }else{
    Packet_Done=1;
  }
}

void D_TOP_(
  hls::stream<ap_axiu<8,1,1,1> >& Gmii,
  ap_uint<16> Dest_Addr_Msb,
  ap_uint<32> Dest_Addr_Lsb,
  ap_uint<16> Src_Addr_Msb,
  ap_uint<32> Src_Addr_Lsb,
  ap_uint<16> Ether_Type,
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Client_Data
){
#pragma HLS INTERFACE axis port=Mac_Client_Data
#pragma HLS INTERFACE axis port=Gmii

#if 1==D_RETURN_S_AXILITE_
#pragma HLS INTERFACE s_axilite port=return bundle=Ctrl
#pragma HLS INTERFACE s_axilite port=Dest_Addr_Msb bundle=Ctrl offset=0x10
#pragma HLS INTERFACE s_axilite port=Dest_Addr_Lsb bundle=Ctrl offset=0x18
#pragma HLS INTERFACE s_axilite port=Src_Addr_Msb bundle=Ctrl offset=0x20
#pragma HLS INTERFACE s_axilite port=Src_Addr_Lsb bundle=Ctrl offset=0x28
#pragma HLS INTERFACE s_axilite port=Ether_Type bundle=Ctrl offset=0x30
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
  ap_uint<1> Packet_Done_=0;

  loopMainLoop: do{
#pragma HLS LOOP_TRIPCOUNT min=1 max=1
    Main_Loop<D_MIN_MTU_,D_MAX_MTU_>(Gmii,Dest_Addr_Msb_,Dest_Addr_Lsb_,
                                     Src_Addr_Msb_,Src_Addr_Lsb_,Ether_Type_,
                                     Mac_Client_Data,Packet_Done_);
  }while(0==Packet_Done_);
}
