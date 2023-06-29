#pragma once
#include "../moller_hls.h"
#include "../chan_map.h"

const chan_map chmap[N_SLOT][16] = {
{ {SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1} },
{ {SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 3, -1},{SHOWER_MAX, 3, -1},{SHOWER_MAX, 3, -1},{SHOWER_MAX, 3, -1},{NONE, 3, -1},{NONE, 3, -1},{NONE, 3, -1},{NONE, 3, -1} },
{ {PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1} },
{ {PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 7, -1},{PION_DET, 7, -1},{PION_DET, 7, -1},{PION_DET, 7, -1},{NONE, 7, -1},{NONE, 7, -1},{NONE, 7, -1},{NONE, 7, -1} },
{ {TRIG_SCINT, 8, 71},{TRIG_SCINT, 8, 72},{TRIG_SCINT, 8, 73},{TRIG_SCINT, 8, 74},{TRIG_SCINT, 8, 75},{TRIG_SCINT, 8, 76},{TRIG_SCINT, 8, 77},{TRIG_SCINT, 8, 64},{TRIG_SCINT, 9, 65},{TRIG_SCINT, 9, 66},{TRIG_SCINT, 9, 67},{TRIG_SCINT, 9, 68},{TRIG_SCINT, 9, 69},{TRIG_SCINT, 9, 70},{NONE, 9, -1},{NONE, 9, -1} }
};