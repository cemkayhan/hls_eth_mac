#ifndef BIT_WIDTH_H_INCLUDE_GUARD_
#define BIT_WIDTH_H_INCLUDE_GUARD_

template<int N>                                                                                                                                                                               
struct Bit_Width                                                                                                                                                                              
{                                                                                                                                                                                             
  enum {Value = 1+Bit_Width<N/2>::Value};                                                                                                                                                     
};                                                                                                                                                                                            
                                                                                                                                                                                              
template<>                                                                                                                                                                                    
struct Bit_Width<0>                                                                                                                                                                           
{                                                                                                                                                                                             
  enum {Value = 0};                                                                                                                                                                           
};       

#endif
