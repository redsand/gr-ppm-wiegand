/* -*- c++ -*- */

#define PPM_WIEGAND_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "PPM_Wiegand_swig_doc.i"

%{
#include "PPM_Wiegand/PPM_Peak_Detector.h"
#include "PPM_Wiegand/PPM_Demodulator.h"
#include "PPM_Wiegand/PPM_Signal_Detector.h"
#include "PPM_Wiegand/PPM_Modulator.h"
%}


%include "PPM_Wiegand/PPM_Peak_Detector.h"
GR_SWIG_BLOCK_MAGIC2(PPM_Wiegand, PPM_Peak_Detector);

%include "PPM_Wiegand/PPM_Demodulator.h"
GR_SWIG_BLOCK_MAGIC2(PPM_Wiegand, PPM_Demodulator);
%include "PPM_Wiegand/PPM_Signal_Detector.h"
GR_SWIG_BLOCK_MAGIC2(PPM_Wiegand, PPM_Signal_Detector);

%include "PPM_Wiegand/PPM_Modulator.h"
GR_SWIG_BLOCK_MAGIC2(PPM_Wiegand, PPM_Modulator);
