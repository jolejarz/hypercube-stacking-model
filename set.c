/**************************************************************
***************************************************************
*** 
***   set.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains the routines used to set the parameters for 'evolve'.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int set_seed(double seed_low, double seed_high, struct rng_struct *rng_struct)
{
	unsigned long int seed;
	
	seed=(unsigned long int)seed_high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)seed_low;
	rng_struct->rng_seed(rng_struct, seed);

	return set_ok;
}

short int set_settsc(struct rng_struct *rng_struct)
{
	unsigned int low, high;
	unsigned long int seed;
	
	asm ("rdtsc" : "=a" (low), "=d" (high));
	seed=(unsigned long int)high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)low;
	rng_struct->rng_seed(rng_struct, seed);	
	
	return set_ok;
}

