/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/** @file tml_stochastics.c 
 * This file implements the function regarding random numbers and stochastic
 * computations.
 * It uses the Ziggurat method to generate random variables of normal and exponential
 * distributions [1].
 * 
 * @todo Dont use global static variables for number generator state
 * 
 * [1] Marsaglia, G. and Tsang, W.W., The Ziggurat Method for Generating 
 * Random Variables," Journal of Statistical Software, (2000), 5(8). 
 * Available online at http://www.jstatsoft.org/v05/i08/.
 */

/*
Authors:
- Jorg Lotze
- Hicham Lahlou
*/

#define TML_W32DLL

#include <math.h>
#include <stdlib.h>
#include "tml.h"


/* ------------------------- Random Number Generator by Ziggurat Method */
/* doesnt work properly - so we use another method below */

/* this function is taken from http://c-faq.com/lib/gaussian.html */
/* FIXXX ME: probably a better version around - but it works well */
      double gaussrand()
      {
        static double V1, V2, S;
        static int phase = 0;
        double X;

        if(phase == 0) {
          do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
            } while(S >= 1 || S == 0);

          X = V1 * sqrt(-2 * log(S) / S);
        } else
          X = V2 * sqrt(-2 * log(S) / S);

        phase = 1 - phase;

        return X;
      }
      
      /** generates a standard normally distributed random number (double) */
      #define RNOR (gaussrand())

/* ------------------------------------------------------------ */


TMLIMP(void, tml_sr_RandGausInit, (unsigned long seed, float mean, float stdDev,
    TML_sr_RandGausState *statePtr))
{
  statePtr->mean = mean;
  statePtr->stDev = stdDev;
  srand(seed); 
}
    
    
TMLIMP(void, tml_sc_RandGausInit, (unsigned long seed, float mean, float stdDev,
    TML_sc_RandGausState *statePtr))
{
  statePtr->mean = mean;
  statePtr->stDev = stdDev;
  srand(seed);  
}

TMLIMP(void, tml_dr_RandGausInit, (unsigned long seed, double mean, double stdDev,
    TML_dr_RandGausState *statePtr))
{
  statePtr->mean = mean;
  statePtr->stDev = stdDev;
  srand(seed);  
}

TMLIMP(void, tml_dc_RandGausInit, (unsigned long seed, double mean, double stdDev,
    TML_dc_RandGausState *statePtr))
{
  statePtr->mean = mean;
  statePtr->stDev = stdDev;
  srand(seed);  
}

TMLIMP(float, tml_sr_RandGaus, (TML_sr_RandGausState *statePtr))
{
  return ((float)RNOR) * statePtr->stDev + statePtr->mean;
}

TMLIMP(SCplx, tml_sc_RandGaus, (TML_sc_RandGausState *statePtr))
{
  SCplx tmp;
  tmp.re = ((float)RNOR * statePtr->stDev) + statePtr->mean;
  tmp.im = ((float)RNOR * statePtr->stDev) + statePtr->mean;
  return tmp;
}

TMLIMP(double, tml_dr_RandGaus, (TML_dr_RandGausState *statePtr))
{
  return ((double)RNOR) * statePtr->stDev + statePtr->mean;
}

TMLIMP(DCplx, tml_dc_RandGaus, (TML_dc_RandGausState *statePtr))
{

  DCplx tmp;
  tmp.re = ((double)RNOR * statePtr->stDev) + statePtr->mean;
  tmp.im = ((double)RNOR * statePtr->stDev) + statePtr->mean;
  return tmp;
}

TMLIMP(void, tml_sr_vRandGaus, (TML_sr_RandGausState *statePtr, float *samps, int sampsLen))
{
  int i;
  for (i = 0; i < sampsLen; i++)
    samps[i] = ((float)RNOR) * statePtr->stDev + statePtr->mean; 
}

TMLIMP(void, tml_sc_vRandGaus, (TML_sc_RandGausState *statePtr, SCplx *samps, int sampsLen))
{
  int i;
  for (i = 0; i < sampsLen; i++)
  {
    samps[i].re = ((float)RNOR) * statePtr->stDev + statePtr->mean;
    samps[i].im = ((float)RNOR) * statePtr->stDev + statePtr->mean;
  } 
}

TMLIMP(void, tml_dr_vRandGaus, (TML_dr_RandGausState *statePtr, double *samps, int sampsLen))
{
  int i;
  for (i = 0; i < sampsLen; i++)
    samps[i] = ((double)RNOR) * statePtr->stDev + statePtr->mean; 
}

TMLIMP(void, tml_dc_vRandGaus, (TML_dc_RandGausState *statePtr, DCplx *samps, int sampsLen))
{
  int i;
  for (i = 0; i < sampsLen; i++)
  {
    samps[i].re = ((double)RNOR) * statePtr->stDev + statePtr->mean;
    samps[i].im = ((double)RNOR) * statePtr->stDev + statePtr->mean;
  } 
}
