#include "hls_tpg_ctrl.h"

void Hls_Tpg_Ctrl(
  ap_uint<32> Tpg_Ctrl[1000]
){
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=Tpg_Ctrl offset=off

  static unsigned State=0;
#pragma HLS RESET variable=State

  switch(State){
    case 0:
      State=1;
      break;
    case 1:
      Tpg_Ctrl[0x10/4]=640;
      State=2;
      break;
    case 2:
      Tpg_Ctrl[0x18/4]=512;
      State=3;
      break;
    case 3:
      Tpg_Ctrl[0x0/4]=0x81;
      State=4;
      break;
    default:
      State=4;
      break;
  }
}
  
