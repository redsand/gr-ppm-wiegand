#ifndef INCLUDED_PPM_WIEGAND_PPM_PEAK_DETECTOR_IMPL_H
#define INCLUDED_PPM_WIEGAND_PPM_PEAK_DETECTOR_IMPL_H
#include <PPM_Wiegand/PPM_Peak_Detector.h>
namespace gr {
  namespace PPM_Wiegand {
    class PPM_Peak_Detector_impl : public PPM_Peak_Detector
    {
     private:
      float d_peak_threshold;
      time_t monitor_period;
      float d_peak_max;
      
     public:
      PPM_Peak_Detector_impl(float peak_threshold);
      ~PPM_Peak_Detector_impl();
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };
  } // namespace PPM_Wiegand
} // namespace gr
#endif /* INCLUDED_PPM_WIEGAND_PPM_PEAK_DETECTOR_IMPL_H */

