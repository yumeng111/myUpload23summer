#ifndef YURIYCLCTPATTERNGEN_H_
#define YURIYCLCTPATTERNGEN_H_

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <vector>
#include <limits>       // std::numeric_limits
#include "CSCConstants.h"

namespace emu { namespace pc {

class Yuriy_CLCT_PatternGen
{ 
public:
  struct CLCT{
    unsigned int bx;
    unsigned int keystrip;
    unsigned int localhs;
    unsigned int dcfeb;
    unsigned int pattern;
    unsigned int nhits;

  };

  static int layerorder_all[6][6];
  template <size_t XS_per_CFEB>
  int  GetOutputXStrip(int cfeb, int input_xstrip, int tmb_compile_type);
  template <size_t XS_per_CFEB>
  int  GetInputXStrip(int output_xstrip, int tmb_compile_type);
  template <size_t XS_per_CFEB>
  int  GetInputCFEBByX(int output_xstrip, int tmb_compile_type);
  bool hits_Generation(int strip, int pattern, int Nhits, unsigned int hits[CSCConstants::NUM_DCFEBS]);
  void triad_Generation( unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], int strip, int pattern, int Nhits , int tmb_compile_type);
  void dumptriads(unsigned int triads[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS]);
  void finalizetriads(unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], unsigned int (&triads_final)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS]);
  void writetriads(unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], std::fstream* oss, int bx);
  void fillonebx(std::fstream* oss,unsigned int bx);
  void reversion(unsigned int &num, unsigned int nbit);
  void triadreversion(unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], unsigned int nbit);
  void Yuriy_CLCT_PatternGenerator(char * textfile, char * patfiledir);




};
}}
#endif