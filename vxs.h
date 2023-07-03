#ifndef VXS_H
#define VXS_H


#include <ap_int.h>
#include <hls_stream.h>
#include "variables.h"
#include "chan_map.h"
#include "detector_type.h"
#include "std_map.h"

// Detector to channel mapping
// generated from parameter file (usually found in chan_map/*.conf
// see map.csh to create new parameter file(s) and/or header file(s)
#include "chan_map/naive_map.h"

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
// - trig:    bitmap for time - [0]=>0ns, [1]=>4ns, [2]=>8ns, ..., [7]=28ns, when bit=0 no trigger, when bit=1 trigger
typedef struct
{
	ap_uint<8> trig;
} trigger_t;

// trigger_array_t
// - trig_array: array of the detector timing bitmaps.
//				 We have three detector types so three elements
//				 IMPORTANT:
//				 trig_array[0] => scint detectors
//				 trif_array[1] => pion detectors
//				 trig_array[2] => shower detectors
//				 See detector_type.h to see how this was determined (det_enum int# -> zero)
typedef struct
{
	trigger_t trig_array[3];
} trigger_array_t;


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

// det_information_t
// - total_energy: the summed energy across all detectors.
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
		hls::stream<fadc_hits_t> &s_fadc_hits,
		hls::stream<trigger_array_t> &s_det_timing,
		hls::stream<shower_pion_det_bitmap_t> &s_pion_bitmap,
		hls::stream<shower_pion_det_bitmap_t> &s_shower_bitmap,
		hls::stream<scint_det_bitmap_t> &s_scint_bitmap,
		hls::stream<det_information_t> &s_pion_info,
		hls::stream<det_information_t> &s_shower_info,
		hls::stream<det_information_t> &s_scint_info
);

/* make_timing_bitmap:
 * Brief: 		creates a timing bitmap from input data
 * Description: The time reported by the fadc is backwards. Early events arrive
 * 				to the fadc and wait for the next clock cycle so they are reported
 * 				as a later time. This functions rearranges the time appropriately
 * 				'Early' events -> true later
 * 				'Late' events -> true early
 * Parameter:	fadc_time - time from the fadc (given in 4ns ticks)
 * 				ptr_timing - address of the trigger bitmap
 * Comment:
 * 				See detector_type.h to see how this was determined
 *				det_id = 7 => scint ; arr_trig_bitmap[0] => scint
 *				det_id = 8 => pion  ; arr_trig_bitmap[1] => pion
 *				det_id = 9 => shower; arr_trig_bitmap[2] => shower
 */
void make_timing_bitmap(const ap_uint<3> fadc_time, trigger_t *ptr_timing);

/* scint_coincidence:
 * Brief: 		checks to see if scint pairs meet coincidence condition
 * Description: Takes in the timing information for each pair and computes if
 * 				the difference is within the defined tolerance
 * 				tolerance defined in variables.h
 * Parameter:   scint1 - timing information for the first scint
 * 				scint2 - timing information for th esecond scit
 * 				hit_dt - coincidence tolerance (given in 4ns ticks)
 *
 */
ap_uint<1> scint_coincidence(const ap_uint<3> scint1, const ap_uint<3> scint2, const ap_uint<3> hit_dt);

/* make_shower_pion_bitmap
 * Brief: 		creates a bitmap of the segment that were hit
 * Description: Sets the appropriate bit to 1 if the corresponding segment was hit
 * Parameter:	seg_bitmap - pointer to the segment bitmap
 * 				seg_num - segment number that was hit
 */
void make_shower_pion_bitmap(ap_uint<28> &seg_bitmap, int seg_num);

/* make_scint_bitmap
 * Brief: 		creates a bitmap of the segment that were hit
 * Description: Sets the appropriate bit to 1 if the corresponding segment was hit
 * Parameter:	seg_bitmap - pointer to the segment bitmap
 * 				seg_num - segment number that was hit
 */
void make_scint_bitmap(ap_uint<7> &seg_bitmap, int seg_num);


#endif
