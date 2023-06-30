#include "vxs.h"


// Top function for Vitis
void vxs
(
		ap_uint<3> hit_dt,
		ap_uint<13> energy_threshold,
		ap_uint<16> detector_threshold,
		hls::stream<fadc_hits_t> s_fadc_hits,
		hls::stream<trigger_shower_pion_det_t> s_pion_trig,
		hls::stream<trigger_shower_pion_det_t> s_shower_trig,
		hls::strean<shower_pion_det_bitmap_t>s_scint_trig,
		hls::stream<det_information_t> s_pion_info,
		hls::stream<det_information_t> s_shower_info,
		hls::stream<det_information_t> s_scint_info
)
{


}
