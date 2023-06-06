#include "hls_eth_mac.h"

int main()
{
  ap_uint<8> Gmii;
  hls::stream<ap_axiu<8,1,1,1> > Mac_Client_Data;
  ap_uint<11> Mac_Client_Data_Len;
  ap_uint<1> Gen_Rand_Mac_Client_Data;
  ap_uint<DEST_ADDR_MSB_BW_> Dest_Addr_Msb;
  ap_uint<DEST_ADDR_LSB_BW_> Dest_Addr_Lsb;
  ap_uint<SRC_ADDR_MSB_BW_> Src_Addr_Msb;
  ap_uint<SRC_ADDR_LSB_BW_> Src_Addr_Lsb;
  ap_uint<ETHER_TYPE_BW_> Ether_Type;

  Dest_Addr_Msb=0x1122;
  Dest_Addr_Lsb=0x33445566;
  Src_Addr_Msb=0xAABB;
  Src_Addr_Lsb=0xCCDDEEFF;
  Ether_Type=0x8123;
  Mac_Client_Data_Len=46;
  for(auto i=0;i<Mac_Client_Data_Len-1;++i){
    ap_axiu<8,1,1,1> Mac_Client_Data_;
    Mac_Client_Data.write(Mac_Client_Data_);
  }
  ap_axiu<8,1,1,1> Mac_Client_Data_;
  Mac_Client_Data_.last=1;
  Mac_Client_Data.write(Mac_Client_Data_);

  Hls_Eth_Mac(&Gmii,Dest_Addr_Msb,Dest_Addr_Lsb,Src_Addr_Msb,Src_Addr_Lsb,
    Ether_Type,Mac_Client_Data);

  //auto Packet_Size=Mac_Client_Data_Len+PREAMBLE_N_OCTETS_+SFD_N_OCTETS_+
  //  DEST_ADDR_N_OCTETS_+SRC_ADDR_N_OCTETS_+ETHER_TYPE_N_OCTETS_+CRC32_N_OCTETS_;
  //for(auto i=0;i<Packet_Size;++i){
  //  std::cout << "[" << i << "], " << Gmii[i] << '\n';
  //}
}
