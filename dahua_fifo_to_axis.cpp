#include "dahua_fifo_to_axis.h"

void Dahua_Fifo_To_Axis(ap_uint<D_VID_DWIDTH_+2> Fifo_In[FIFO_IN_SIZE],
                        hls::stream<ap_axiu<D_PPC_*D_AXIS_DWIDTH_,1,1,1> >& Axis_Out,
                        ap_uint<10> Width, ap_uint<10> Height)
{
#pragma HLS INTERFACE ap_fifo port=Fifo_In
#pragma HLS INTERFACE axis port=Axis_Out
#if defined(D_AP_CTRL_NONE_)
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE ap_stable port=Width,Height
#else
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=Height offset=0x0010
#pragma HLS INTERFACE s_axilite port=Width offset=0x0018
#endif
#pragma HLS DATAFLOW

  hls::stream<ap_uint<D_VID_DWIDTH_> > Valid_Pix;
#pragma HLS STREAM variable=Valid_Pix depth=8

#if defined(D_OVERRIDE_COL_ROW_)
  Filter_Valid_Pix<D_COL_,D_ROW_,D_PPC_,D_VID_DWIDTH_,D_AXIS_DWIDTH_,D_FIELD_BLANK_,D_LINE_BLANK_>(Fifo_In,Valid_Pix,D_COL_,D_ROW_);
  Drive_Axis_Out<D_COL_,D_ROW_,D_PPC_,D_VID_DWIDTH_,D_AXIS_DWIDTH_>(Valid_Pix,Axis_Out,D_COL_,D_ROW_);
#else
  Filter_Valid_Pix<D_COL_,D_ROW_,D_PPC_,D_VID_DWIDTH_,D_AXIS_DWIDTH_,D_FIELD_BLANK_,D_LINE_BLANK_>(Fifo_In,Valid_Pix,Width,Height);
  Drive_Axis_Out<D_COL_,D_ROW_,D_PPC_,D_VID_DWIDTH_,D_AXIS_DWIDTH_>(Valid_Pix,Axis_Out,Width,Height);
#endif
}
