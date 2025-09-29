/**************************************************************
***************************************************************
*** 
***   set.h
***
***   Jason Olejarz
***   2011-01-26
***
***   This is the header file for set.c.
***
***************************************************************
**************************************************************/

enum {set_error, set_ok};

short int set_seed(double seed_low, double seed_high, struct rng_struct *rng_struct);
short int set_settsc(struct rng_struct *rng_struct);

