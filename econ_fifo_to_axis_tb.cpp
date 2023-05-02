#include "econ_fifo_to_axis.h"

#if defined(D_VERBOSE_)
#include <fstream>
#endif

template<int COL,int ROW,int PPC,int VID_DWIDTH,int FIELD_BLANK,int LINE_BLANK>
void Fill_Frame(ap_uint<VID_DWIDTH+2> Fifo_In[FIFO_IN_SIZE])
{
  int Pix_Cntr=0;
  for(int i=0;i<FIELD_BLANK;++i){
    Fifo_In[Pix_Cntr++]=0;
  }

  for(int i=0;i<ROW;++i){
    for(int j=0;j<(COL/PPC);++j){
      Fifo_In[Pix_Cntr].range(VID_DWIDTH-1,0)=i+j%PPC;
      Fifo_In[Pix_Cntr].range(VID_DWIDTH+1,VID_DWIDTH)=3;
      ++Pix_Cntr;
    }
    for(int k=0;k<LINE_BLANK;++k){
      Fifo_In[Pix_Cntr].range(VID_DWIDTH-1,0)=0;
      Fifo_In[Pix_Cntr].range(VID_DWIDTH+1,VID_DWIDTH)=1;
      ++Pix_Cntr;
    }
  }
}

template<int COL,int ROW,int PPC,int VID_DWIDTH,int AXIS_DWIDTH>
void Drain_Frame(hls::stream<ap_axiu<PPC*AXIS_DWIDTH,1,1,1> >& Axis_Out)
{
  for(int i=0;i<ROW;++i){
    for(int j=0;j<COL/PPC;++j){
#if defined(D_VERBOSE_)
      static std::ofstream ofs {"log.txt"};

      ap_uint<PPC*AXIS_DWIDTH>Tmp_Pix=Axis_Out.read().data;
      for(int k=0;k<PPC;++k){
        ofs << "Pix[" << i << "][" << j*PPC+PPC-k-1 << "]: "
            << Tmp_Pix.range((PPC-k)*VID_DWIDTH-1,(PPC-k-1)*VID_DWIDTH) << ", ";
      }
      ofs << '\n';
#else
      Axis_Out.read();
#endif
    }
  }
}

int main()
{
  ap_uint<D_VID_DWIDTH_+2>* Fifo_In=new ap_uint<D_VID_DWIDTH_+2>[FIFO_IN_SIZE];
  hls::stream<ap_axiu<D_PPC_*D_AXIS_DWIDTH_,1,1,1> > Axis_Out;
  ap_uint<COLS_BW> Cols=D_COL_;
  ap_uint<ROWS_BW> Rows=D_ROW_;

  for(int i=0;i<D_N_FRAMES_;++i){
    Fill_Frame<D_COL_,D_ROW_,D_PPC_,D_VID_DWIDTH_,D_FIELD_BLANK_,D_LINE_BLANK_>(Fifo_In);
  #if defined(D_INJECT_ERROR_)
    Econ_Fifo_To_Axis(Fifo_In,Axis_Out,Cols+1,Rows);
  #else
    Econ_Fifo_To_Axis(Fifo_In,Axis_Out,Cols,Rows);
  #endif
    Drain_Frame<D_COL_,D_ROW_,D_PPC_,D_VID_DWIDTH_,D_AXIS_DWIDTH_>(Axis_Out);
  }
}
