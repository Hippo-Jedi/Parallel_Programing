all: arraymult 

simd.p5.o:	simd.p5.h  simd.p5.cpp
		g++  -c  simd.p5.cpp -o simd.p5.o

arraymult:	arraymult.cpp simd.p5.o
		clang++ -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp arraymult.cpp -o simd.p5.o