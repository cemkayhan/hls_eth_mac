#include "videoip.h"
#include <iostream>

int main()
{
   hls::stream<ap_axis<96,1,1,1> > insrc1;
   hls::stream<ap_axis<96,1,1,1> > outsrc1;

   ap_axis<96,1,1,1> tmp1;
   tmp1.data=23;
   tmp1.user=1;
   tmp1.last=0;
   tmp1.id=0;
   tmp1.dest=0;
   tmp1.strb=-1;
   tmp1.keep=-1;
   
   insrc1.write(tmp1);

   videoip_top(insrc1,outsrc1);

   ap_axis<96,1,1,1> tmp2 = outsrc1.read();
   std::cout << "tmp2.data: " << std::hex << tmp2.data << '\n'
             << "tmp2.user: " << std::hex << tmp2.user << '\n'
             << "tmp2.last: " << std::hex << tmp2.last << '\n'
             << "tmp2.id: "   << std::hex << tmp2.id << '\n'
             << "tmp2.dest: " << std::hex << tmp2.dest << '\n'
             << "tmp2.strb: " << std::hex << tmp2.strb << '\n'
             << "tmp2.keep: " << std::hex << tmp2.keep << '\n';
}
