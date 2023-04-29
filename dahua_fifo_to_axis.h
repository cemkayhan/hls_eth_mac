#ifndef DAHUA_FIFO_TO_AXIS_H_INCLUDE_GUARD_
#define DAHUA_FIFO_TO_AXIS_H_INCLUDE_GUARD_

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

static const unsigned FIFO_IN_SIZE = D_FIELD_BLANK_+(D_COL_+D_LINE_BLANK_)*D_ROW_;

void Dahua_Fifo_To_Axis(ap_uint<D_VID_DWIDTH_+2> Fifo_In[FIFO_IN_SIZE],
                        hls::stream<ap_axiu<D_PPC_*D_AXIS_DWIDTH_,1,1,1> >& Axis_Out,
                        ap_uint<10> Width, ap_uint<10> Height);

template<int COL,int ROW,int PPC,int VID_DWIDTH,int AXIS_DWIDTH,int FIELD_BLANK,int LINE_BLANK>
void Filter_Valid_Pix(ap_uint<VID_DWIDTH+2> Fifo_In[FIFO_IN_SIZE],
                      hls::stream<ap_uint<VID_DWIDTH> >& Fifo_Out,
                      ap_uint<10> Width, ap_uint<10> Height
) {
  ap_uint<20> Pix_Cntr=0;
  ap_uint<32> Dummy_Cntr=0;
  ap_uint<10> Width_=Width;
  ap_uint<10> Height_=Height;

  loop_wait_sof: while(0!=Fifo_In[Dummy_Cntr++].range(VID_DWIDTH+1,VID_DWIDTH)){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=FIELD_BLANK max=COL*ROW+LINE_BLANK*ROW
  };

  loop_filter_pix: for(Pix_Cntr=0; Pix_Cntr<(Width_/PPC)*Height_*PPC;){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=COL*ROW+LINE_BLANK*ROW max=COL*ROW+LINE_BLANK*ROW
    ap_uint<D_VID_DWIDTH_+2> Fifo_In_Pix=Fifo_In[Dummy_Cntr];
    if(3==Fifo_In_Pix.range(VID_DWIDTH+1,VID_DWIDTH)){
      Fifo_Out.write(Fifo_In_Pix.range(VID_DWIDTH-1,0));
      ++Pix_Cntr;
    }
    ++Dummy_Cntr;
  }
}

template<int COL,int ROW,int PPC,int VID_DWIDTH,int AXIS_DWIDTH>
void Drive_Axis_Out(hls::stream<ap_uint<VID_DWIDTH> >& Valid_Pix,
                    hls::stream<ap_axiu<PPC*AXIS_DWIDTH,1,1,1> >& Axis_Out,
                    ap_uint<10> Width, ap_uint<10> Height
) { 
  ap_uint<10> Width_=Width;
  ap_uint<10> Height_=Height;

  loop_row: for(ap_uint<10> j=0;j<Height_;++j){
#pragma HLS LOOP_TRIPCOUNT min=ROW max=ROW
    loop_col: for(ap_uint<10> k=0;k<(Width_/PPC);++k){
#pragma HLS PIPELINE II=PPC
#pragma HLS LOOP_TRIPCOUNT min=COL/PPC max=COL/PPC
      ap_axiu<PPC*AXIS_DWIDTH,1,1,1> Pix;
      loop_pix: for(int i=0;i<PPC;++i){
#pragma HLS UNROLL
        Pix.data.range(i*VID_DWIDTH+VID_DWIDTH-1,i*VID_DWIDTH)=Valid_Pix.read();
      }
      Pix.keep=-1;
      Pix.strb=-1;
      if(0==j&&0==k){
        Pix.user=1;
      } else {
        Pix.user=0;
      }
      if((Width_/PPC-1)==k){
        Pix.last=1;
      } else {
        Pix.last=0;
      }
      Axis_Out.write(Pix);
    }
  }
}

#endif
