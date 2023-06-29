HEADER = ~/FPGA/MOLLER_VTP/include/

a.out: vxs.h vxs.cpp main.cpp
	g++ -I $(HEADER) vxs.cpp main.cpp
debug: vxs.h vxs.cpp main.cpp
	g++ -I $(HEADER) -g vxs.cpp main.cpp