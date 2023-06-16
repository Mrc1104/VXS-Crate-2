HEADER = ~/FPGA/MOLLER_VTP/include/

a.out: vxs.h vxs.cpp main.cpp
	g++ -I $(HEADER) vxs.cpp main.cpp
genData: vxs.h vxs.cpp generateFakeData.cpp
	g++ -o fakedata.out -I $(HEADER) vxs.cpp generateFakeData.cpp
	./fakedata.out
debug: vxs.h vxs.cpp main.cpp
	g++ -I $(HEADER) -g vxs.cpp main.cpp
