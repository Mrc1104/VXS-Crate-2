#include "vxs.h"
#include "variables.h"

#include "chan_map/naive_map.h"
void generateRndData(hit_t* vxs_chan){

	std::srand(1); // set specific seed for testing latency
	ap_uint<13> energy;
	ap_uint<3> time;
	for(int ch = 0; ch < N_CHAN; ch++){
		energy = 0;
		time = 0;
		if(ch%8 < 4){ // rings 1 - 4
           	if( (rand()%100) < 5 ){ // 5% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
        else if( (ch%8 < 7) && (ch%8 > 3) ){ // rings 5a, 5b, 5c
           	if( (rand()%100) < 10 ){ // 10% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
        else if( ch%8 > 6){ // ring 6
           	if( (rand()%100) < 5 ){ // 5% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
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
	ap_uint<16> ring_threshold = 1;

	// hls::stream<T> behaves like a fifo array with infinite depth
	hls::stream<fadc_hits_t> s_fadc_hits; // raw data stream from the
	hls::stream<trigger_t> s_time_trigger; // output stream for for the trigger data
	hls::stream<ring_trigger_t> s_ring_trigger; // output stream for for the ring trigger data
	hls::stream<ring_all_t> s_ring_all_t; // output stream for the ring data
    
	fadc_hits_t fadc_hits;
	generateRndData(fadc_hits.vxs_chan);



    return 0;    
}
