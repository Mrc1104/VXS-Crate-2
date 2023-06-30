#include "vxs.h"
#include <iostream>
		using std::cout; using std::endl;

// Top function for Vitis
void vxs
(
		ap_uint<3> hit_dt,
		ap_uint<13> energy_threshold,
		ap_uint<16> detector_threshold,
		hls::stream<fadc_hits_t> &s_fadc_hits,
		hls::stream<trigger_array_t> &s_det_timing,
		hls::stream<shower_pion_det_bitmap_t> &s_pion_bitmap,
		hls::stream<shower_pion_det_bitmap_t> &s_shower_bitmap,
		hls::stream<scint_det_bitmap_t> &s_scint_bitmap,
		hls::stream<det_information_t> &s_pion_info,
		hls::stream<det_information_t> &s_shower_info,
		hls::stream<det_information_t> &s_scint_info
)
{
	fadc_hits_t fadc_hits = s_fadc_hits.read();

	// initilize data elements
	trigger_array_t arr_trig_bitmap = {0,0,0};
	shower_pion_det_bitmap_t shower_bitmap = {0,0};
	shower_pion_det_bitmap_t pion_bitmap = {0,0};
	scint_det_bitmap_t scit_bitmap = {0,0,};

	for(int ch = 0; ch < N_CHAN; ch++){
		if(!fadc_hits.vxs_chan[ch].e>=energy_threshold){continue;}
		int fadc_channel = ch%16; // channel # inside fadc ( [0 , 15] )
		int slot = (ch - fadc_channel)/16; // slot # 9starts at 0)
		/* Get Channel to Detector Mapping */
		int det_id = chmap[slot][fadc_channel].DET_ID;
		int seg_num = chmap[slot][fadc_channel].SEG_NUM;
		int sub_element = chmap[slot][fadc_channel].SUB_ELEMENT;

		/*
		//Debuging block
		cout << "Channel: " << ch << endl;
		cout << "Energy: " << fadc_hits.vxs_chan[ch].e << endl;
		cout << "det_id: " << det_id << endl;
		cout << "seg_num: " << seg_num << endl;
		cout << "sub_element: " << sub_element << endl;
		cout << " ________________ " << endl;
		*/
		if(det_id == NONE){continue;}
		if( det_id == TRIG_SCINT )
		{
			ap_uint<3> scint1_time = fadc_hits.vxs_chan[ch].t;
			ap_uint<3> scint2_time = fadc_hits.vxs_chan[sub_element].t;
			if( (scint_coincidence(scint1_time,scint2_time, hit_dt) ) && (ch < sub_element))
			{
				// (bool statement # 1) == check to see if pair timing satisfies coincidence tolerance
				// (bool statement # 2) == check to make sure we do not double count
				if(scint1_time > scint2_time) // remember, the timing data reported by the fadc is flipped. See description of make_timing_bitmap()
				{
					// the shortest time is out designated "true" time ; scint1_time is shorter than scint2
					make_timing_bitmap(scint1_time, &arr_trig_bitmap.trig_array[det_id-7]);
				}
				else
				{
					// the shortest time is out designated "true" time ; scint2_time is shorter than scint1
					make_timing_bitmap(scint2_time, &arr_trig_bitmap.trig_array[det_id-7]);
				}
			}
		}
		else
		{
			make_timing_bitmap(fadc_hits.vxs_chan[ch].t, &arr_trig_bitmap.trig_array[det_id-7]);
		}

	} // end for-loop




}

void make_timing_bitmap(const ap_uint<3> fadc_time, trigger_t *ptr_timing)
{

	ap_uint<4> t_buff=0;
	if(fadc_time >=4)
		t_buff = fadc_time; // map pre time 4 to 7 -> 4 to 7 (unchanged)
	else if(fadc_time < 4)
		t_buff = fadc_time + 8; // map cur time 0 to 3 -> 8 to 11 (move to time after pre hit window)
	ap_uint<3> t_actual = t_buff - 4;

	ptr_timing->trig[t_actual] = 1;
}

ap_uint<1> scint_coincidence(const ap_uint<3> scint1, const ap_uint<3> scint2, ap_uint<3> hit_dt)
{
	ap_uint<3> diff = (scint1 < scint2) ? (scint2 - scint1) : (scint1 - scint2);
	return (diff <= hit_dt) ? 1 : 0;

}
