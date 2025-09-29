/**************************************************************
***************************************************************
*** 
***   run.h
***
***   Jason Olejarz
***   2011-01-26
***
***   This is the header file for run.c.
***
***************************************************************
**************************************************************/

// ******** NOTE:  Some routines do not care whether the evolution is in 3d, 4d, or 5d, so they assume
// ******** that the parameters structure is that for the 3d case.  Therefore, the 3d, 4d, and 5d
// ******** parameters structures are set up to be identical in their first entries.  This allows these
// ******** routines to assume that the evolution is 3d even though it may be 4d or 5d.

// ******** NOTE:  Some of the entries that are `int' in the 3d parameters structure are `short int'
// ******** in the 4d and 5d parameters structures.  This is to save memory.

struct run_parameters
{
	struct rng_struct *rng_struct;
	unsigned long int maxsteps;
	double maxtime;
	short int fullbias;
	double prob_outer_corner_flips;
	int *map_out;
	int *index_out;
	int *map_in;
	int *index_in;
	int coordinate;
	short int move;
	unsigned long int steps;
	double time;
	int hlen;
	int xlen;
	int ylen;
	int *height_initial;
	int *height;
	int *statex_initial;
	int *statex;
	int *statey_initial;
	int *statey;
};

struct run_parameters_4d
{
	struct rng_struct *rng_struct;
	unsigned long int maxsteps;
	double maxtime;
	short int fullbias;
	double prob_outer_corner_flips;
	int *map_out;
	int *index_out;
	int *map_in;
	int *index_in;
	int coordinate;
	short int move;
	unsigned long int steps;
	double time;
	short int hlen;
	short int xlen;
	short int ylen;
	short int qlen;
	short int *height_initial;
	short int *height;
	short int *statex_initial;
	short int *statex;
	short int *statey_initial;
	short int *statey;
	short int *stateq_initial;
	short int *stateq;
};

struct run_parameters_4d3
{
	struct rng_struct *rng_struct;
	unsigned long int maxsteps;
	double maxtime;
	short int fullbias;
	double prob_outer_corner_flips;
	int *map_out;
	unsigned long int *index_out;
	int *map_in;
	int *index_in;
	int coordinate;
	short int move;
	unsigned long int steps;
	double time;
	short int hlen;
	short int xlen;
	short int ylen;
	short int qlen;
	short int *height_initial;
	short int *height;
	short int *statex_initial;
	short int *statex;
	short int *statey_initial;
	short int *statey;
	short int *stateq_initial;
	short int *stateq;
};

struct run_parameters_5d
{
	struct rng_struct *rng_struct;
	unsigned long int maxsteps;
	double maxtime;
	short int fullbias;
	double prob_outer_corner_flips;
	int *map_out;
	int *index_out;
	int *map_in;
	int *index_in;
	int coordinate;
	short int move;
	unsigned long int steps;
	double time;
	short int hlen;
	short int xlen;
	short int ylen;
	short int qlen;
	short int rlen;
	short int *height_initial;
	short int *height;
	short int *statex_initial;
	short int *statex;
	short int *statey_initial;
	short int *statey;
	short int *stateq_initial;
	short int *stateq;
	short int *stater_initial;
	short int *stater;
};

struct integrate_parameters
{
	int length;
	int maxtime;
	double *height;
	double delta_t;
	double delta_x;
};

short int run (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters);
short int runbias (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters);
short int run4d (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_4d *parameters);
short int run4d2 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_4d *parameters);
short int run4d3 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_4d3 *parameters);
short int run5d (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_5d *parameters);
short int run_integrate_1 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters);
short int run_integrate_4d_1 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters);
short int run_integrate_2 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters);
short int run_integrate_4d_2 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters);
short int run_integrate_3 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters);

