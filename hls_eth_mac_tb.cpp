#include "hls_eth_mac.h"
#include <iostream>

int main()
{
  Gmii_Intf<D_MAX_MTU_,D_OCTET_BW_> Gmii;
  hls::stream<ap_uint<D_OCTET_BW_> > Mac_Client_Data;
  ap_uint<1> Gen_Rand_Mac_Client_Data;
  ap_uint<D_LENGTH_TYPE_BW_> Length_Type;
  ap_uint<D_DEST_ADDR_BW_> Dest_Addr;
  ap_uint<D_SRC_ADDR_BW_> Src_Addr;

  Length_Type=100;
  Dest_Addr = 1;
  Src_Addr = 2;
  Gen_Rand_Mac_Client_Data=1;
  if(!Gen_Rand_Mac_Client_Data){
    for(auto i=0;i<Length_Type;++i) Mac_Client_Data.write(i);
  }
  Hls_Eth_Mac(Gmii,Mac_Client_Data,Gen_Rand_Mac_Client_Data,Length_Type,Dest_Addr,Src_Addr);
  for(auto i=0;i<Length_Type+7+1+6+6+2;++i){
    std::cout << Gmii[i] << '\n';
  }
}
