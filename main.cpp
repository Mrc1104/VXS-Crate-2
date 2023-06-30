#include <iostream>

#include "vxs.h"
#include "variables.h"

void generateRndData(hit_t* vxs_chan){

	std::srand(20); // set specific seed for testing latency
	ap_uint<13> energy;
	ap_uint<3> time;
	for(int ch = 0; ch < N_CHAN; ch++){
		energy = 0;
		time = 0;
//		if(ch%8 < 4){ // rings 1 - 4
//           	if( (rand()%100) < 5 ){ // 5% chance
//               	energy = rand() % 8192;
//               	time = rand() % 8;
//           	}
//        }
//        else if( (ch%8 < 7) && (ch%8 > 3) ){ // rings 5a, 5b, 5c
//           	if( (rand()%100) < 10 ){ // 10% chance
//               	energy = rand() % 8192;
//               	time = rand() % 8;
//           	}
//        }
//        else if( ch%8 > 6){ // ring 6
//           	if( (rand()%100) < 5 ){ // 5% chance
//               	energy = rand() % 8192;
//               	time = rand() % 8;
//           	}
//        }
		if( ((rand()%100) + 1) < 51){ // 50% chance
			               	energy = rand() % 8192;
			               	time = rand() % 8;
		}
		vxs_chan[ch].e = energy;
		vxs_chan[ch].t = energy;
	}
}

int main()
{
	ap_uint<3> hit_dt = COINCIDENCE_TOLERANCE; // time tolerance for coincidence (in 4ns ticks)
	// ap_uint<13> energy_threshold = 5e3;
	ap_uint<13> energy_threshold = 1;
	ap_uint<16> detector_threshold = 1;
	// hls::stream<T> behaves like a fifo array with infinite depth
	hls::stream<fadc_hits_t> s_fadc_hits; // raw data stream from the
	hls::stream<trigger_array_t> s_det_timing;
	hls::stream<shower_pion_det_bitmap_t> s_pion_bitmap;
	hls::stream<shower_pion_det_bitmap_t> s_shower_bitmap;
	hls::stream<scint_det_bitmap_t> s_scint_bitmap;
	hls::stream<det_information_t> s_pion_info;
	hls::stream<det_information_t> s_shower_info;
	hls::stream<det_information_t> s_scint_info;


	fadc_hits_t fadc_hits;
	generateRndData(fadc_hits.vxs_chan);
	s_fadc_hits.write(fadc_hits);
	std::cout << "NOT HERE" << std::endl;
	while(!s_fadc_hits.empty()){
		std::cout << "HERE" << std::endl;
		vxs
		(
				hit_dt,
				energy_threshold,
				detector_threshold,
				s_fadc_hits,
				s_det_timing,
				s_pion_bitmap,
				s_shower_bitmap,
				s_scint_bitmap,
				s_pion_info,
				s_shower_info,
				s_scint_info
		);
	}

    return 0;    
}
