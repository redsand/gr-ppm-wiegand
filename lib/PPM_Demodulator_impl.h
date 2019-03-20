#ifndef INCLUDED_PPM_WIEGAND_PPM_DEMODULATOR_IMPL_H
#define INCLUDED_PPM_WIEGAND_PPM_DEMODULATOR_IMPL_H
#include <PPM_Wiegand/PPM_Demodulator.h>

#include "PPM_Configuration.h"

namespace gr {
  namespace PPM_Wiegand {
    class PPM_Demodulator_impl : public PPM_Demodulator
    {
     private:
      int d_state;
      int d_nbr_peak_detected;
      int bit_mode;
      unsigned long code[2];
      float d_nbr_samples_since_last_peak;
      float d_nbr_samples_guard_time;
      float d_nbr_samples_since_displayed;
      float d_nbr_samples_refreshing_display;
      float d_nbr_samples_command_spread;
      float d_nbr_samples_command_zero;

     public:
      PPM_Demodulator_impl(float samp_rate);
      ~PPM_Demodulator_impl();
      int parity(unsigned long ino, int iter);
      int getBitOffset (unsigned v);
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);
      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };
  } // namespace PPM_Wiegand
} // namespace gr
#endif /* INCLUDED_PPM_WIEGAND_PPM_DEMODULATOR_IMPL_H */

