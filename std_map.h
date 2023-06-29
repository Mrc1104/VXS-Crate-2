#ifndef STD_MAP_H
#define STD_MAP_H
#include <map>
const std::map<std::string, det_type> m = 
{
    {"NONE", NONE},
    {"RING_ONE", RING_ONE},
    {"RING_TWO", RING_TWO},
    {"RING_THREE", RING_THREE},
    {"RING_FOUR", RING_FOUR},
    {"RING_FIVE", RING_FIVE},
    {"RING_SIX", RING_SIX},
    {"TRIG_SCINT", TRIG_SCINT},
    {"PION_DET", PION_DET},
    {"SHOWER_MAX", SHOWER_MAX}
};
const std::map<det_type, std::string> m2 = 
{
    {NONE, "NONE"},
    {RING_TWO, "RING_TWO"},
    {RING_THREE, "RING_THREE"},
    {RING_FOUR, "RING_FOUR"},
    {RING_FIVE, "RING_FIVE"},
    {RING_SIX, "RING_SIX"},
    {TRIG_SCINT, "TRIG_SCINT"},
    {PION_DET, "PION_DET"},
    {SHOWER_MAX, "SHOWER_MAX"}
};
#endif
