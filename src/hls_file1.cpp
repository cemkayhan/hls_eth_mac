#include "hls_file1.h"

void D_TOP_(
  ap_uint<32> Ctrl[16]
){
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=Ctrl offset=off

  static unsigned State=0;
#pragma HLS RESET variable=State

  switch(State){
    case 0:
      State=1;
      break;
    case 1:
      Ctrl[0x0/4]=0x81;
      State=2;
      break;
    default:
      State=2;
      break;
  }
}
  
