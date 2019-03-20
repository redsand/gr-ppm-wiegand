#ifndef INCLUDED_PPM_WIEGAND_PPM_MODULATOR_IMPL_H
#define INCLUDED_PPM_WIEGAND_PPM_MODULATOR_IMPL_H
#include <PPM_Wiegand/PPM_Modulator.h>
namespace gr {
  namespace PPM_Wiegand {
    class PPM_Modulator_impl : public PPM_Modulator
    {
     private:
      int d_nbr_samples;
      int d_inside_peak;
      float d_sample_rate;
      float d_nbr_samples_peak_duration;
      float d_command_step;
      float d_next_command;
      float d_command_spread;
      float d_command_zero;
      char d_mode = 0; 
      char d_bit_mode = 0; 
      size_t d_bits_to_send;


     public:
      PPM_Modulator_impl(float samp_rate);
      ~PPM_Modulator_impl();
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);
      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };
  } // namespace PPM_Wiegand
} // namespace gr
#endif /* INCLUDED_PPM_WIEGAND_PPM_MODULATOR_IMPL_H */

