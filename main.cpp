#include <iostream>
using std::fout; using std::endl;
#include <string>
#include <fstream>
using std::string;
#include "vxs.h"
#include "variables.h"

void generateRndData(hit_t* vxs_chan){

	std::srand(1); // set specific seed for testing latency
	ap_uint<13> energy;
	ap_uint<3> time;
	for(int ch = 0; ch < N_CHAN; ch++){
		energy = 0;
		time = 0;
		if( ((rand()%100) + 1) < 20){ // 20% chance
			energy = rand() % 8192;
			time = rand() % 8;
		}
		vxs_chan[ch].e = energy;
		vxs_chan[ch].t = time;
	}
}

void make_known_data(hit_t* vxs_chan){
	
	ap_uint<13> energy;
	ap_uint<3> time;
	for(int ch = 0; ch < N_CHAN; ch++){
		energy = 0;
		time = 0;
		int fadc_channel = ch%16;	
		int slot = (ch - fadc_channel)/16;
		if(slot == 1){
			if(fadc_channel > 11){
				energy = 25;
				time = 2;
			}	
		}
		if(slot == 3){
			if(fadc_channel > 11){
				energy = 25;
				time = 2;
			}	
		}
		if(slot == 4){
			if(fadc_channel > 13){
				energy = 25;
				time = 2;
			}	
		}
		vxs_chan[ch].e = energy;
		vxs_chan[ch].t = time;
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
	// generateRndData(fadc_hits.vxs_chan);
	make_known_data(fadc_hits.vxs_chan);
	s_fadc_hits.write(fadc_hits);
	while(!s_fadc_hits.empty()){
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
	// to make output prettier
	string border = "_________________________________________________________________________________________";

	std::ofstream fout("C:/Users/mrcco/Documents/GitHub/VXS-Crate-2/output.txt");
	while( !s_det_timing.empty() ){
		trigger_array_t output = s_det_timing.read();
		fout << border << endl;
		for( int det_index = 0; det_index < 3; det_index++ ){
			int t32ns = 0;
			 /* output.trig_array[0] => scint
				output.trig_array[1] => pion
			    output.trig_array[2] => shower
			 */
			string det_name= "";
			if(det_index == 0) { det_name = "TRIG_SCINT"; }
			if(det_index == 1) { det_name = "PION"; }
			if(det_index == 2) { det_name = "SHOWER_MAX"; }
			fout << det_name << ": Timing Information: MSB(12ns) <----------> LSB (-16ns)" << endl;
			for( int time_tick = 7; time_tick > -1; time_tick-- ){
				if(output.trig_array[det_index].trig[time_tick])
					fout << "Trigger Found at T=" << ( (t32ns*32)+(time_tick*4)-16 ) << "ns\n";
			}
			for( int time_tick = 7; time_tick > -1; time_tick-- ){
				fout << "[" << output.trig_array[det_index].trig[time_tick] << "]";
			}
			fout << "---" << endl;
			t32ns++;
		}
		fout << border << endl;
	}
	while( !s_pion_bitmap.empty() ){
		shower_pion_det_bitmap_t output = s_pion_bitmap.read();
		fout << "PION: MSB(Seg#27) <----------> LSB (Seg#0)" << endl;
		for( int segment_index = 27; segment_index > -1; segment_index-- ){
			fout << "[" << output.segment[segment_index] << "]";
		}
		fout << endl;
	}
	while( !s_shower_bitmap.empty() ){
		shower_pion_det_bitmap_t output = s_shower_bitmap.read();
		fout << border << endl;
		fout << "SHOWER_MAX: MSB(Seg#27) <----------> LSB (Seg#0)" << endl;
		for( int segment_index = 27; segment_index > -1; segment_index-- ){
			fout << "[" << output.segment[segment_index] << "]";
		}
		fout << endl;
	}
	while( !s_scint_bitmap.empty() ){
		scint_det_bitmap_t output = s_scint_bitmap.read();
		fout << border << endl;
		fout << "TRIG_SCINT: MSB(Seg_Group#6) <----------> LSB (Seg_Group#0)" << endl;
		for( int segment_index = 6; segment_index > -1; segment_index-- ){
			fout << "[" << output.segment[segment_index] << "]";
		}
		fout << endl;
	}
	while( !s_pion_info.empty() ){
		det_information_t output = s_pion_info.read();
		fout << border << endl;
		fout << "PION:" << endl;
		fout << "Total Energy: " << output.total_energy << endl;
		fout << "Number Hits: " << output.total_hits << endl;
	}
	while( !s_shower_info.empty() ){
		det_information_t output = s_shower_info.read();
		fout << border << endl;
		fout << "SHOWER_MAX:" << endl;
		fout << "Total Energy: " << output.total_energy << endl;
		fout << "Number Hits: " << output.total_hits << endl;
	}
	while( !s_scint_info.empty() ){
		det_information_t output = s_scint_info.read();
		fout << border << endl;
		fout << "TRIG_SCINT:" << endl;
		fout << "Total Energy: " << output.total_energy << endl;
		fout << "Number Hits: " << output.total_hits << endl;
	}
	fout << border << endl;


    return 0;    
}
