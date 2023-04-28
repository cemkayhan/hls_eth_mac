#include "dahua_fifo_to_axis.h"

#if defined(D_VERBOSE_)
#include <fstream>
#endif

void Fill_Frame(ap_uint<18> Fifo_In[FIFO_IN_SIZE],
int Col,int Row,int Field_Blank,int Line_Blank)
{
  int Pix_Cntr=0;
  for(int i=0;i<Field_Blank;++i){
    Fifo_In[Pix_Cntr++]=0;
  }

  for(int i=0;i<Row;++i){
    for(int j=0;j<Col;++j){
      Fifo_In[Pix_Cntr].range(15,0)=i+j%4;
      Fifo_In[Pix_Cntr].range(17,16)=3;
      ++Pix_Cntr;
    }
    for(int k=0;k<Line_Blank;++k){
      Fifo_In[Pix_Cntr].range(15,0)=0;
      Fifo_In[Pix_Cntr].range(17,16)=1;
      ++Pix_Cntr;
    }
  }
}

void Drain_Frame(hls::stream<ap_axiu<96,1,1,1> >& Axis_Out,int Col,int Row,int Ppc)
{
  for(int i=0;i<Row;++i){
    for(int j=0;j<Col/Ppc;++j){
#if defined(D_VERBOSE_)
      static std::ofstream ofs {"log.txt"};

      ap_uint<96>Tmp_Pix=Axis_Out.read().data;
      ofs << "Pix[" << i << "][" << j*Ppc+3 << "]: "
          << Tmp_Pix.range(63,48) << ", "
          << "Pix[" << i << "][" << j*Ppc+2 << "]: "
          << Tmp_Pix.range(47,32) << ", "
          << "Pix[" << i << "][" << j*Ppc+1 << "]: "
          << Tmp_Pix.range(31,16) << ", "
          << "Pix[" << i << "][" << j*Ppc+0 << "]: "
          << Tmp_Pix.range(15,0) << '\n';
#else
      Axis_Out.read();
#endif
    }
  }
}

int main()
{
  ap_uint<18>* Fifo_In=new ap_uint<18>[FIFO_IN_SIZE];
  hls::stream<ap_axiu<96,1,1,1> > Axis_Out;
  ap_uint<10> Width=D_COL_;
  ap_uint<10> Height=D_ROW_;

  Fill_Frame(Fifo_In,D_COL_,D_ROW_,D_FIELD_BLANK_,D_LINE_BLANK_);
  Dahua_Fifo_To_Axis(Fifo_In,Axis_Out,Width+1,Height);
  Drain_Frame(Axis_Out,D_COL_,D_ROW_,D_PPC_);

  Fill_Frame(Fifo_In,D_COL_,D_ROW_,D_FIELD_BLANK_,D_LINE_BLANK_);
  Dahua_Fifo_To_Axis(Fifo_In,Axis_Out,Width+1,Height);
  Drain_Frame(Axis_Out,D_COL_,D_ROW_,D_PPC_);

  Fill_Frame(Fifo_In,D_COL_,D_ROW_,D_FIELD_BLANK_,D_LINE_BLANK_);
  Dahua_Fifo_To_Axis(Fifo_In,Axis_Out,Width+1,Height);
  Drain_Frame(Axis_Out,D_COL_,D_ROW_,D_PPC_);

  Fill_Frame(Fifo_In,D_COL_,D_ROW_,D_FIELD_BLANK_,D_LINE_BLANK_);
  Dahua_Fifo_To_Axis(Fifo_In,Axis_Out,Width+1,Height);
  Drain_Frame(Axis_Out,D_COL_,D_ROW_,D_PPC_);
}
