#include "hls_file1.h"

int main()
{
  ap_uint<32> Ctrl[16];
  for(auto i=0;i<16;++i){
    Hls_File1(Ctrl);
  }
}
