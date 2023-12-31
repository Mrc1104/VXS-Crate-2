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
#ifndef __SYNTHESIS__
  // Initialize for simulation only (creates a problem for synthesis scheduling)
  static fadc_hits_t fadc_hits_pre = {0,0};
#else
  static fadc_hits_t fadc_hits_pre;
#endif


	hit_t arr_event[N_CHAN] = {0,0,0,0};
	for(int ch = 0; ch < N_CHAN; ch++){
#pragma HLS UNROLL
		arr_event[ch] = make_event(fadc_hits_pre.vxs_chan[ch], fadc_hits.vxs_chan[ch]);
	}
	// set curr fadc data to previous fadc data
	fadc_hits_pre = fadc_hits;
	// Initialize data elements
	trigger_array_t arr_trig_bitmap = {0,0,0};

	shower_pion_det_bitmap_t shower_bitmap = {0,0};
	shower_pion_det_bitmap_t pion_bitmap = {0,0};
	scint_det_bitmap_t scint_bitmap = {0,0,};

	// possible optimization, store hit data in new arrays then add later for better parallelism

	det_information_t shower_information = {0,0};
	det_information_t pion_information = {0,0};
	det_information_t scint_information = {0,0};
	for(int ch = 0; ch < N_CHAN; ch++){
		// if(arr_event[ch].e<energy_threshold){continue;}
		int fadc_channel = ch%16; // channel # inside fadc ( [0 , 15] )
		int slot = (ch - fadc_channel)/16; // slot # 9 starts at 0)
		/* Get Channel to Detector Mapping */
		int det_id = chmap[slot][fadc_channel].DET_ID;
		int seg_num = chmap[slot][fadc_channel].SEG_NUM;
		int sub_element = chmap[slot][fadc_channel].SUB_ELEMENT;
		/* Check whch detector we are dealing with
		   It is possible no detector is slotted (empty fadc channel) */
		if(det_id == NONE){continue;}
		if( det_id == TRIG_SCINT )
		{
			ap_uint<3> scint1_time = arr_event[ch].t;
			ap_uint<3> scint2_time = arr_event[sub_element].t;
			if( (scint_coincidence(scint1_time,scint2_time, hit_dt) ) && (ch < sub_element))
			{
				// (bool statement # 1) == check to see if pair timing satisfies coincidence tolerance
				// (bool statement # 2) == check to make sure we do not double count

				if(scint1_time > scint2_time) // remember, the timing data reported by the fadc is flipped. See description of make_timing_bitmap()
				{
					// the shortest time is our designated "true" time ; scint1_time is shorter than scint2
					make_timing_bitmap(scint1_time, &arr_trig_bitmap.trig_array[det_id-7]);
				}
				else
				{
					// the shortest time is our designated "true" time ; scint2_time is shorter than scint1
					make_timing_bitmap(scint2_time, &arr_trig_bitmap.trig_array[det_id-7]);
				}

				// the scint pairs are aligned front to back so they correspond to the same 4 segments (each pair covers 4 segments)
				make_scint_bitmap(scint_bitmap.segment, seg_num);
				scint_information.total_energy += arr_event[ch].e;
				scint_information.total_hits++;
			}
		}

		else if(det_id == PION_DET)
		{
			make_shower_pion_bitmap(pion_bitmap.segment, seg_num);
			pion_information.total_energy += arr_event[ch].e;
			pion_information.total_hits++;
		}
		else if(det_id == SHOWER_MAX)
		{
			make_shower_pion_bitmap(shower_bitmap.segment, seg_num);
			shower_information.total_energy += arr_event[ch].e;
			shower_information.total_hits++;
		}
		make_timing_bitmap(arr_event[ch].t, &arr_trig_bitmap.trig_array[det_id-7]);
	} // end for-loop

	// write data to output streams
	s_det_timing.write(arr_trig_bitmap);
	s_pion_bitmap.write(pion_bitmap);
	s_shower_bitmap.write(shower_bitmap);
	s_scint_bitmap.write(scint_bitmap);
	s_pion_info.write(pion_information);
	s_shower_info.write(shower_information);
	s_scint_info.write(scint_information);
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

void make_shower_pion_bitmap(ap_uint<28>& seg_bitmap, int seg_num)
{
	seg_bitmap[seg_num] = 1;
}

void make_scint_bitmap(ap_uint<7>& seg_bitmap, int seg_num)
{
	// One scintillator covers 4 segments
	// [0]-[3] -> [0]
	// [4]-[7] -> [1]
	//  ...
	// [24]-[27]->[7]

	int seg_group = seg_num % 4;
	int group = (seg_num - seg_group)/4;

	seg_bitmap[group] = 1;
}

hit_t make_event(
	hit_t pre_hit, 
	hit_t cur_hit
)
{
	hit_t tmp = {0,0};
	if(pre_hit.t >=4)
		tmp = pre_hit;
	else if(cur_hit.t < 4)
		tmp = cur_hit;
	return tmp;
}