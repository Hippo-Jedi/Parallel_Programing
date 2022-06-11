#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "simd.p5.h"

unsigned int seed = 0;  // a thread-private variable

float Ranf( unsigned int *seedp,  float low, float high ) {
    float r = (float) rand_r( seedp );              // 0 - RAND_MAX
    
    return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

float x = Ranf( &seed, -1.f, 1.f );

int main( int argc, char *argv[ ] ) {
    #ifndef _OPENMP
        fprintf( stderr, "OpenMP is not available\n" );
        return 1;
    #endif
    //Different array sizes to try. 1k to 32 million
    int size[10] = {1000,2000,5000,10000,100000,1000000,5000000,10000000,20000000,32000000};
    for (int r = 0; r < 10; r++) {
        float *A = new float[size[r]];
        float *B = new float[size[r]];
        float *C = new float[size[r]];
        //Part 1
        // Create arrays of random floats
        for (int i = 0; i < size[r]; i++) {
            A[i] = Ranf(&seed,0,10);
            B[i] = Ranf(&seed,0,10);
        }
        double start = omp_get_wtime();
        for (int j = 0; j < size[r]; j++) {
            C[j] = A[j] * B[j];
        }
        double end = omp_get_wtime();
        std::cout << "REGUMul " << " Size: " << size[r] << " Time: " << (end - start) << "\n";
    
        //Part 2
        for (int i = 0; i < size[r]; i++) {
            A[i] = Ranf(&seed,0,10);
            B[i] = Ranf(&seed,0,10);
        }
        start = omp_get_wtime();
        SimdMul(A, B, C, size[r]);
        end = omp_get_wtime();
        std::cout << "SIMDMul" << " Size: " << size[r] << " Time: " << (end - start) << "\n";
    
        //Part 3
        for (int i = 0; i < size[r]; i++) {
            A[i] = Ranf(&seed,0,10);
            B[i] = Ranf(&seed,0,10);
        }
        float sum = 0;
        start = omp_get_wtime();
        for (int i = 0; i < size[r]; i++) {
            sum += A[i] * B[i];
        }
        end = omp_get_wtime();
        std::cout << "REGUSum" << " Size: " << size[r] << " Time: " << (end - start) << "\n";
    
        //Part 4
        for (int i = 0; i < size[r]; i++) {
            A[i] = Ranf(&seed,0,10);
            B[i] = Ranf(&seed,0,10);
        }
        start = omp_get_wtime();
        SimdMulSum(A, B, size[r]);
        end = omp_get_wtime();
        std::cout << "SIMDSum" << " Size: " << size[r] << " Time: " << (end - start) << "\n";
    }
    return 0;
}