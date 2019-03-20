#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <gnuradio/io_signature.h>
#include "PPM_Demodulator_impl.h"


#include "PPM_Configuration.h"
#define BIT_ZERO	0
#define BIT_ONE		1
#define READING 1
#define LISTENING 0

#define BLUE "\033[00;34m"
#define WHITE "\033[00;37m"
#define RED "\033[00;35m"

FILE *fp=NULL;

namespace gr {
  namespace PPM_Wiegand {
    PPM_Demodulator::sptr
    PPM_Demodulator::make(float samp_rate)
    {
      return gnuradio::get_initial_sptr
        (new PPM_Demodulator_impl(samp_rate));
    }

    PPM_Demodulator_impl::PPM_Demodulator_impl(float samp_rate)
      : gr::block("PPM_Demodulator",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 0, sizeof(float)))
    {
      //INIT
      fp = fopen("/home/redsand/.ids", "a+");
      code[0] = 0;
      code[1] = 0;
      bit_mode = BIT_ZERO; // start with expecting a ZERO, we can recover it otherwise
      d_nbr_peak_detected= 0;
      d_nbr_samples_since_displayed = 0;
      d_nbr_samples_since_last_peak = 0;
      d_nbr_samples_guard_time = samp_rate * GUARD_TIME;
      printf("Guard time: %.2f samples\n", d_nbr_samples_guard_time);
      d_nbr_samples_refreshing_display = samp_rate * 1.0 / 12.0 ; 
      d_nbr_samples_command_spread = COMMAND_SPREAD * samp_rate;
      d_nbr_samples_command_zero = COMMAND_ZERO * samp_rate;

      // Welcome
      d_state = LISTENING;
      printf("\n\n%sWelcome to redsand's Wiegan Key-Fob PPM Decoder!\n", RED);
    }

    int PPM_Demodulator_impl::parity(unsigned long ino, int iter) {
	while(iter > 4) {
		ino ^= ino >> iter;
		iter /= 2;
	}
	ino &= 0xf;
	return (0x6996 >> ino) & 1;
    }

    PPM_Demodulator_impl::~PPM_Demodulator_impl(){}
    void
    PPM_Demodulator_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }
    int
    PPM_Demodulator_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      unsigned long long temp;
      unsigned int id=0;
      for(int i = 0; i < noutput_items; i++){
        switch(d_state){

          case LISTENING:
            // PEAK DETECTED
            if(in[i] > 0){
              d_state = READING;
              d_nbr_samples_since_last_peak = 0;
              d_nbr_peak_detected = 1;
	      bit_mode = BIT_ZERO;
	      code[0] <<= 1;
	      code[1] <<= 1;
	      //printf("\nBitstream: \n");
            }
          break;

          case READING:
            // SECURITY OUT OF FRAME
            d_nbr_samples_since_last_peak++;
            if(d_nbr_samples_since_last_peak > d_nbr_samples_guard_time){
	      if(d_nbr_peak_detected >= 26) {
	    
              unsigned int parity_check = 0, facility = 0;
              unsigned long long mask = 0xffff;
              unsigned char high_parity, low_parity; 
              unsigned int high;
              unsigned int low;
	      switch(d_nbr_peak_detected) {
		case 26:
			temp = code[1] >> 1; // remove low parity
			id = temp & 0x0000FFFF;
			temp >>= 12;
			facility = temp & 0x000000FF;
        		mask = 0x1fff; // 13 bits
        		high_parity = (code[1] >> 25) & 1;
		        low_parity = (code[1]) & 1;
		        high = code[1] >> 12 & mask; // top 13 bits;
        		low = code[1] & mask;
			break;
		case 33:
			temp = code[1] >> 1; // remove low parity
			id = temp & 0x00FFFFFF; // 24 bits for 33
			temp >>= 24;
			facility = (unsigned int) code[1];
			facility = facility >> 25; // 24 plus one for some reason. i expeted 24
			mask = 0x1ffff;
        		high_parity = (code[1] >> 32) & 1;
			low_parity = (code[1]) & 1;
			high = code[1] >> 16; & mask; // top 17 bits;
			low = code[1] & mask;
			break;
	      }
		int parity_check_high = parity(high, 17);
		int parity_check_low = parity(low, 17);
		if(parity_check_high == high_parity && parity_check_low == low_parity) {
			parity_check = 1;
		         printf("\n[%d]: Code: %#lx Facility: %d CardId: %u %s\n", d_nbr_peak_detected, code[1], facility, id, parity_check == 0 ? "(parity failure)" : "(parity success)");
		         fprintf(fp,"[%d]: Code: %#lx Facility: %d CardId: %u %s\n", d_nbr_peak_detected, code[1], facility, id, parity_check == 0 ? "(parity failure)" : "(parity success)");
		} else if(parity_check_high == high_parity) {
		         printf("\n[%d]: Code: %#lx Facility: %d CardId: %u %s (low mismatch) high: 0x%x (%d) low: 0x%x (%d)\n", d_nbr_peak_detected, code[1], facility, id, "(parity failure)", high, high_parity, low, low_parity);
		         fprintf(fp, "\n[%d]: Code: %#lx Facility: %d CardId: %u %s (low mismatch) high: 0x%x (%d) low: 0x%x (%d)\n", d_nbr_peak_detected, code[1], facility, id, "(parity failure)", high, high_parity, low, low_parity);
		} else if(parity_check_low == low_parity) {
		         printf("\n[%d]: Code: %#lx Facility: %d CardId: %u %s (high mismatch) high: 0x%x (%d) low: 0x%x (%d)\n", d_nbr_peak_detected, code[1], facility, id, "(parity failure)", high, high_parity, low, low_parity);
		         fprintf(fp, "\n[%d]: Code: %#lx Facility: %d CardId: %u %s (high mismatch) high: 0x%x (%d) low: 0x%x (%d)\n", d_nbr_peak_detected, code[1], facility, id, "(parity failure)", high, high_parity, low, low_parity);
		} else {
		         printf("\n[%d]: Code: %#lx Facility: %d CardId: %u %s (double mismatch) high: 0x%x (%d) low: 0x%x (%d)\n", d_nbr_peak_detected, code[1], facility, id, "(parity failure)", high, high_parity, low, low_parity);
		         fprintf(fp, "\n[%d]: Code: %#lx Facility: %d CardId: %u %s (double mismatch) high: 0x%x (%d) low: 0x%x (%d)\n", d_nbr_peak_detected, code[1], facility, id, "(parity failure)", high, high_parity, low, low_parity);
		}
	      } //else
	      	 //printf("\nTimeout detected, invalid data...\n");
	      // get facility code:
	      code[0] = 0;
	      code[1] = 0;
              d_state = LISTENING;
            }

            // PEAK DETECTED
            if(in[i] > 0){
	      float peak_measure = d_nbr_samples_since_last_peak / d_nbr_samples_command_spread; 
	      // printf("Peak[%d] %.2f =  (since last %.2f - %.2f) / %.2f\n", d_nbr_peak_detected, peak_measure, d_nbr_samples_since_last_peak, d_nbr_samples_command_zero, d_nbr_samples_command_spread);

	      // if 1st and 2nd peak, then determine if we're on the short end
	      // if so, we rewrite our first bit to 0 and change our state
              if(d_nbr_peak_detected == 1) {
		// was our 1st bit
		if(peak_measure >= 0.9 && peak_measure <= 1.1) { // another 0 so we're good...
			printf("Bitstream: 0"); // initial bit is 1 for now
	        } else if (peak_measure <= 0.6 && peak_measure >= 0.4) {
			bit_mode = BIT_ONE;
			code[0] = code[1] = 0;
		        code[0] <<= 1;
		        if(code[1] & 0x80000000)
			code[0]++;
		        code[1] <<= 1;
		        code[1]++;
			printf("Bitstream: 1"); // initial bit is 1 for now
		} else
			printf("[%d] In initial bit mode but our signal is too far out of expectation: %.2f\n", d_nbr_peak_detected, peak_measure);

	      }

	      if(bit_mode == BIT_ZERO) { // one mode
		if(peak_measure >= 0.9 && peak_measure <= 1.1) {
		      bit_mode = BIT_ZERO;
		      code[0] <<= 1;
		      code[1] <<= 1;
	      	      printf("0");
		} else if(peak_measure >= 1.4 && peak_measure <= 1.6) { //
		      bit_mode = BIT_ONE;
		      code[0] <<= 1;
		      if(code[1] & 0x80000000)
			code[0]++;
		      code[1] <<= 1;
		      code[1]++;
	      	      printf("1");
		} else {
			//printf("[%d] In bit one mode but our signal is too far out of expectation, possible shifted zeros prior? %.2f steps\n", d_nbr_peak_detected, peak_measure);
			// does this mean the all the previous values are inverted? a zero perhaps
			// 
			break; // doesnt match our range, we dont log it
			
		}
	      } else { // one mode
		if(peak_measure >= 0.9 && peak_measure <= 1.1) {
		      bit_mode = BIT_ONE;
		      code[0] <<= 1;
		      if(code[1] & 0x80000000)
			code[0]++;
		      code[1] <<= 1;
		      code[1]++;
	      	      printf("1");
		} else if(peak_measure >= 0.4 && peak_measure <= 0.6) { //
		      bit_mode = BIT_ZERO;
		      code[0] <<= 1;
		      code[1] <<= 1;
	      	      printf("0");
		} else {
			// printf("[%d] In bit zero mode but our signal is too far out of expectation: %.2f\n", d_nbr_peak_detected, peak_measure);
			break; // doesnt match our range, we dont log it
		}
	      }

              d_state = READING;
              d_nbr_peak_detected++;

	      //printf("(%d) Mode: %d : %.2f (samples %.2f)\n", d_nbr_peak_detected, bit_mode, peak_measure, d_nbr_samples_since_last_peak);
              d_nbr_samples_since_last_peak = 0;
            }
          break;
        }
        
      }

	fflush(fp);
      consume_each (noutput_items);
      return noutput_items;
    }
  } /* namespace PPM_Wiegand */
} /* namespace gr */

