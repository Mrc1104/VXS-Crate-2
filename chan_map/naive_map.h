#pragma once
#include "../variables.h"
#include "../chan_map.h"

const chan_map chmap[N_SLOT][16] = {
{ {SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 0, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1},{SHOWER_MAX, 1, -1} },
{ {SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 2, -1},{SHOWER_MAX, 3, -1},{SHOWER_MAX, 3, -1},{SHOWER_MAX, 3, -1},{SHOWER_MAX, 3, -1},{NONE, 3, -1},{NONE, 3, -1},{NONE, 3, -1},{NONE, 3, -1} },
{ {PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 4, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1},{PION_DET, 5, -1} },
{ {PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 6, -1},{PION_DET, 7, -1},{PION_DET, 7, -1},{PION_DET, 7, -1},{PION_DET, 7, -1},{NONE, 7, -1},{NONE, 7, -1},{NONE, 7, -1},{NONE, 7, -1} },
{ {TRIG_SCINT, 0, 71},{TRIG_SCINT, 4, 72},{TRIG_SCINT, 8, 73},{TRIG_SCINT, 8, 74},{TRIG_SCINT, 16, 75},{TRIG_SCINT, 20, 76},{TRIG_SCINT, 24, 77},{TRIG_SCINT, 0, 64},{TRIG_SCINT, 4, 65},{TRIG_SCINT, 8, 66},{TRIG_SCINT, 9, 67},{TRIG_SCINT, 16, 68},{TRIG_SCINT, 20, 69},{TRIG_SCINT, 24, 70},{NONE, 9, -1},{NONE, 9, -1} }
};