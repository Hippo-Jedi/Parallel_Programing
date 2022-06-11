#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef NUMTRIES
	#define NUMTRIES	100
#endif

int	NowYear;		// 2022 - 2027
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int monthCount;

float nowNumPred;   

unsigned int seed = 0;

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

omp_lock_t	Lock;
int		NumInThreadTeam;
int		NumAtBarrier;
int		NumGone;

// Returns square root
float SQR(float x) {
    return x*x;
}

// Returns random float in range
float Ranf(unsigned int *seedp, float low, float high) {
    float r = (float) rand_r( seedp ); // 0 - RAND_MAX
    return(low+r*(high-low)/(float)RAND_MAX);
}

// Returns random int in range
int Ranf(unsigned int *seedp, int ilow, int ihigh) {
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;
    return (int)(Ranf(seedp, low,high));
}

// Sets NowPrecip and NowTemp for next environment state
void setNextEnvironment() {
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. ) {
        NowPrecip = 0.;
    }
}

void Deer() {
    while(NowYear < 2028) {
        int nextNumDeer;

        if (float(NowNumDeer) > NowHeight) {
            nextNumDeer = NowNumDeer--;
        }
        else if (float(NowNumDeer) < NowHeight) {
            nextNumDeer = NowNumDeer++;
        }
        else {
            nextNumDeer = NowNumDeer;
        }
        if (nextNumDeer < 0) {
            nextNumDeer = 0;
        }

        if (nowNumPred > .0) {
            nextNumDeer = nextNumDeer - 2;
        }
        
        // DoneComputing barrier:
        #pragma omp barrier

        NowNumDeer = nextNumDeer;

        // DoneAssigning barrier:
        #pragma omp barrier

        // DonePrinting barrier:
        #pragma omp barrier
    }
}

void Grain() {
    while(NowYear < 2028) {
        float nextHeight;

        float tempFactor = exp(-SQR((NowTemp-MIDTEMP)/10.));
        float precipFactor = exp(-SQR((NowPrecip-MIDPRECIP)/10.));

        nextHeight = NowHeight;
        nextHeight += tempFactor*precipFactor*GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer*ONE_DEER_EATS_PER_MONTH;

        if (nextHeight < 0) {
            nextHeight = 0;
        }

        // DoneComputing barrier:
        #pragma omp barrier

        NowHeight = nextHeight;
        
        // DoneAssigning barrier:
        #pragma omp barrier

        // DonePrinting barrier:
        #pragma omp barrier
    }
}

void Watcher() {
    while(NowYear < 2028) {
 
        // DoneComputing barrier:
        #pragma omp barrier

        // DoneAssigning barrier:
        #pragma omp barrier

        // Increment Time
        if (NowMonth >= 11) {
            NowMonth = 0;
            NowYear++;
        }
        else {
            NowMonth++;
        }
        monthCount++;

        setNextEnvironment();

    printf("%d, %d,%d,%lf,%lf,%lf\n", monthCount, NowNumDeer, int(nowNumPred * 100), (NowHeight * 2.54),(5./9.)*(NowTemp-32), (NowPrecip * 2.54));

        // DonePrinting barrier:
        #pragma omp barrier
    }
}

void Predators() {
    while(NowYear < 2028) {
        int rand = Ranf(&seed, 2, 10);
        float nextPred = 0.0;

        if (NowNumDeer > 4) {
            nextPred = .1;
        }

        #pragma omp barrier
        nowNumPred = nextPred;

        #pragma omp barrier
        
        #pragma omp barrier
    }
}

int main() {
    NowMonth = 0;
    monthCount = 0;
    NowYear  = 2022;
    NowNumDeer = 1;
    NowHeight =  1.;
    nowNumPred = 0.;

    setNextEnvironment();

    printf("Month,NowNumDeer,nowNumPred,NowHeight,NowTemp,NowPrecip\n");
    printf("%d,%d,%d,%lf,%lf,%lf\n", monthCount, NowNumDeer, int(nowNumPred*100), (NowHeight * 2.54),(5./9.)*(NowTemp-32), (NowPrecip * 2.54));

    omp_set_num_threads(4);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Deer();
        }

        #pragma omp section
        {
            Grain();
        }

        #pragma omp section
        {
            Watcher();
        }

        #pragma omp section
        {
            Predators();
        }
    } // implied barrier -- all functions must return in order
	// to allow any of them to get past here
}