#include "vxs.h"
#include <iostream>
		using std::cout; using std::endl;

// Top function for Vitis
void vxs
(
		ap_uint<3> hit_dt,
		ap_uint<13> energy_threshold,
		ap_uint<16> detector_threshold,
		const chan_map arr_chan_map[][16],
		hls::stream<fadc_hits_t> &s_fadc_hits,
		hls::stream<trigger_shower_pion_det_t> &s_pion_trig,
		hls::stream<trigger_shower_pion_det_t> &s_shower_trig,
		hls::stream<shower_pion_det_bitmap_t> &s_scint_trig,
		hls::stream<det_information_t> &s_pion_info,
		hls::stream<det_information_t> &s_shower_info,
		hls::stream<det_information_t> &s_scint_info
)
{
	fadc_hits_t fadc_hits = s_fadc_hits.read();
	for(int ch = 0; ch < N_CHAN; ch++){
		if(!fadc_hits.vxs_chan[ch].e>=energy_threshold){continue;}
		int fadc_channel = ch%16; // channel # inside fadc ( [0 , 15] )
		int slot = (ch - fadc_channel)/16; // slot # 9starts at 0)
		/* Get Channel to Detector Mapping */
		int det_id = arr_chan_map[slot][fadc_channel].DET_ID;
		int seg_num = arr_chan_map[slot][fadc_channel].SEG_NUM;
		int sub_element = arr_chan_map[slot][fadc_channel].SUB_ELEMENT;
		cout << "det_id: " << det_id << endl;
		cout << "seg_num: " << seg_num << endl;
		cout << "sub_element: " << sub_element << endl;
		cout << " ________________ " << endl;
	}

}
