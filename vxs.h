#ifndef VXS_H
#define VXS_H


#include <ap_int.h>
#include <hls_stream.h>
#include "variables.h"
#include "chan_map.h"
#include "std_map.h"

// hit_t:
// - every 32ns each fadc reports 13 bit energy, and 3 bit hit time (time offset in current 32ns clock: 0=0ns, 1=4ns, 2=8ns, ..., 7=28ns)
// - if the channel has no hit, then the energy, e, will be reported as 0
// - energy, e, will saturate at 8191 (e.g. if the FADC intergal [after pedastal subtr. and gian] is greater than 8191, the FADC reports 8191)
typedef struct 
{
	ap_uint<13> e;
	ap_uint<3> t;
} hit_t;


// fadc_hits_t:
// - contains 70  VXS channels of hit_t reported by the FADCs each 32ns pulse
// - vxs_ch[ 0 ] to vxs_ch[ 15 ]: VME slot __, ch 0 to 15 FADC channels
// - vxs_ch[ 16 ] to vxs_ch[ 31 ]: VME slot __, ch 0 to 15 FADC channels
// - vxs_ch[ 32 ] to vxs_ch[ 47 ]: VME slot __, ch 0 to 15 FADC channels
// - (VXS switch A and B are at VME slot positions 11,12 so the FADC cannot be installed here
// - vxs_ch[ 48 ] to vxs_ch[ 63 ]: VME slot __, ch 0 to 15 FADC channels
// - vxs_ch[ 64 ] to vxs_ch[ 79 ]: VME slot __, ch 0 to 15 FADC channels
// Note: 
//      28 Channels -> ShowerMax
//      28 Channels -> Pion Det
//      14 Channels -> Trigger Scintillators (7 pairs)
//      Total: 70 Channels (slots 8-10 and slots 13-14)
//      Only on free channel when all said and done so we are packing the channels rather than dedicating 32 channels (2 whole FADC) 
//      to showermax / pion det or 16 channels (1 whole FADC) to trigger scint.  
//

typedef struct
{
	hit_t vxs_chan[N_CHAN];
} fadc_hits_t;


// trigger_t:
// - trig: 			   bitmap for time - [0]=>0ns, [1]=>4ns, [2]=>8ns, ..., [7]=28ns, when bit=0 no trigger, when bit=1 trigger
typedef struct
{
	ap_uint<8> trig;
} trigger_t;

// - 28-element array, one for each detector and stores timing info: [0] = det_1, [1] = det_2, ..., [7] = det_7, ..., etc; when bit=0 no time_trigger, when bit=1 time_trigger
// - Comment: is there a better way to store and transmit this data?
typedef struct
{
	trigger_t det_trig[28]; // one element corresponds to one detector
} trigger_shower_pion_det_t;

// - 7-element array, one for each detector and stores timing info: [0] = det_1, [1] = det_2, ..., [7] = det_7, ..., etc; when bit=0 no time_trigger, when bit=1 time_trigger
// - Comment: is there a better way to store and transmit this data?
typedef struct
{
	trigger_t det_trig[7]; // one element corresponds to one detector
} trigger_scint_det_t;

// shower_pion_det_bitmap_t:
// - segment: bitmap for detector bitmap for each segment - [0]=s0, [1]=s1, [2]=s2, ..., [7]=s6; when bit=0 no trigger, when bit=1 trigger
// - bitpadding: padding to make struct 32 bits wide
typedef struct
{
	ap_uint<28> segment;
	ap_uint<4> bitpadding;
} shower_pion_det_bitmap_t;

// scint_det_bitmap_t:
// - segment: bitmap for detector bitmap for each segment - [0]=s0, [1]=s1, [2]=s2, ..., [7]=s6; when bit=0 no trigger, when bit=1 trigger
// - bitpadding: padding to make struct 8 bits wide
typedef struct 
{
	ap_uint<7> segment;
	ap_uint<1> bitpadding;
} scint_det_bitmap_t;

typedef struct
{
	ap_uint<16> total_energy;
	ap_uint<8> total_hits;
} det_information_t;

// Function Declarations 

/*
*  vsx:
*  Brief: Top Function for the Vitis program. 
*  Description: Runs through all fadc data, generates timing bitmap,
*				detector-hit bitmap, coincidence for the TRIG_SCINT,
*  Parameter:	hit_dt - coincidence tolerance (given in 4ns ticks)
*  				energy_threshold - minimum energy for a hit to occur
*  				detector_threshold - the minimum summed energy over all segments for a detector to be hit
*  				s_fadc_hits - input stream of fadc data
*  				s_pion_trig - output stream of pion_det trigger data
*  				s_shower_trig - output stream of shower_det trigger data
*  				s_scint_trig - output stream of scint_det trigger data
*  				s_pion_info - output stream of total pion_det information
*  				s_shower_info - output stream of total shower_det information
*  				s_scint_info - output stream of total scint_det information
*/
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
);







#endif
