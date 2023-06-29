#ifndef VXS_H
#define VXS_H


#include <ap_int.h>
#include <hls_stream.h>
#include "variables.h"

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
// - 8-element array, one for each ring: [0] = r1, [1] = r2, ..., [7] = r7; when bit=0 no time_trigger, when bit=1 time_trigger
typedef struct
{
	ap_uint<8> trig[8];
} trigger_t;

// ring_trigger_t:
// - ring trig: bitmap for ring hit - [0]=r0, [1]=r1, [2]=r2, ..., [7]=r6; when bit=0 no ring_trigger, when bit=1 ring_trigger
typedef struct 
{
	ap_uint<8> ring;
} ring_trigger_t;


// Function Declarations 

/*
*  vsx:
*  Brief: Top Function for the Vitis program. 
*  Description: Runs through all fadc data, generates timing bitmap,
*				detector-hit bitmap, coincidence for the TRIG_SCINT,
*								 
*/
void vxs
(


);







#endif
