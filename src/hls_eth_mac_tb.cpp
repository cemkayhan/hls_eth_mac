#include "hls_eth_mac.h"

int main()
{
  hls::stream<ap_axiu<8,1,1,1> > Mac_Data;
  ap_uint<16> Mac_Data_Len;
  hls::stream<ap_uint<8> > Gmii;
  ap_uint<16> Dest_Addr_Msb;
  ap_uint<32> Dest_Addr_Lsb;
  ap_uint<16> Src_Addr_Msb;
  ap_uint<32> Src_Addr_Lsb;
  ap_uint<16> Ether_Type;

  Dest_Addr_Msb=0x1122;
  Dest_Addr_Lsb=0x33445566;
  Src_Addr_Msb=0xAABB;
  Src_Addr_Lsb=0xCCDDEEFF;
  Ether_Type=0x8123;
  Mac_Data_Len=460;

  ap_axiu<8,1,1,1> Mac_Data_;
  Mac_Data_.last=0;
  for(auto i=0;i<Mac_Data_Len;++i){
    Mac_Data_.data=i;
    Mac_Data.write(Mac_Data_);
  }
  Mac_Data_.data=0xFF;
  Mac_Data_.last=1;
  Mac_Data.write(Mac_Data_);

  Mac_Data_.last=0;
  for(auto i=0;i<Mac_Data_Len;++i){
    Mac_Data_.data=i;
    Mac_Data.write(Mac_Data_);
  }
  Mac_Data_.data=0xFF;
  Mac_Data_.last=1;
  Mac_Data.write(Mac_Data_);

  D_TOP_(
    Mac_Data,
    Gmii,
    Dest_Addr_Msb,Dest_Addr_Lsb,
    Src_Addr_Msb,Src_Addr_Lsb,
    Ether_Type
  );

  while(!Gmii.empty()){
    Gmii.read();
  }

  //auto Packet_Size=Mac_Client_Data_Len+PREAMBLE_N_OCTETS_+SFD_N_OCTETS_+
  //  DEST_ADDR_N_OCTETS_+SRC_ADDR_N_OCTETS_+ETHER_TYPE_N_OCTETS_+CRC32_N_OCTETS_;
  //for(auto i=0;i<Packet_Size;++i){
  //  std::cout << "[" << i << "], " << Gmii[i] << '\n';
  //}
}
