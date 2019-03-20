#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gnuradio/io_signature.h>
#include "PPM_Peak_Detector_impl.h"
namespace gr {
  namespace PPM_Wiegand {
    PPM_Peak_Detector::sptr
    PPM_Peak_Detector::make(float peak_threshold)
    {
      return gnuradio::get_initial_sptr
        (new PPM_Peak_Detector_impl(peak_threshold));
    }

    PPM_Peak_Detector_impl::PPM_Peak_Detector_impl(float peak_threshold)
      : gr::sync_block("PPM_Peak_Detector",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
      d_peak_threshold = peak_threshold;
      set_history(1);
      monitor_period = time(NULL);
      d_peak_max = 0;
    }
    PPM_Peak_Detector_impl::~PPM_Peak_Detector_impl(){}
    int
    PPM_Peak_Detector_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      time_t now = time(NULL);

	// skip the 1st one to avoid a pointer underread
      for(int i = 1; i < noutput_items; i++){
        // RISING EDGE ABOVE THRESHOLD
	//printf("Comparing %.2f > peak (%.2f) and previous %.2f is less than %.2f\n", in[i], d_peak_threshold, in[i - 1], d_peak_threshold);
	d_peak_max = in[i] > d_peak_max ? in[i] : d_peak_max;
        if(in[i] > d_peak_threshold && in[i - 1] < d_peak_threshold) {
		out[i] = 1;
		// printf("Peak found!\n");
	} else
		out[i] = 0;      
      }

      if(now > monitor_period + (60 * 5)) {
	printf("Peak monitor: %.2f in 5 minutes\n", d_peak_max);
	d_peak_max = 0;
	monitor_period = now;
      }

      return noutput_items;
    }
  } /* namespace PPM_Wiegand */
} /* namespace gr */

