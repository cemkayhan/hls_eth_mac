#if !defined(HLS_ETH_MAC_H_INCLUDE_GUARD_)
#define HLS_ETH_MAC_H_INCLUDE_GUARD_

#include "bit_width.h"
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "power2.h"
#include "crc32.h"
#include <array>

static const auto PREAMBLE_=ap_uint<56> {0x55555555555555};
static const auto START_FRAME_DELIM_=ap_uint<8> {0xD5};
static const auto MTU_1500_=ap_uint<16> {1500};

template<int MIN_MTU_,int MAX_MTU_>
static void Fill_Bufs(
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Data,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_MTU_-1>::Value>::Value>& Buf1,
  ap_uint<16>& Buf1_Len,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_MTU_-1>::Value>::Value>& Buf2,
  ap_uint<16>& Buf2_Len,
  ap_uint<16> Mtu
){
  ap_uint<16> Buf1_Len_=0;
  ap_axiu<8,1,1,1> Buf1_Data;
  loopBuf1: do{
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MAX_MTU_ max=MAX_MTU_
    Mac_Data>>Buf1_Data;
    Buf1[Buf1_Len_++]=Buf1_Data.data;
  }while(!Buf1_Data.last&&Mtu>Buf1_Len_);

  loopBuf1Pad: while(MIN_MTU_>Buf1_Len_){
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MIN_MTU_ max=MIN_MTU_
    Buf1[Buf1_Len_++]=0;
  };
  Buf1_Len=Buf1_Len_;

  ap_uint<16> Buf2_Len_=0;
  ap_axiu<8,1,1,1> Buf2_Data;
  loopBuf2: do{
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MAX_MTU_ max=MAX_MTU_
    Mac_Data>>Buf2_Data;
    Buf2[Buf2_Len_++]=Buf2_Data.data;
  }while(!Buf2_Data.last&&Mtu>Buf2_Len_);

  loopBuf2Pad: while(MIN_MTU_>Buf2_Len_){
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MIN_MTU_ max=MIN_MTU_
    Buf2[Buf2_Len_++]=0;
  };
  Buf2_Len=Buf2_Len_;
}

template<int MIN_MTU_,int MAX_MTU_>
static void Packetize_Buf(
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_MTU_-1>::Value>::Value>& Buf,
  ap_uint<16>& Buf_Len,
  hls::stream<ap_uint<10> >& Gmii,
  ap_uint<16> Dest_Addr_Msb,
  ap_uint<32> Dest_Addr_Lsb,
  ap_uint<16> Src_Addr_Msb,
  ap_uint<32> Src_Addr_Lsb,
  ap_uint<16> Ether_Type,
  ap_uint<8> Slot_Time
){
#pragma HLS INLINE

  ap_uint<32> Crc_Result=-1;

  loopAll: for(ap_uint<16> I=0;I<22+Buf_Len+4+Slot_Time;++I){
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MAX_MTU_ max=MAX_MTU_

    if(21+4+Buf_Len<I) Gmii<< 0;

    else if((25+Buf_Len)==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, ap_uint<8> {~Crc_Result(31,24)})};
    else if((24+Buf_Len)==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, ap_uint<8> {~Crc_Result(23,16)})};
    else if((23+Buf_Len)==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, ap_uint<8> {~Crc_Result(15,8)})};
    else if((22+Buf_Len)==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, ap_uint<8> {~Crc_Result(7,0)})};

    else if(21<I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Buf[I-22],Crc_Result))};

    else if(21==I){
      if(MTU_1500_<Ether_Type)
        Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Ether_Type(7,0),Crc_Result))};
      else
        Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Buf_Len(7,0),Crc_Result))};
    }
    else if(20==I){
      if(MTU_1500_<Ether_Type)
        Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Ether_Type(15,8),Crc_Result))};
      else
        Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Buf_Len(15,8),Crc_Result))};
    }

    else if(19==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Src_Addr_Lsb(7,0),Crc_Result))};
    else if(18==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Src_Addr_Lsb(15,8),Crc_Result))};
    else if(17==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Src_Addr_Lsb(23,16),Crc_Result))};
    else if(16==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Src_Addr_Lsb(31,24),Crc_Result))};
    else if(15==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Src_Addr_Msb(7,0),Crc_Result))};
    else if(14==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Src_Addr_Msb(15,8),Crc_Result))};
    else if(13==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Dest_Addr_Lsb(7,0),Crc_Result))};
    else if(12==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Dest_Addr_Lsb(15,8),Crc_Result))};
    else if(11==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Dest_Addr_Lsb(23,16),Crc_Result))};
    else if(10==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Dest_Addr_Lsb(31,24),Crc_Result))};
    else if( 9==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Dest_Addr_Msb(7,0),Crc_Result))};
    else if( 8==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, Update_Crc(Dest_Addr_Msb(15,8),Crc_Result))};
    else if( 7==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, START_FRAME_DELIM_)};
    else if( 6==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, PREAMBLE_(0*8+7,0*8))};
    else if( 5==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, PREAMBLE_(1*8+7,1*8))};
    else if( 4==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, PREAMBLE_(2*8+7,2*8))};
    else if( 3==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, PREAMBLE_(3*8+7,3*8))};
    else if( 2==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, PREAMBLE_(4*8+7,4*8))};
    else if( 1==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, PREAMBLE_(5*8+7,5*8))};
    else if( 0==I) Gmii<< ap_uint<10> {(ap_uint<1> {1}, ap_uint<1> {0}, PREAMBLE_(6*8+7,6*8))};
  }
}

template<int MIN_MTU_,int MAX_MTU_>
static void Packetize_Bufs(
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_MTU_-1>::Value>::Value>& Buf1,
  ap_uint<16>& Buf1_Len,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_MTU_-1>::Value>::Value>& Buf2,
  ap_uint<16>& Buf2_Len,
  hls::stream<ap_uint<10> >& Gmii,
  ap_uint<16> Dest_Addr_Msb,
  ap_uint<32> Dest_Addr_Lsb,
  ap_uint<16> Src_Addr_Msb,
  ap_uint<32> Src_Addr_Lsb,
  ap_uint<16> Ether_Type,
  ap_uint<8> Slot_Time
){
  Packetize_Buf<MIN_MTU_,MAX_MTU_>(
    Buf1,Buf1_Len,
    Gmii,
    Dest_Addr_Msb,Dest_Addr_Lsb,
    Src_Addr_Msb,Src_Addr_Lsb,
    Ether_Type,Slot_Time
  );
  Packetize_Buf<MIN_MTU_,MAX_MTU_>(
    Buf2,Buf2_Len,
    Gmii,
    Dest_Addr_Msb,Dest_Addr_Lsb,
    Src_Addr_Msb,Src_Addr_Lsb,
    Ether_Type,Slot_Time
  );
}

void D_TOP_(
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Data,
  hls::stream<ap_uint<10> >& Gmii,
  ap_uint<16> Dest_Addr_Msb,
  ap_uint<32> Dest_Addr_Lsb,
  ap_uint<16> Src_Addr_Msb,
  ap_uint<32> Src_Addr_Lsb,
  ap_uint<16> Ether_Type,
  ap_uint<16> Mtu,
  ap_uint<8> Slot_Time
);

#endif
