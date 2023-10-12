# VXS-Crate-2
Repo for the Shower Max, Pion, and Scintillator detector FPGA code. Basic Trigger ideas. 

# Layout
There are two git repos,
1. MOLLER_VTP (https://github.com/Mrc1104/MOLLER_VTP)

and 

2. VXS-Crate-2 (this one)

The two are very similar projects but with different scopes. MOLLER_VTP is for only the thin quartz detectors wheras the VXS-Crate-2 is for the shower max, pion detectors, and trigger scintillator pairs. Both repos, however, were trying to set up basic trigger functionality: can we see what detector was hit, which segment it occured in, and the timing of the hit. 

Moller_VTP was written first and served as the primary testing ground for ideas and changes that were ported over to VXS-Crate-2. It is strongly recommended to look through MOLLER_VTP first. Additionally, the general ideas in VXS-Crate-2 come from MOLLER_VTP.

One thing to note, VXS-Crate-2 and MOLLER_VTP are *almost* one-to-one in terms of functionality. The only difference is that VXS-Crate-2 does not have the static trigger configuration capabilities as described in MOLLER_VTP (I never got around to porting that over. I can port it over if its needed, just let me know).

# Main Files
The three files
* main.cpp      -- testbench file
* vxs.cpp       -- top function file
* vxs.h         -- header file

are the main file programs of the rep. They are meant to be used with the AMD Vitis software to program a FPGA. 


## main.cpp
Start up program. Vitis expects a testbench function so this is it. It generates the random data, intitializes the input streams, calls the top function, moller_hls(), and then parses the resulting output streams. The output streams can either be printed to the screen or to a file. 

## vxs.h
Header file for the functions and structures declarations used in the top function, vxs(). The top function houses the majority of the logic and calls subfunctions to preform specific tasks.

## vxs.cpp
Implementation file for the functions declared in vxs.h. 

### About:
The goal was to create simple trigger logic for the Pion det, Trigger Scintillator,and shower max where we can generate fake data, parse said data,and output timing and placement of the hits for every detector across all 28 segments.
* Shower Max            - 28 channels 
* Pion Det              - 28 Channels 
* Trigger Scintillators - (7 pairs, Each pair spans 4 segments)

(See the DAQ update to technical board, 12 June 2023 by Paul King)

We have 10 FADC with one channel free that provides energy and time data every 32ns.We also want to have a channel-to-detector map so we can have abritrary channel-detector configurations. Likewise, we want to be able to select which detectors, which segments, and with what scalars that we want (NOT IMPLEMENTED). 
The configuration is done very statically using configuration scripts.
* map.csh - Takes input from the user, calls the script.mk, and generates chan-det mapping arrays. 

## Contact:

Name: Ryan Conaway

Email: mrc@jlab.org
