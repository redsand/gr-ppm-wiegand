#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <gnuradio/io_signature.h>
#include "PPM_Modulator_impl.h"


#include "PPM_Configuration.h"
#define BLUE "\033[00;34m"
#define WHITE "\033[00;37m"
#define RED "\033[00;35m"
//#define NBR_CHANNEL 4


namespace gr {
	namespace PPM_Wiegand {
		int getch(void){
			struct termios oldattr, newattr;
			int ch;
			tcgetattr( STDIN_FILENO, &oldattr );
			newattr = oldattr;
			newattr.c_lflag &= ~( ICANON | ECHO );
			tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
			ch = getchar();
			tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
			return ch;
		}
		int kbhit(void){
			struct termios oldt, newt;
			int ch;
			int oldf;
			tcgetattr(STDIN_FILENO, &oldt);
			newt = oldt;
			newt.c_lflag &= ~(ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &newt);
			oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
			fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
			ch = getchar();
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
			fcntl(STDIN_FILENO, F_SETFL, oldf);
			if(ch != EOF){
				ungetc(ch, stdin);
				return 1;
			}
			return 0;
		}
		PPM_Modulator::sptr
		PPM_Modulator::make(float samp_rate)
		{
			return gnuradio::get_initial_sptr
			(new PPM_Modulator_impl(samp_rate));
		}
		PPM_Modulator_impl::PPM_Modulator_impl(float samp_rate)
		: gr::block("PPM_Modulator",
			gr::io_signature::make(0, 0, sizeof(float)),
			gr::io_signature::make(1, 1, sizeof(float)))
		{

			// INIT
			d_sample_rate = samp_rate;
			d_nbr_samples = 0;
			d_nbr_samples_peak_duration = PEAK_DURATION * samp_rate;
			d_command_step = COMMAND_STEP * samp_rate;
			d_command_spread = COMMAND_SPREAD * samp_rate;
			d_command_zero = COMMAND_ZERO * samp_rate;

			d_next_command = GUARD_TIME * samp_rate;

			d_mode = 0; // waiting
			d_bit_mode = 0; // bit zero mode

			d_bits_to_send = 33;

			// WELCOME
			printf("\n\n%sWelcome to redsand's Wiegan Key-Fob PPM Command System\n", RED);
			printf("Sample peak duration: %.2f\n", d_nbr_samples_peak_duration);
			printf("Command step: %.2f\n", d_command_step);
			printf("Command zero: %.2f\n", COMMAND_ZERO * samp_rate);
			printf("Command spread: %.2f\n", COMMAND_SPREAD * samp_rate);
			printf("Guard: %.2f\n", GUARD_TIME * samp_rate);
		}

		PPM_Modulator_impl::~PPM_Modulator_impl(){}
		void
		PPM_Modulator_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
		{
			ninput_items_required[0] = noutput_items;
		}
		int
		PPM_Modulator_impl::general_work (int noutput_items,
			gr_vector_int &ninput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
		{
			const float *in = (const float *) input_items[0];
			float *out = (float *) output_items[0];


			// XXX ALL MY FANCY CODE HERE (not today tho)

			return noutput_items;
	}
  } /* namespace PPM_Wiegand */
} /* namespace gr */

