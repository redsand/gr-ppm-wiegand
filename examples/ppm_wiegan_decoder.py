#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Ppm Wiegan Decoder
# Generated: Wed Mar 20 16:00:58 2019
##################################################


if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import scopesink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import PPM_Wiegand
import osmosdr
import time
import wx


class ppm_wiegan_decoder(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Ppm Wiegan Decoder")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 1000000

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_scopesink2_0 = scopesink2.scope_sink_f(
        	self.GetWin(),
        	title='Scope Plot',
        	sample_rate=samp_rate,
        	v_scale=0,
        	v_offset=0,
        	t_scale=0,
        	ac_couple=False,
        	xy_mode=False,
        	num_inputs=1,
        	trig_mode=wxgui.TRIG_MODE_AUTO,
        	y_axis_label='Counts',
        )
        self.Add(self.wxgui_scopesink2_0.win)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_f(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title='FFT Plot',
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.rtlsdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + '' )
        self.rtlsdr_source_0.set_sample_rate(samp_rate)
        self.rtlsdr_source_0.set_center_freq(295000000, 0)
        self.rtlsdr_source_0.set_freq_corr(0, 0)
        self.rtlsdr_source_0.set_dc_offset_mode(1, 0)
        self.rtlsdr_source_0.set_iq_balance_mode(1, 0)
        self.rtlsdr_source_0.set_gain_mode(False, 0)
        self.rtlsdr_source_0.set_gain(50, 0)
        self.rtlsdr_source_0.set_if_gain(25, 0)
        self.rtlsdr_source_0.set_bb_gain(25, 0)
        self.rtlsdr_source_0.set_antenna('', 0)
        self.rtlsdr_source_0.set_bandwidth(100000, 0)

        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((-1, ))
        self.blocks_add_const_vxx_0 = blocks.add_const_vff((-1.02, ))
        self.analog_am_demod_cf_0 = analog.am_demod_cf(
        	channel_rate=samp_rate,
        	audio_decim=1,
        	audio_pass=28000,
        	audio_stop=30000,
        )
        self.analog_agc2_xx_0 = analog.agc2_ff(1e-1, 1e-2, 1.0, 100.0)
        self.analog_agc2_xx_0.set_max_gain(65536)
        self.PPM_Wiegand_PPM_Peak_Detector_0 = PPM_Wiegand.PPM_Peak_Detector(0)
        self.PPM_Wiegand_PPM_Demodulator_0 = PPM_Wiegand.PPM_Demodulator(samp_rate)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.PPM_Wiegand_PPM_Peak_Detector_0, 0), (self.PPM_Wiegand_PPM_Demodulator_0, 0))
        self.connect((self.analog_agc2_xx_0, 0), (self.blocks_add_const_vxx_0, 0))
        self.connect((self.analog_am_demod_cf_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.PPM_Wiegand_PPM_Peak_Detector_0, 0))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.wxgui_scopesink2_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.analog_agc2_xx_0, 0))
        self.connect((self.rtlsdr_source_0, 0), (self.analog_am_demod_cf_0, 0))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.wxgui_scopesink2_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.rtlsdr_source_0.set_sample_rate(self.samp_rate)


def main(top_block_cls=ppm_wiegan_decoder, options=None):

    tb = top_block_cls()
    tb.Start(True)
    tb.Wait()


if __name__ == '__main__':
    main()
