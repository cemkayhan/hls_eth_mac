#ifndef ECON_FIFO_TO_AXIS_H_INCLUDE_GUARD_
#define ECON_FIFO_TO_AXIS_H_INCLUDE_GUARD_

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "bit_width.h"

static const unsigned FIFO_IN_SIZE = D_FIELD_BLANK_+(D_COL_+D_LINE_BLANK_)*D_ROW_;
static const unsigned COLS_BW = Bit_Width<D_COL_>::Value;
static const unsigned ROWS_BW = Bit_Width<D_ROW_>::Value;

void Econ_Fifo_To_Axis(ap_uint<D_VID_DWIDTH_+2> Fifo_In[FIFO_IN_SIZE],
                        hls::stream<ap_axiu<D_PPC_*D_AXIS_DWIDTH_,1,1,1> >& Axis_Out,
                        ap_uint<COLS_BW> Cols, ap_uint<ROWS_BW> Rows);

template<int COL,int ROW,int PPC,int VID_DWIDTH,int AXIS_DWIDTH,int FIELD_BLANK,int LINE_BLANK>
void Filter_Valid_Pix(ap_uint<VID_DWIDTH+2> Fifo_In[FIFO_IN_SIZE],
                      hls::stream<ap_uint<VID_DWIDTH> >& Fifo_Out,
                      ap_uint<COLS_BW> Cols, ap_uint<ROWS_BW> Rows
) {
  ap_uint<COLS_BW+ROWS_BW> Pix_Cntr=0;
  ap_uint<32> Dummy_Cntr=0;
  ap_uint<COLS_BW> Cols_=Cols;
  ap_uint<ROWS_BW> Rows_=Rows;

  loop_wait_sof: while(0!=Fifo_In[Dummy_Cntr++].range(VID_DWIDTH+1,VID_DWIDTH)){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=FIELD_BLANK max=COL*ROW+LINE_BLANK*ROW
  };

  loop_filter_pix: for(Pix_Cntr=0; Pix_Cntr<(Cols_/PPC)*Rows_;){
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
                    ap_uint<COLS_BW> Cols, ap_uint<ROWS_BW> Rows
) {
  ap_uint<COLS_BW> Cols_=Cols;
  ap_uint<ROWS_BW> Rows_=Rows;

  loop_row: for(ap_uint<ROWS_BW> j=0;j<Rows_;++j){
#pragma HLS LOOP_TRIPCOUNT min=ROW max=ROW
    loop_col: for(ap_uint<COLS_BW> k=0;k<(Cols_/PPC);++k){
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
      if((Cols_/PPC-1)==k){
        Pix.last=1;
      } else {
        Pix.last=0;
      }
      Axis_Out.write(Pix);
    }
  }
}

#endif
