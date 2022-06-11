#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#define _USE_MATH_DEFINES

// setting the number of threads:
#ifndef NUMT
#define NUMT		4
#endif

// setting the number of numnodes:
#ifndef NUMNODES
#define NUMNODES		32
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTRIALS
#define NUMTRIALS	100
#endif

#define XMIN	 -1.
#define XMAX	 1.
#define YMIN	 -1.
#define YMAX	 1.

const float N = 2.5f;
const float R = 1.2f;

void		TimeOfDaySeed( );
float 		Height (int , int);

void
TimeOfDaySeed( )
{
	struct tm y2k = { 0 };
	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time_t  timer;
	time( &timer );
	double seconds = difftime( timer, mktime(&y2k) );
	unsigned int seed = (unsigned int)( 1000.*seconds );    // milliseconds
	srand( seed );
}

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float rn = pow( fabs(R), (double)N );
	float r = rn - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(double)N );
	return height;
}

int main( int argc, char *argv[ ] )
{
#ifndef _OPENMP
		    printf( stderr, "No OpenMP support!\n" );
		    return 1;
#endif

  //float tn = tan( Radians(30));
	TimeOfDaySeed( );		// seed the random number generator

	// get ready to record the maximum Heights:
	float maxMegaHeights = 0.;      
	double executionTime = 0.;

 
	// the area of a single full-sized tile:

	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	omp_set_num_threads( NUMT );

	double SumVolume = 0.;

	for ( int k = 0; k <= NUMTRIALS; k++){	
	  double time0 = omp_get_wtime( );

	  double volume = 0.0;

		#pragma omp parallel for default(none), shared(fullTileArea), reduction(+:volume)
		for( int i = 0; i < NUMNODES*NUMNODES; i++ )
		{
			
			int iu = i % NUMNODES;
			int iv = i / NUMNODES;

			float edgeModifier = 1.0;	//edge modifier

			if ((iu == 0 || iu == NUMNODES - 1) && (iv == 0 || iv == NUMNODES - 1))	//Corner modifier
			{
				volume += (double)(edgeModifier * fullTileArea) * Height(iu, iv) / 4.0;
			}
			else if ((iu == 0 || iu == NUMNODES - 1) || (iv == 0 || iv == NUMNODES - 1)) //Edge modifier
			{
				volume += (double)(edgeModifier * fullTileArea) * Height(iu, iv) / 2.0;
			}
			else
			{
				//add volume
				volume += (double)(edgeModifier * fullTileArea) * Height(iu, iv);
			}
			
		}

		double time1 = omp_get_wtime( );
		double megaHeightsPerSecond = (double)NUMNODES * NUMNODES / ( time1 - time0 ) / 1000000.;
			if( megaHeightsPerSecond > maxMegaHeights )
				maxMegaHeights = megaHeightsPerSecond;
		executionTime = time1 - time0;
    SumVolume += volume;

	}
	
  double AvgVolume = SumVolume / NUMTRIALS;
	//printf("Peak Performance = %10.2lf MegaHeights/Sec\n", maxMegaHeights);
	//printf( "Execution time for %d threads and %d NUMNODES: %lf\n", NUMT, NUMNODES, executionTime);
	//printf( "Volume: %lf\n", volume);
  printf("%d, %d, %lf, %lf\n", NUMT, NUMNODES, maxMegaHeights, AvgVolume);
}
