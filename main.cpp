#include <iostream>
using std::cout; using std::endl;
#include <string>
using std::string;
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

	while( !s_det_timing.empty() ){
		trigger_array_t output = s_det_timing.read();
		cout << border << endl;
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
			cout << det_name << ": Timing Information: MSB(12ns) <----------> LSB (-16ns)" << endl;
			for( int time_tick = 7; time_tick > -1; time_tick-- ){
				if(output.trig_array[det_index].trig[time_tick])
					cout << "Trigger Found at T=" << ( (t32ns*32)+(time_tick*4)-16 ) << "ns\n";
			}
			for( int time_tick = 7; time_tick > -1; time_tick-- ){
				cout << "[" <<output.trig_array[det_index].trig[time_tick] << "]";
			}
			cout << "---" << endl;
			t32ns++;
		}
		cout << border << endl;
	}
	while( !s_pion_bitmap.empty() ){
		shower_pion_det_bitmap_t output = s_pion_bitmap.read();
		cout << "PION: MSB(Seg#28) <----------> LSB (Seg#1)" << endl;
		for( int segment_index = 27; segment_index > -1; segment_index-- ){
			cout << "[" << output.segment[segment_index] << "]";
		}
		cout << endl;
	}
	while( !s_shower_bitmap.empty() ){
		shower_pion_det_bitmap_t output = s_shower_bitmap.read();
		cout << border << endl;
		cout << "SHOWER_MAX: MSB(Seg#28) <----------> LSB (Seg#1)" << endl;
		for( int segment_index = 27; segment_index > -1; segment_index-- ){
			cout << "[" << output.segment[segment_index] << "]";
		}
		cout << endl;
	}
	while( !s_scint_bitmap.empty() ){
		scint_det_bitmap_t output = s_scint_bitmap.read();
		cout << border << endl;
		cout << "TRIG_SCINT: MSB(Seg_Group#7) <----------> LSB (Seg_Group#1)" << endl;
		for( int segment_index = 6; segment_index > -1; segment_index-- ){
			cout << "[" << output.segment[segment_index] << "]";
		}
		cout << endl;
	}
	while( !s_pion_info.empty() ){
		det_information_t output = s_pion_info.read();
		cout << border << endl;
		cout << "PION:" << endl;
		cout << "Total Energy: " << output.total_energy << endl;
		cout << "Number Hits: " << output.total_hits << endl;
	}
	while( !s_shower_info.empty() ){
		det_information_t output = s_shower_info.read();
		cout << border << endl;
		cout << "SHOWER_MAX:" << endl;
		cout << "Total Energy: " << output.total_energy << endl;
		cout << "Number Hits: " << output.total_hits << endl;
	}
	while( !s_scint_info.empty() ){
		det_information_t output = s_scint_info.read();
		cout << border << endl;
		cout << "TRIG_SCINT:" << endl;
		cout << "Total Energy: " << output.total_energy << endl;
		cout << "Number Hits: " << output.total_hits << endl;
	}
	cout << border << endl;


    return 0;    
}
