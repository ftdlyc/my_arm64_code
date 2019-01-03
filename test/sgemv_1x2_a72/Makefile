CXX = aarch64-linux-gnu-g++
CXXFLAGS = -O0 -std=c++11 -w
CXX_DEFINES =
CXX_INCLUDES =

test_sgemv_1x2_a72: main.o sgemv_1x2_a72.o
	$(CXX) main.o sgemv_1x2_a72.o -o test_sgemv_1x2_a72

main.o: main.cc
	$(CXX) $(CXXFLAGS) $(CXX_DEFINES) $(CXX_INCLUDES) -c main.cc
sgemv_1x2_a72.o: ../../sgemv_1x2_a72.s
	$(CXX) -c ../../sgemv_1x2_a72.s
clean:
	rm test_sgemv_1x2_a72 main.o test_sgemv_1x2_a72.o

