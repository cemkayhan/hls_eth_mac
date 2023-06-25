#include "hls_tpg_ctrl.h"

int main()
{
  ap_uint<32> Tpg_Ctrl[1000];
  for(auto i=0;i<200;++i){
    Hls_Tpg_Ctrl(Tpg_Ctrl);
  }
}
