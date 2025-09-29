/**************************************************************
***************************************************************
*** 
***   run.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the core calculations for evolving the interfaces.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int run (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	short int fullbias=parameters->fullbias;
	double prob_outer_corner_flips=parameters->prob_outer_corner_flips;
	double prob_inner_corner_flips=1-prob_outer_corner_flips;
	unsigned long int steps=parameters->steps;
	int *height_initial=parameters->height_initial;
	int *height=parameters->height;
	int *statex_initial=parameters->statex_initial;
	int *statex=parameters->statex;
	int *statey_initial=parameters->statey_initial;
	int *statey=parameters->statey;
	int *map_out=parameters->map_out;
	int *index_out=parameters->index_out;
	int *map_in=parameters->map_in;
	int *index_in=parameters->index_in;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, i, j;

	int x, y;

	int xminus;
	int xplus;
	int yminus;
	int yplus;

	int area=xlen*ylen;

	int holes_out[2]={};
	int holes_out_count=0;
	int holes_in[2]={};
	int holes_in_count=0;
	
	for (i=0; i<xlen*ylen; i++)
	{
		*(index_out+i)=0;
		*(map_out+i)=0;
		*(index_in+i)=0;
		*(map_in+i)=0;
	}

	// Determine which spins are active.
	for (i=0; i<xlen*ylen; i++)
	{
		x=i%xlen;
		y=i/xlen;
		
		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==xlen-1) xplus=0;
		else xplus=x+1;
		if (y==ylen-1) yplus=0;
		else yplus=y+1;

		// Check if the current height can increase.
		if (*(statex_initial+x+y*xlen)>0 && *(statey_initial+x+y*xlen)>0)
		{
			(*index_out)++;
			*(index_out+*index_out)=x+y*xlen;
			*(map_out+x+y*xlen)=*index_out;
		}

		// Check if the current height can decrease.
		if (*(statex_initial+xplus+y*xlen)>0 && *(statey_initial+x+yplus*xlen)>0)
		{
			(*index_in)++;
			*(index_in+*index_in)=x+y*xlen;
			*(map_in+x+y*xlen)=*index_in;
		}
	}

	memcpy (height, height_initial, sizeof(int)*xlen*ylen);
	memcpy (statex, statex_initial, sizeof(int)*xlen*ylen);
	memcpy (statey, statey_initial, sizeof(int)*xlen*ylen);

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;
	
	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	// If the numbers of outer and inner corners are both zero, then there is nothing to do.
	if (*index_out==0 && *index_in==0) return 0;

	// If only outer corners can flip, then there must be at least one outer corner.
	if (fullbias==1 && *index_out==0) return 0;
	
	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/((*index_out)*prob_outer_corner_flips+(*index_in)*prob_inner_corner_flips));

		// This random number is used to determine whether the flipped spin is an outer or inner corner.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		
		if (fullbias==1 || random_number<(*index_out)*prob_outer_corner_flips/((*index_out)*prob_outer_corner_flips+(*index_in)*prob_inner_corner_flips))
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_out)*random_number);
			
			a=*(index_out+j);

			x=a%xlen;
			y=a/xlen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			// Increase the selected height.
			// If the height is already the maximum, then set the new height to zero.
			if (*(height+a)<hlen-1) (*(height+a))++;
			else *(height+a)=0;
			
			(*(statex+a))--;
			(*(statey+a))--;

			(*(statex+xplus+y*xlen))++;
			(*(statey+x+yplus*xlen))++;

			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=2;

			// If the flipped spin became active, enable it.
			if (*(map_in+a)==0)
			{
				(*index_in)++;
				*(index_in+*index_in)=a;
				*(map_in+a)=*index_in;
			}
			// If the flipped spin became inactive, disable it.
			if (*(statex+a)==0 || *(statey+a)==0)
			{
				*(index_out+j)=0;
				holes_out[holes_out_count++]=j;
				*(map_out+a)=0;
			}

			// If either of the two neighboring spins at (xplus, y) or (x, yplus) became active,
			// enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_out+xplus+y*xlen)==0 && *(statex+xplus+y*xlen)>0 && *(statey+xplus+y*xlen)>0)
			//
			// However, since the current height increased, the condition *(statex+xplus+y*xlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+xplus+y*xlen)==0 && *(statey+xplus+y*xlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=xplus+y*xlen;
				*(map_out+xplus+y*xlen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+yplus*xlen)==0 && *(statex+x+yplus*xlen)>0 && *(statey+x+yplus*xlen)>0)
			//
			// However, since the current height increased, the condition *(statey+x+yplus*xlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+yplus*xlen)==0 && *(statex+x+yplus*xlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=x+yplus*xlen;
				*(map_out+x+yplus*xlen)=*index_out;
			}

			// If either of the two neighboring spins at (xminus, y) or (x, yminus) became inactive,
			// disable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_in+xminus+y*xlen)!=0 && (*(statex+x+y*xlen)==0 || *(statey+xminus+yplus*xlen)==0))
			//
			// However, since the current height increased, the condition *(statey+xminus+yplus*xlen)==0 is
			// never satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+xminus+y*xlen)!=0 && *(statex+x+y*xlen)==0)
			{
				*(index_in+*(map_in+xminus+y*xlen))=0;
				holes_in[holes_in_count++]=*(map_in+xminus+y*xlen);
				*(map_in+xminus+y*xlen)=0;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+yminus*xlen)!=0 && (*(statex+xplus+yminus*xlen)==0 || *(statey+x+y*xlen)==0))
			//
			// However, since the current height increased, the condition *(statex+xplus+yminus*xlen)==0 is
			// never satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+x+yminus*xlen)!=0 && *(statey+x+y*xlen)==0)
			{
				*(index_in+*(map_in+x+yminus*xlen))=0;
				holes_in[holes_in_count++]=*(map_in+x+yminus*xlen);
				*(map_in+x+yminus*xlen)=0;
			}
		}
		else
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_in)*random_number);
			
			a=*(index_in+j);

			x=a%xlen;
			y=a/xlen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			// Decrease the selected height.
			// If the height is already the minimum, then set the new height to dh*units-1.
			if (*(height+a)>0) (*(height+a))--;
			else *(height+a)=hlen-1;
			
			(*(statex+a))++;
			(*(statey+a))++;

			(*(statex+xplus+y*xlen))--;
			(*(statey+x+yplus*xlen))--;

			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=1;

			// If the flipped spin became active, enable it.
			if (*(map_out+a)==0)
			{
				(*index_out)++;
				*(index_out+*index_out)=a;
				*(map_out+a)=*index_out;
			}
			// If the flipped spin became inactive, disable it.
			if (*(statex+xplus+y*xlen)==0 || *(statey+x+yplus*xlen)==0)
			{
				*(index_in+j)=0;
				holes_in[holes_in_count++]=j;
				*(map_in+a)=0;
			}

			// If either of the two neighboring spins at (xminus, y) or (x, yminus) became active,
			// enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_in+xminus+y*xlen)==0 && *(statex+x+y*xlen)>0 && *(statey+xminus+yplus*xlen)>0)
			//
			// However, since the current height decreased, the condition *(statex+x+y*xlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+xminus+y*xlen)==0 && *(statey+xminus+yplus*xlen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=xminus+y*xlen;
				*(map_in+xminus+y*xlen)=*index_in;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+yminus*xlen)==0 && *(statex+xplus+yminus*xlen)>0 && *(statey+x+y*xlen)>0)
			//
			// However, since the current height decreased, the condition *(statey+x+y*xlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+x+yminus*xlen)==0 && *(statex+xplus+yminus*xlen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=x+yminus*xlen;
				*(map_in+x+yminus*xlen)=*index_in;
			}

			// If either of the two neighboring spins at (xplus, y) or (x, yplus) became inactive,
			// disable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_out+xplus+y*xlen)!=0 && (*(statex+xplus+y*xlen)==0 || *(statey+xplus+y*xlen)==0))
			//
			// However, since the current height decreased, the condition *(statey+xplus+y*xlen)==0 is
			// never satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+xplus+y*xlen)!=0 && *(statex+xplus+y*xlen)==0)
			{
				*(index_out+*(map_out+xplus+y*xlen))=0;
				holes_out[holes_out_count++]=*(map_out+xplus+y*xlen);
				*(map_out+xplus+y*xlen)=0;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+yplus*xlen)!=0 && (*(statex+x+yplus*xlen)==0 || *(statey+x+yplus*xlen)==0))
			//
			// However, since the current height decreased, the condition *(statex+x+yplus*xlen)==0 is
			// never satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+yplus*xlen)!=0 && *(statey+x+yplus*xlen)==0)
			{
				*(index_out+*(map_out+x+yplus*xlen))=0;
				holes_out[holes_out_count++]=*(map_out+x+yplus*xlen);
				*(map_out+x+yplus*xlen)=0;
			}
		}

		// Fill in holes in the active spins array for heights that can increase, if necessary.		
		holes_out_count--;
		while (holes_out_count>=0)
		{
			if (*(index_out+*index_out)!=0 || (*(index_out+*index_out)==0 && *map_out==*index_out))
			{
				*(index_out+holes_out[holes_out_count])=*(index_out+*index_out);
				*(map_out+*(index_out+*index_out))=holes_out[holes_out_count];
				*(index_out+*index_out)=0;
				holes_out[holes_out_count]=0;
			}
			else if (holes_out[0]==*index_out) holes_out[0]=holes_out[1];
			
			(*index_out)--;
			holes_out_count--;

			if (*index_out==0) break;
		}
		holes_out_count=0;

		// Fill in holes in the active spins array for heights that can decrease, if necessary.		
		holes_in_count--;
		while (holes_in_count>=0)
		{
			if (*(index_in+*index_in)!=0 || (*(index_in+*index_in)==0 && *map_in==*index_in))
			{
				*(index_in+holes_in[holes_in_count])=*(index_in+*index_in);
				*(map_in+*(index_in+*index_in))=holes_in[holes_in_count];
				*(index_in+*index_in)=0;
				holes_in[holes_in_count]=0;
			}
			else if (holes_in[0]==*index_in) holes_in[0]=holes_in[1];
			
			(*index_in)--;
			holes_in_count--;

			if (*index_in==0) break;
		}
		holes_in_count=0;
	
		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}

		parameters->move=0;
	}
	while (evolve_continue==1 && 
		(*index_out>0 || (fullbias==0 && *index_in>0)) &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));
	//
	// The simulation stops if:
	// 	- the # of active spins is zero,
	// 	- the number of steps exceeds the maximum allowed number of steps, or
	// 	- the time exceeds the maximum allowed time.

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int runbias (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	short int fullbias=parameters->fullbias;
	double prob_outer_corner_flips=parameters->prob_outer_corner_flips;
	double prob_inner_corner_flips=1-prob_outer_corner_flips;
	unsigned long int steps=parameters->steps;
	int *height_initial=parameters->height_initial;
	int *height=parameters->height;
	int *statex_initial=parameters->statex_initial;
	int *statex=parameters->statex;
	int *statey_initial=parameters->statey_initial;
	int *statey=parameters->statey;
	int *map_out=parameters->map_out;
	int *index_out=parameters->index_out;
	int *map_in=parameters->map_in;
	int *index_in=parameters->index_in;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, i, j;

	int x, y;

	int xminus;
	int xplus;
	int yminus;
	int yplus;

	int area=xlen*ylen;

	int holes_out[2]={};
	int holes_out_count=0;
	int holes_in[2]={};
	int holes_in_count=0;
	
	for (i=0; i<xlen*ylen; i++)
	{
		*(index_out+i)=0;
		*(map_out+i)=0;
		*(index_in+i)=0;
		*(map_in+i)=0;
	}

	// Determine which spins are active.
	for (i=0; i<xlen*ylen; i++)
	{
		x=i%xlen;
		y=i/xlen;
		
		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==xlen-1) xplus=0;
		else xplus=x+1;
		if (y==ylen-1) yplus=0;
		else yplus=y+1;

		// Check if the current height can increase.
		if (*(statex_initial+x+y*xlen)>0 && *(statey_initial+x+y*xlen)>0)
		{
			(*index_out)++;
			*(index_out+*index_out)=x+y*xlen;
			*(map_out+x+y*xlen)=*index_out;
		}

		// Check if the current height can decrease.
//		if (*(statex_initial+xplus+y*xlen)>0 && *(statey_initial+x+yplus*xlen)>0)
//		{
//			(*index_in)++;
//			*(index_in+*index_in)=x+y*xlen;
//			*(map_in+x+y*xlen)=*index_in;
//		}
	}

	memcpy (height, height_initial, sizeof(int)*xlen*ylen);
	memcpy (statex, statex_initial, sizeof(int)*xlen*ylen);
	memcpy (statey, statey_initial, sizeof(int)*xlen*ylen);

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;
	
	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	// If the numbers of outer and inner corners are both zero, then there is nothing to do.
	if (*index_out==0 && *index_in==0) return 0;

	// If only outer corners can flip, then there must be at least one outer corner.
	if (fullbias==1 && *index_out==0) return 0;
	
	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/((*index_out)*prob_outer_corner_flips));

		// This random number is used to determine whether the flipped spin is an outer or inner corner.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_out)*random_number);
			
			a=*(index_out+j);

			x=a%xlen;
			y=a/xlen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			// Increase the selected height.
			// If the height is already the maximum, then set the new height to zero.
			if (*(height+a)<hlen-1) (*(height+a))++;
			else *(height+a)=0;
			
			(*(statex+a))--;
			(*(statey+a))--;

			(*(statex+xplus+y*xlen))++;
			(*(statey+x+yplus*xlen))++;

			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=2;

			// If the flipped spin became active, enable it.
//			if (*(map_in+a)==0)
//			{
//				(*index_in)++;
//				*(index_in+*index_in)=a;
//				*(map_in+a)=*index_in;
//			}
			// If the flipped spin became inactive, disable it.
			if (*(statex+a)==0 || *(statey+a)==0)
			{
				*(index_out+j)=0;
				holes_out[holes_out_count++]=j;
				*(map_out+a)=0;
			}

			// If either of the two neighboring spins at (xplus, y) or (x, yplus) became active,
			// enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_out+xplus+y*xlen)==0 && *(statex+xplus+y*xlen)>0 && *(statey+xplus+y*xlen)>0)
			//
			// However, since the current height increased, the condition *(statex+xplus+y*xlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+xplus+y*xlen)==0 && *(statey+xplus+y*xlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=xplus+y*xlen;
				*(map_out+xplus+y*xlen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+yplus*xlen)==0 && *(statex+x+yplus*xlen)>0 && *(statey+x+yplus*xlen)>0)
			//
			// However, since the current height increased, the condition *(statey+x+yplus*xlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+yplus*xlen)==0 && *(statex+x+yplus*xlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=x+yplus*xlen;
				*(map_out+x+yplus*xlen)=*index_out;
			}

			// If either of the two neighboring spins at (xminus, y) or (x, yminus) became inactive,
			// disable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_in+xminus+y*xlen)!=0 && (*(statex+x+y*xlen)==0 || *(statey+xminus+yplus*xlen)==0))
			//
			// However, since the current height increased, the condition *(statey+xminus+yplus*xlen)==0 is
			// never satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
//			if (*(map_in+xminus+y*xlen)!=0 && *(statex+x+y*xlen)==0)
//			{
//				*(index_in+*(map_in+xminus+y*xlen))=0;
//				holes_in[holes_in_count++]=*(map_in+xminus+y*xlen);
//				*(map_in+xminus+y*xlen)=0;
//			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+yminus*xlen)!=0 && (*(statex+xplus+yminus*xlen)==0 || *(statey+x+y*xlen)==0))
			//
			// However, since the current height increased, the condition *(statex+xplus+yminus*xlen)==0 is
			// never satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
//			if (*(map_in+x+yminus*xlen)!=0 && *(statey+x+y*xlen)==0)
//			{
//				*(index_in+*(map_in+x+yminus*xlen))=0;
//				holes_in[holes_in_count++]=*(map_in+x+yminus*xlen);
//				*(map_in+x+yminus*xlen)=0;
//			}

		// Fill in holes in the active spins array for heights that can increase, if necessary.		
		holes_out_count--;
		while (holes_out_count>=0)
		{
			if (*(index_out+*index_out)!=0 || (*(index_out+*index_out)==0 && *map_out==*index_out))
			{
				*(index_out+holes_out[holes_out_count])=*(index_out+*index_out);
				*(map_out+*(index_out+*index_out))=holes_out[holes_out_count];
				*(index_out+*index_out)=0;
				holes_out[holes_out_count]=0;
			}
			else if (holes_out[0]==*index_out) holes_out[0]=holes_out[1];
			
			(*index_out)--;
			holes_out_count--;

			if (*index_out==0) break;
		}
		holes_out_count=0;

		// Fill in holes in the active spins array for heights that can decrease, if necessary.		
//		holes_in_count--;
//		while (holes_in_count>=0)
//		{
//			if (*(index_in+*index_in)!=0 || (*(index_in+*index_in)==0 && *map_in==*index_in))
//			{
//				*(index_in+holes_in[holes_in_count])=*(index_in+*index_in);
//				*(map_in+*(index_in+*index_in))=holes_in[holes_in_count];
//				*(index_in+*index_in)=0;
//				holes_in[holes_in_count]=0;
//			}
//			else if (holes_in[0]==*index_in) holes_in[0]=holes_in[1];
//			
//			(*index_in)--;
//			holes_in_count--;
//
//			if (*index_in==0) break;
//		}
//		holes_in_count=0;
	
		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}

		parameters->move=0;
	}
	while (evolve_continue==1 && 
		*index_out>0 &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));
	//
	// The simulation stops if:
	// 	- the # of active spins is zero,
	// 	- the number of steps exceeds the maximum allowed number of steps, or
	// 	- the time exceeds the maximum allowed time.

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int run4d (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_4d *parameters)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	short int hlen=parameters->hlen;
	short int xlen=parameters->xlen;
	short int ylen=parameters->ylen;
	short int qlen=parameters->qlen;
	short int fullbias=parameters->fullbias;
	double prob_outer_corner_flips=parameters->prob_outer_corner_flips;
	double prob_inner_corner_flips=1-prob_outer_corner_flips;
	unsigned long int steps=parameters->steps;
	short int *height_initial=parameters->height_initial;
	short int *height=parameters->height;
	short int *statex_initial=parameters->statex_initial;
	short int *statex=parameters->statex;
	short int *statey_initial=parameters->statey_initial;
	short int *statey=parameters->statey;
	short int *stateq_initial=parameters->stateq_initial;
	short int *stateq=parameters->stateq;
	int *map_out=parameters->map_out;
	int *index_out=parameters->index_out;
	int *map_in=parameters->map_in;
	int *index_in=parameters->index_in;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, i, j;

	int x, y, q;

	int xminus;
	int xplus;
	int yminus;
	int yplus;
	int qminus;
	int qplus;

	int xylen=xlen*ylen;

	int holes_out[3]={};
	int holes_out_count=0;
	int holes_in[3]={};
	int holes_in_count=0;
	
	for (i=0; i<xlen*ylen*qlen; i++)
	{
		*(index_out+i)=0;
		*(map_out+i)=0;
		*(index_in+i)=0;
		*(map_in+i)=0;
	}

	// Determine which spins are active.
	for (i=0; i<xlen*ylen*qlen; i++)
	{
		x=i%xlen;
		y=(i/xlen)%ylen;
		q=i/xylen;
		
		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==xlen-1) xplus=0;
		else xplus=x+1;
		if (y==ylen-1) yplus=0;
		else yplus=y+1;
		if (q==qlen-1) qplus=0;
		else qplus=q+1;

		// Check if the current height can increase.
		if (*(statex_initial+x+y*xlen+q*xylen)>0 && *(statey_initial+x+y*xlen+q*xylen)>0 && *(stateq_initial+x+y*xlen+q*xylen)>0)
		{
			(*index_out)++;
			*(index_out+*index_out)=x+y*xlen+q*xylen;
			*(map_out+x+y*xlen+q*xylen)=*index_out;
		}

		// Check if the current height can decrease.
		if (*(statex_initial+xplus+y*xlen+q*xylen)>0 && *(statey_initial+x+yplus*xlen+q*xylen)>0 && *(stateq_initial+x+y*xlen+qplus*xylen)>0)
		{
			(*index_in)++;
			*(index_in+*index_in)=x+y*xlen+q*xylen;
			*(map_in+x+y*xlen+q*xylen)=*index_in;
		}
	}

	memcpy (height, height_initial, sizeof(short int)*xlen*ylen*qlen);
	memcpy (statex, statex_initial, sizeof(short int)*xlen*ylen*qlen);
	memcpy (statey, statey_initial, sizeof(short int)*xlen*ylen*qlen);
	memcpy (stateq, stateq_initial, sizeof(short int)*xlen*ylen*qlen);

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;
	
	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	// If the numbers of outer and inner corners are both zero, then there is nothing to do.
	if (*index_out==0 && *index_in==0) return 0;

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/((*index_out)*prob_outer_corner_flips+(*index_in)*prob_inner_corner_flips));
											
		// This random number is used to determine whether the flipped spin is an outer or inner corner.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		
		if (fullbias==1 || random_number<(*index_out)*prob_outer_corner_flips/((*index_out)*prob_outer_corner_flips+(*index_in)*prob_inner_corner_flips))
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_out)*random_number);
			
			a=*(index_out+j);

			x=a%xlen;
			y=(a/xlen)%ylen;
			q=a/xylen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;
			if (q==0) qminus=qlen-1;
			else qminus=q-1;
			if (q==qlen-1) qplus=0;
			else qplus=q+1;

			// Increase the selected height.
			// If the height is already the maximum, then set the new height to zero.
			if (*(height+a)<hlen-1) (*(height+a))++;
			else *(height+a)=0;
			
			(*(statex+a))--;
			(*(statey+a))--;
			(*(stateq+a))--;

			(*(statex+xplus+y*xlen+q*xylen))++;
			(*(statey+x+yplus*xlen+q*xylen))++;
			(*(stateq+x+y*xlen+qplus*xylen))++;

			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=2;

			// If the flipped spin became active, enable it.
			if (*(map_in+a)==0)
			{
				(*index_in)++;
				*(index_in+*index_in)=a;
				*(map_in+a)=*index_in;
			}
			// If the flipped spin became inactive, disable it.
			if (*(statex+a)==0 || *(statey+a)==0 || *(stateq+a)==0)
			{
				*(index_out+j)=0;
				holes_out[holes_out_count++]=j;
				*(map_out+a)=0;
			}

			// If any one of the three neighboring spins at (xplus, y, q), (x, yplus, q), or
			// (x, y, qplus) became active, enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_out+xplus+y*xlen+q*xylen)==0 && *(statex+xplus+y*xlen+q*xylen)>0 && *(statey+xplus+y*xlen+q*xylen)>0 && *(stateq+xplus+y*xlen+q*xylen)>0)
			//
			// However, since the current height increased, the condition *(statex+xplus+y*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+xplus+y*xlen+q*xylen)==0 && *(statey+xplus+y*xlen+q*xylen)>0 && *(stateq+xplus+y*xlen+q*xylen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=xplus+y*xlen+q*xylen;
				*(map_out+xplus+y*xlen+q*xylen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+yplus*xlen+q*xylen)==0 && *(statex+x+yplus*xlen+q*xylen)>0 && *(statey+x+yplus*xlen+q*xylen)>0 && *(stateq+x+yplus*xlen+q*xylen)>0)
			//
			// However, since the current height increased, the condition *(statey+x+yplus*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+yplus*xlen+q*xylen)==0 && *(statex+x+yplus*xlen+q*xylen)>0 && *(stateq+x+yplus*xlen+q*xylen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=x+yplus*xlen+q*xylen;
				*(map_out+x+yplus*xlen+q*xylen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+y*xlen+qplus*xylen)==0 && *(statex+x+y*xlen+qplus*xylen)>0 && *(statey+x+y*xlen+qplus*xylen)>0 && *(stateq+x+y*xlen+qplus*xylen)>0)
			//
			// However, since the current height increased, the condition *(stateq+x+y*xlen+qplus*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+y*xlen+qplus*xylen)==0 && *(statex+x+y*xlen+qplus*xylen)>0 && *(statey+x+y*xlen+qplus*xylen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=x+y*xlen+qplus*xylen;
				*(map_out+x+y*xlen+qplus*xylen)=*index_out;
			}

			// If any one of the three neighboring spins at (xminus, y, q), (x, yminus, q), or
			// (x, y, qminus) became inactive, disable it.
			if (*(map_in+xminus+y*xlen+q*xylen)!=0 && (*(statex+x+y*xlen+q*xylen)==0 || *(statey+xminus+yplus*xlen+q*xylen)==0 || *(stateq+xminus+y*xlen+qplus*xylen)==0))
			{
				*(index_in+*(map_in+xminus+y*xlen+q*xylen))=0;
				holes_in[holes_in_count++]=*(map_in+xminus+y*xlen+q*xylen);
				*(map_in+xminus+y*xlen+q*xylen)=0;
			}
			if (*(map_in+x+yminus*xlen+q*xylen)!=0 && (*(statex+xplus+yminus*xlen+q*xylen)==0 || *(statey+x+y*xlen+q*xylen)==0 || *(stateq+x+yminus*xlen+qplus*xylen)==0))
			{
				*(index_in+*(map_in+x+yminus*xlen+q*xylen))=0;
				holes_in[holes_in_count++]=*(map_in+x+yminus*xlen+q*xylen);
				*(map_in+x+yminus*xlen+q*xylen)=0;
			}
			if (*(map_in+x+y*xlen+qminus*xylen)!=0 && (*(statex+xplus+y*xlen+qminus*xylen)==0 || *(statey+x+yplus*xlen+qminus*xylen)==0 || *(stateq+x+y*xlen+q*xylen)==0))
			{
				*(index_in+*(map_in+x+y*xlen+qminus*xylen))=0;
				holes_in[holes_in_count++]=*(map_in+x+y*xlen+qminus*xylen);
				*(map_in+x+y*xlen+qminus*xylen)=0;
			}
		}
		else
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_in)*random_number);
			
			a=*(index_in+j);

			x=a%xlen;
			y=(a/xlen)%ylen;
			q=a/xylen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;
			if (q==0) qminus=qlen-1;
			else qminus=q-1;
			if (q==qlen-1) qplus=0;
			else qplus=q+1;

			// Decrease the selected height.
			// If the height is already the minimum, then set the new height to dh*units-1.
			if (*(height+a)>0) (*(height+a))--;
			else *(height+a)=hlen-1;
			
			(*(statex+a))++;
			(*(statey+a))++;
			(*(stateq+a))++;

			(*(statex+xplus+y*xlen+q*xylen))--;
			(*(statey+x+yplus*xlen+q*xylen))--;
			(*(stateq+x+y*xlen+qplus*xylen))--;

			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=1;

			// If the flipped spin became active, enable it.
			if (*(map_out+a)==0)
			{
				(*index_out)++;
				*(index_out+*index_out)=a;
				*(map_out+a)=*index_out;
			}
			// If the flipped spin became inactive, disable it.
			if (*(statex+xplus+y*xlen+q*xylen)==0 || *(statey+x+yplus*xlen+q*xylen)==0 || *(stateq+x+y*xlen+qplus*xylen)==0)
			{
				*(index_in+j)=0;
				holes_in[holes_in_count++]=j;
				*(map_in+a)=0;
			}

			// If any one of the three neighboring spins at (xminus, y, q), (x, yminus, q), or
			// (x, y, qminus) became active, enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_in+xminus+y*xlen+q*xylen)==0 && *(statex+x+y*xlen+q*xylen)>0 && *(statey+xminus+yplus*xlen+q*xylen)>0 && *(stateq+xminus+y*xlen+qplus*xylen)>0)
			//
			// However, since the current height decreased, the condition *(statex+x+y*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+xminus+y*xlen+q*xylen)==0 && *(statey+xminus+yplus*xlen+q*xylen)>0 && *(stateq+xminus+y*xlen+qplus*xylen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=xminus+y*xlen+q*xylen;
				*(map_in+xminus+y*xlen+q*xylen)=*index_in;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+yminus*xlen+q*xylen)==0 && *(statex+xplus+yminus*xlen+q*xylen)>0 && *(statey+x+y*xlen+q*xylen)>0 && *(stateq+x+yminus*xlen+qplus*xylen)>0)
			//
			// However, since the current height decreased, the condition *(statey+x+y*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+x+yminus*xlen+q*xylen)==0 && *(statex+xplus+yminus*xlen+q*xylen)>0 && *(stateq+x+yminus*xlen+qplus*xylen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=x+yminus*xlen+q*xylen;
				*(map_in+x+yminus*xlen+q*xylen)=*index_in;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+y*xlen+qminus*xylen)==0 && *(statex+xplus+y*xlen+qminus*xylen)>0 && *(statey+x+yplus*xlen+qminus*xylen)>0 && *(stateq+x+y*xlen+q*xylen)>0)
			//
			// However, since the current height decreased, the condition *(stateq+x+y*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+x+y*xlen+qminus*xylen)==0 && *(statex+xplus+y*xlen+qminus*xylen)>0 && *(statey+x+yplus*xlen+qminus*xylen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=x+y*xlen+qminus*xylen;
				*(map_in+x+y*xlen+qminus*xylen)=*index_in;
			}

			// If any one of the three neighboring spins at (xplus, y, q), (x, yplus, q), or
			// (x, y, qplus) became inactive, disable it.
			if (*(map_out+xplus+y*xlen+q*xylen)!=0 && (*(statex+xplus+y*xlen+q*xylen)==0 || *(statey+xplus+y*xlen+q*xylen)==0 || *(stateq+xplus+y*xlen+q*xylen)==0))
			{
				*(index_out+*(map_out+xplus+y*xlen+q*xylen))=0;
				holes_out[holes_out_count++]=*(map_out+xplus+y*xlen+q*xylen);
				*(map_out+xplus+y*xlen+q*xylen)=0;
			}
			if (*(map_out+x+yplus*xlen+q*xylen)!=0 && (*(statex+x+yplus*xlen+q*xylen)==0 || *(statey+x+yplus*xlen+q*xylen)==0 || *(stateq+x+yplus*xlen+q*xylen)==0))
			{
				*(index_out+*(map_out+x+yplus*xlen+q*xylen))=0;
				holes_out[holes_out_count++]=*(map_out+x+yplus*xlen+q*xylen);
				*(map_out+x+yplus*xlen+q*xylen)=0;
			}
			if (*(map_out+x+y*xlen+qplus*xylen)!=0 && (*(statex+x+y*xlen+qplus*xylen)==0 || *(statey+x+y*xlen+qplus*xylen)==0 || *(stateq+x+y*xlen+qplus*xylen)==0))
			{
				*(index_out+*(map_out+x+y*xlen+qplus*xylen))=0;
				holes_out[holes_out_count++]=*(map_out+x+y*xlen+qplus*xylen);
				*(map_out+x+y*xlen+qplus*xylen)=0;
			}
		}

		// Fill in holes in the active spins array for heights that can increase, if necessary.		
		holes_out_count--;
		while (holes_out_count>=0)
		{
			if (*(index_out+*index_out)!=0 || (*(index_out+*index_out)==0 && *map_out==*index_out))
			{
				*(index_out+holes_out[holes_out_count])=*(index_out+*index_out);
				*(map_out+*(index_out+*index_out))=holes_out[holes_out_count];
				*(index_out+*index_out)=0;
				holes_out[holes_out_count]=0;
			}
			else if (holes_out[0]==*index_out) holes_out[0]=holes_out[1];
			
			(*index_out)--;
			holes_out_count--;

			if (*index_out==0) break;
		}
		holes_out_count=0;

		// Fill in holes in the active spins array for heights that can decrease, if necessary.		
		holes_in_count--;
		while (holes_in_count>=0)
		{
			if (*(index_in+*index_in)!=0 || (*(index_in+*index_in)==0 && *map_in==*index_in))
			{
				*(index_in+holes_in[holes_in_count])=*(index_in+*index_in);
				*(map_in+*(index_in+*index_in))=holes_in[holes_in_count];
				*(index_in+*index_in)=0;
				holes_in[holes_in_count]=0;
			}
			else if (holes_in[0]==*index_in) holes_in[0]=holes_in[1];
			
			(*index_in)--;
			holes_in_count--;

			if (*index_in==0) break;
		}
		holes_in_count=0;

		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}

		parameters->move=0;
	}
	while (evolve_continue==1 && 
		(*index_out>0 || (fullbias==0 && *index_in>0)) &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));
	//
	// The simulation stops if:
	// 	- the # of active spins is zero,
	// 	- the number of steps exceeds the maximum allowed number of steps, or
	// 	- the time exceeds the maximum allowed time.

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int run4d2 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_4d *parameters)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	short int hlen=parameters->hlen;
	short int xlen=parameters->xlen;
	short int ylen=parameters->ylen;
	short int qlen=parameters->qlen;
	short int fullbias=parameters->fullbias;
	double prob_outer_corner_flips=parameters->prob_outer_corner_flips;
	double prob_inner_corner_flips=1-prob_outer_corner_flips;
	unsigned long int steps=parameters->steps;
	short int *height=parameters->height;
	int *map_out=parameters->map_out;
	int *index_out=parameters->index_out;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, i, j;

	int x, y, q;

	int xminus;
	int xplus;
	int yminus;
	int yplus;
	int qminus;
	int qplus;

	int xylen=xlen*ylen;

	int holes_out[3]={};
	int holes_out_count=0;
	int holes_in[3]={};
	int holes_in_count=0;
	
	for (i=0; i<xlen*ylen*qlen; i++)
	{
		*(index_out+i)=0;
		*(map_out+i)=0;
	}

	// There is initially only one active spin.
	(*index_out)++;
	*(index_out+*index_out)=0;
	*map_out=*index_out;
	
	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;
	
	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	// If the numbers of outer and inner corners are both zero, then there is nothing to do.
	if (*index_out==0) return 0;

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/((*index_out)*prob_outer_corner_flips));
											
		// This random number is used to determine whether the flipped spin is an outer or inner corner.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		
		if (fullbias==1)
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_out)*random_number);
			
			a=*(index_out+j);

			x=a%xlen;
			y=(a/xlen)%ylen;
			q=a/xylen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;
			if (q==0) qminus=qlen-1;
			else qminus=q-1;
			if (q==qlen-1) qplus=0;
			else qplus=q+1;

			// Increase the selected height.
			// If the height is already the maximum, then set the new height to zero.
			if (*(height+a)<hlen-1) (*(height+a))++;
			else *(height+a)=0;
			
			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=2;

			// If the flipped spin became inactive, disable it.
			if ((xminus<xlen-1 && *(height+a)==*(height+xminus+y*xlen+q*xylen)) || (yminus<ylen-1 && *(height+a)==*(height+x+yminus*xlen+q*xylen)) || (qminus<qlen-1 && *(height+a)==*(height+x+y*xlen+qminus*xylen)))
			{
				*(index_out+j)=0;
				holes_out[holes_out_count++]=j;
				*(map_out+a)=0;
			}

			// If any one of the three neighboring spins at (xplus, y, q), (x, yplus, q), or
			// (x, y, qplus) became active, enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_out+xplus+y*xlen+q*xylen)==0 && *(statex+xplus+y*xlen+q*xylen)>0 && *(statey+xplus+y*xlen+q*xylen)>0 && *(stateq+xplus+y*xlen+q*xylen)>0)
			//
			// However, since the current height increased, the condition *(statex+xplus+y*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+xplus+y*xlen+q*xylen)==0 && (yminus==ylen-1 || *(height+xplus+y*xlen+q*xylen)<*(height+xplus+yminus*xlen+q*xylen)) && (qminus==qlen-1 || *(height+xplus+y*xlen+q*xylen)<*(height+xplus+y*xlen+qminus*xylen)))
			{
				(*index_out)++;
				*(index_out+*index_out)=xplus+y*xlen+q*xylen;
				*(map_out+xplus+y*xlen+q*xylen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+yplus*xlen+q*xylen)==0 && *(statex+x+yplus*xlen+q*xylen)>0 && *(statey+x+yplus*xlen+q*xylen)>0 && *(stateq+x+yplus*xlen+q*xylen)>0)
			//
			// However, since the current height increased, the condition *(statey+x+yplus*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+yplus*xlen+q*xylen)==0 && (xminus==xlen-1 || *(height+x+yplus*xlen+q*xylen)<*(height+xminus+yplus*xlen+q*xylen)) && (qminus==qlen-1 || *(height+x+yplus*xlen+q*xylen)<*(height+x+yplus*xlen+qminus*xylen)))
			{
				(*index_out)++;
				*(index_out+*index_out)=x+yplus*xlen+q*xylen;
				*(map_out+x+yplus*xlen+q*xylen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+y*xlen+qplus*xylen)==0 && *(statex+x+y*xlen+qplus*xylen)>0 && *(statey+x+y*xlen+qplus*xylen)>0 && *(stateq+x+y*xlen+qplus*xylen)>0)
			//
			// However, since the current height increased, the condition *(stateq+x+y*xlen+qplus*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+y*xlen+qplus*xylen)==0 && (xminus==xlen-1 || *(height+x+y*xlen+qplus*xylen)<*(height+xminus+y*xlen+qplus*xylen)) && (yminus==ylen-1 || *(height+x+y*xlen+qplus*xylen)<*(height+x+yminus*xlen+qplus*xylen)))
			{
				(*index_out)++;
				*(index_out+*index_out)=x+y*xlen+qplus*xylen;
				*(map_out+x+y*xlen+qplus*xylen)=*index_out;
			}
		}

		// Fill in holes in the active spins array for heights that can increase, if necessary.		
		holes_out_count--;
		while (holes_out_count>=0)
		{
			if (*(index_out+*index_out)!=0 || (*(index_out+*index_out)==0 && *map_out==*index_out))
			{
				*(index_out+holes_out[holes_out_count])=*(index_out+*index_out);
				*(map_out+*(index_out+*index_out))=holes_out[holes_out_count];
				*(index_out+*index_out)=0;
				holes_out[holes_out_count]=0;
			}
			else if (holes_out[0]==*index_out) holes_out[0]=holes_out[1];
			
			(*index_out)--;
			holes_out_count--;

			if (*index_out==0) break;
		}
		holes_out_count=0;

		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}

		parameters->move=0;
	}
	while (evolve_continue==1 && 
		*index_out>0 &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));
	//
	// The simulation stops if:
	// 	- the # of active spins is zero,
	// 	- the number of steps exceeds the maximum allowed number of steps, or
	// 	- the time exceeds the maximum allowed time.

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int run4d3 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_4d3 *parameters)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	short int hlen=parameters->hlen;
	short int xlen=parameters->xlen;
	short int ylen=parameters->ylen;
	short int qlen=parameters->qlen;
	short int fullbias=parameters->fullbias;
	double prob_outer_corner_flips=parameters->prob_outer_corner_flips;
	double prob_inner_corner_flips=1-prob_outer_corner_flips;
	unsigned long int steps=parameters->steps;
	short int *height=parameters->height;
	int *map_out=parameters->map_out;
	unsigned long int *index_out=parameters->index_out;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	unsigned long int a, i, j;
	
	unsigned long int x, y, q;

	unsigned long int length=xlen;
	unsigned long int offset, offset_yplus, offset_yminus, offset_qplus, offset_qminus, offset_yplus_qminus, offset_yminus_qplus;
	
	unsigned long int xminus;
	unsigned long int xplus;
	unsigned long int yminus;
	unsigned long int yplus;
	unsigned long int qminus;
	unsigned long int qplus;

	unsigned long int xylen=xlen*ylen;

	int holes_out[3]={};
	int holes_out_count=0;
	int holes_in[3]={};
	int holes_in_count=0;
	
	// This is the total number of heights.
	unsigned long int size=((length-1)*((length-1)-1)*(2*(length-1)-1)/6-(length-1)*((length-1)-1)*(2*length+1)/2+(length-1)*length*(length+1))/2+1;

	for (i=0; i<size; i++)
	{
		*(index_out+i)=0;
		*(map_out+i)=0;
	}

	// There is initially only one active spin.
	(*index_out)++;
	*(index_out+*index_out)=0;
	*map_out=*index_out;
	
	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;
	
	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	// If the numbers of outer and inner corners are both zero, then there is nothing to do.
	if (*index_out==0) return 0;

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/((*index_out)*prob_outer_corner_flips));
											
		// This random number is used to determine whether the flipped spin is an outer or inner corner.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		
		if (fullbias==1)
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_out)*random_number);
			
			a=*(index_out+j);

			x=a%xlen;
			y=(a/xlen)%ylen;
			q=a/xylen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;
			if (q==0) qminus=qlen-1;
			else qminus=q-1;
			if (q==qlen-1) qplus=0;
			else qplus=q+1;

			// Set the offsets.
			offset=(q*(q-1)*(2*q-1)/6-q*(q-1)*(2*length+1)/2+q*length*(length+1))/2+y*(length-q)+(y-1)*y/2+x;
			offset_yminus=(q*(q-1)*(2*q-1)/6-q*(q-1)*(2*length+1)/2+q*length*(length+1))/2+yminus*(length-q)+(yminus-1)*yminus/2+x;
			offset_yplus=(q*(q-1)*(2*q-1)/6-q*(q-1)*(2*length+1)/2+q*length*(length+1))/2+yplus*(length-q)+(yplus-1)*yplus/2+x;
			offset_qminus=(qminus*(qminus-1)*(2*qminus-1)/6-qminus*(qminus-1)*(2*length+1)/2+qminus*length*(length+1))/2+y*(length-qminus)+(y-1)*y/2+x;
			offset_qplus=(qplus*(qplus-1)*(2*qplus-1)/6-qplus*(qplus-1)*(2*length+1)/2+qplus*length*(length+1))/2+y*(length-qplus)+(y-1)*y/2+x;
			offset_yplus_qminus=(qminus*(qminus-1)*(2*qminus-1)/6-qminus*(qminus-1)*(2*length+1)/2+qminus*length*(length+1))/2+yplus*(length-qminus)+(yplus-1)*yplus/2+x;
			offset_yminus_qplus=(qplus*(qplus-1)*(2*qplus-1)/6-qplus*(qplus-1)*(2*length+1)/2+qplus*length*(length+1))/2+yminus*(length-qplus)+(yminus-1)*yminus/2+x;

			// Increase the selected height.
			// If the height is already the maximum, then set the new height to zero.
			if (*(height+offset)<hlen-1) (*(height+offset))++;
			else *(height+offset)=0;
			
			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=2;

			// If the flipped spin became inactive, disable it.
			if ((xminus<xlen-1 && *(height+offset)==*(height+offset-x+xminus)) || (yminus<ylen-1 && *(height+offset)==*(height+offset_yminus)) || (qminus<qlen-1 && *(height+offset)==*(height+offset_qminus)))
			{
				*(index_out+j)=0;
				holes_out[holes_out_count++]=j;
				*(map_out+offset)=0;
			}

			// If any one of the three neighboring spins at (xplus, y, q), (x, yplus, q), or
			// (x, y, qplus) became active, enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_out+xplus+y*xlen+q*xylen)==0 && *(statex+xplus+y*xlen+q*xylen)>0 && *(statey+xplus+y*xlen+q*xylen)>0 && *(stateq+xplus+y*xlen+q*xylen)>0)
			//
			// However, since the current height increased, the condition *(statex+xplus+y*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+offset-x+xplus)==0 && (yminus==ylen-1 || *(height+offset-x+xplus)<*(height+offset_yminus-x+xplus)) && (qminus==qlen-1 || *(height+offset-x+xplus)<*(height+offset_qminus-x+xplus)))
			{
				(*index_out)++;
				*(index_out+*index_out)=xplus+y*xlen+q*xylen;
				*(map_out+offset-x+xplus)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+yplus*xlen+q*xylen)==0 && *(statex+x+yplus*xlen+q*xylen)>0 && *(statey+x+yplus*xlen+q*xylen)>0 && *(stateq+x+yplus*xlen+q*xylen)>0)
			//
			// However, since the current height increased, the condition *(statey+x+yplus*xlen+q*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+offset_yplus)==0 && (xminus==xlen-1 || *(height+offset_yplus)<*(height+offset_yplus-x+xminus)) && (qminus==qlen-1 || *(height+offset_yplus)<*(height+offset_yplus_qminus)))
			{
				(*index_out)++;
				*(index_out+*index_out)=x+yplus*xlen+q*xylen;
				*(map_out+offset_yplus)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+y*xlen+qplus*xylen)==0 && *(statex+x+y*xlen+qplus*xylen)>0 && *(statey+x+y*xlen+qplus*xylen)>0 && *(stateq+x+y*xlen+qplus*xylen)>0)
			//
			// However, since the current height increased, the condition *(stateq+x+y*xlen+qplus*xylen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+offset_qplus)==0 && (xminus==xlen-1 || *(height+offset_qplus)<*(height+offset_qplus-x+xminus)) && (yminus==ylen-1 || *(height+offset_qplus)<*(height+offset_yminus_qplus)))
			{
				(*index_out)++;
				*(index_out+*index_out)=x+y*xlen+qplus*xylen;
				*(map_out+offset_qplus)=*index_out;
			}
		}

		// Fill in holes in the active spins array for heights that can increase, if necessary.		
		holes_out_count--;
		while (holes_out_count>=0)
		{
			if (*(index_out+*index_out)!=0 || (*(index_out+*index_out)==0 && *map_out==*index_out))
			{
				*(index_out+holes_out[holes_out_count])=*(index_out+*index_out);
				
				x=(*(index_out+*index_out))%xlen;
				y=((*(index_out+*index_out))/xlen)%ylen;
				q=(*(index_out+*index_out))/xylen;
				
				offset=(q*(q-1)*(2*q-1)/6-q*(q-1)*(2*length+1)/2+q*length*(length+1))/2+y*(length-q)+(y-1)*y/2+x;
				
				*(map_out+offset)=holes_out[holes_out_count];
				*(index_out+*index_out)=0;
				holes_out[holes_out_count]=0;
			}
			else if (holes_out[0]==*index_out) holes_out[0]=holes_out[1];
			
			(*index_out)--;
			holes_out_count--;

			if (*index_out==0) break;
		}
		holes_out_count=0;

		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}

		parameters->move=0;
	}
	while (evolve_continue==1 && 
		*index_out>0 &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));
	//
	// The simulation stops if:
	// 	- the # of active spins is zero,
	// 	- the number of steps exceeds the maximum allowed number of steps, or
	// 	- the time exceeds the maximum allowed time.

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int run5d (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters_5d *parameters)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	short int hlen=parameters->hlen;
	short int xlen=parameters->xlen;
	short int ylen=parameters->ylen;
	short int qlen=parameters->qlen;
	short int rlen=parameters->rlen;
	short int fullbias=parameters->fullbias;
	double prob_outer_corner_flips=parameters->prob_outer_corner_flips;
	double prob_inner_corner_flips=1-prob_outer_corner_flips;
	unsigned long int steps=parameters->steps;
	short int *height_initial=parameters->height_initial;
	short int *height=parameters->height;
	short int *statex_initial=parameters->statex_initial;
	short int *statex=parameters->statex;
	short int *statey_initial=parameters->statey_initial;
	short int *statey=parameters->statey;
	short int *stateq_initial=parameters->stateq_initial;
	short int *stateq=parameters->stateq;
	short int *stater_initial=parameters->stater_initial;
	short int *stater=parameters->stater;
	int *map_out=parameters->map_out;
	int *index_out=parameters->index_out;
	int *map_in=parameters->map_in;
	int *index_in=parameters->index_in;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, i, j;

	int x, y, q, r;

	int xminus;
	int xplus;
	int yminus;
	int yplus;
	int qminus;
	int qplus;
	int rminus;
	int rplus;

	int xylen=xlen*ylen;
	int xyqlen=xlen*ylen*qlen;

	int holes_out[4]={};
	int holes_out_count=0;
	int holes_in[4]={};
	int holes_in_count=0;
	
	for (i=0; i<xlen*ylen*qlen*rlen; i++)
	{
		*(index_out+i)=0;
		*(map_out+i)=0;
		*(index_in+i)=0;
		*(map_in+i)=0;
	}

	// Determine which spins are active.
	for (i=0; i<xlen*ylen*qlen*rlen; i++)
	{
		x=i%xlen;
		y=(i/xlen)%ylen;
		q=(i/xylen)%qlen;
		r=i/xyqlen;
		
		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==xlen-1) xplus=0;
		else xplus=x+1;
		if (y==ylen-1) yplus=0;
		else yplus=y+1;
		if (q==qlen-1) qplus=0;
		else qplus=q+1;
		if (r==rlen-1) rplus=0;
		else rplus=r+1;

		// Check if the current height can increase.
		if (*(statex_initial+x+y*xlen+q*xylen+r*xyqlen)>0 && *(statey_initial+x+y*xlen+q*xylen+r*xyqlen)>0 && *(stateq_initial+x+y*xlen+q*xylen+r*xyqlen)>0 && *(stater_initial+x+y*xlen+q*xylen+r*xyqlen)>0)
		{
			(*index_out)++;
			*(index_out+*index_out)=x+y*xlen+q*xylen+r*xyqlen;
			*(map_out+x+y*xlen+q*xylen+r*xyqlen)=*index_out;
		}

		// Check if the current height can decrease.
		if (*(statex_initial+xplus+y*xlen+q*xylen+r*xyqlen)>0 && *(statey_initial+x+yplus*xlen+q*xylen+r*xyqlen)>0 && *(stateq_initial+x+y*xlen+qplus*xylen+r*xyqlen)>0 && *(stater_initial+x+y*xlen+q*xylen+rplus*xyqlen)>0)
		{
			(*index_in)++;
			*(index_in+*index_in)=x+y*xlen+q*xylen+r*xyqlen;
			*(map_in+x+y*xlen+q*xylen+r*xyqlen)=*index_in;
		}
	}

	memcpy (height, height_initial, sizeof(short int)*xlen*ylen*qlen*rlen);
	memcpy (statex, statex_initial, sizeof(short int)*xlen*ylen*qlen*rlen);
	memcpy (statey, statey_initial, sizeof(short int)*xlen*ylen*qlen*rlen);
	memcpy (stateq, stateq_initial, sizeof(short int)*xlen*ylen*qlen*rlen);
	memcpy (stater, stater_initial, sizeof(short int)*xlen*ylen*qlen*rlen);

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;
	
	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	// If the numbers of outer and inner corners are both zero, then there is nothing to do.
	if (*index_out==0 && *index_in==0) return 0;

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/((*index_out)*prob_outer_corner_flips+(*index_in)*prob_inner_corner_flips));
											
		// This random number is used to determine whether the flipped spin is an outer or inner corner.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		
		if (fullbias==1 || random_number<(*index_out)*prob_outer_corner_flips/((*index_out)*prob_outer_corner_flips+(*index_in)*prob_inner_corner_flips))
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_out)*random_number);
			
			a=*(index_out+j);

			x=a%xlen;
			y=(a/xlen)%ylen;
			q=(a/xylen)%qlen;
			r=a/xyqlen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;
			if (q==0) qminus=qlen-1;
			else qminus=q-1;
			if (q==qlen-1) qplus=0;
			else qplus=q+1;
			if (r==0) rminus=rlen-1;
			else rminus=r-1;
			if (r==rlen-1) rplus=0;
			else rplus=r+1;

			// Increase the selected height.
			// If the height is already the maximum, then set the new height to zero.
			if (*(height+a)<hlen-1) (*(height+a))++;
			else *(height+a)=0;
			
			(*(statex+a))--;
			(*(statey+a))--;
			(*(stateq+a))--;
			(*(stater+a))--;

			(*(statex+xplus+y*xlen+q*xylen+r*xyqlen))++;
			(*(statey+x+yplus*xlen+q*xylen+r*xyqlen))++;
			(*(stateq+x+y*xlen+qplus*xylen+r*xyqlen))++;
			(*(stater+x+y*xlen+q*xylen+rplus*xyqlen))++;

			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=2;

			// If the flipped spin became active, enable it.
			if (*(map_in+a)==0)
			{
				(*index_in)++;
				*(index_in+*index_in)=a;
				*(map_in+a)=*index_in;
			}
			// If the flipped spin became inactive, disable it.
			if (*(statex+a)==0 || *(statey+a)==0 || *(stateq+a)==0 || *(stater+a)==0)
			{
				*(index_out+j)=0;
				holes_out[holes_out_count++]=j;
				*(map_out+a)=0;
			}

			// If any one of the four neighboring spins at (xplus, y, q, r), (x, yplus, q, r),
			// (x, y, qplus, r), or (x, y, q, rplus) became active, enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_out+xplus+y*xlen+q*xylen+r*xyqlen)==0 && *(statex+xplus+y*xlen+q*xylen+r*xyqlen)>0 && *(statey+xplus+y*xlen+q*xylen+r*xyqlen)>0 && *(stateq+xplus+y*xlen+q*xylen+r*xyqlen)>0 && *(stater+xplus+y*xlen+q*xylen+r*xyqlen)>0)
			//
			// However, since the current height increased, the condition *(statex+xplus+y*xlen+q*xylen+r*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+xplus+y*xlen+q*xylen+r*xyqlen)==0 && *(statey+xplus+y*xlen+q*xylen+r*xyqlen)>0 && *(stateq+xplus+y*xlen+q*xylen+r*xyqlen)>0 && *(stater+xplus+y*xlen+q*xylen+r*xyqlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=xplus+y*xlen+q*xylen+r*xyqlen;
				*(map_out+xplus+y*xlen+q*xylen+r*xyqlen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+yplus*xlen+q*xylen+r*xyqlen)==0 && *(statex+x+yplus*xlen+q*xylen+r*xyqlen)>0 && *(statey+x+yplus*xlen+q*xylen+r*xyqlen)>0 && *(stateq+x+yplus*xlen+q*xylen+r*xyqlen)>0 && *(stater+x+yplus*xlen+q*xylen+r*xyqlen)>0)
			//
			// However, since the current height increased, the condition *(statey+x+yplus*xlen+q*xylen+r*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+yplus*xlen+q*xylen+r*xyqlen)==0 && *(statex+x+yplus*xlen+q*xylen+r*xyqlen)>0 && *(stateq+x+yplus*xlen+q*xylen+r*xyqlen)>0 && *(stater+x+yplus*xlen+q*xylen+r*xyqlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=x+yplus*xlen+q*xylen+r*xyqlen;
				*(map_out+x+yplus*xlen+q*xylen+r*xyqlen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+y*xlen+qplus*xylen+r*xyqlen)==0 && *(statex+x+y*xlen+qplus*xylen+r*xyqlen)>0 && *(statey+x+y*xlen+qplus*xylen+r*xyqlen)>0 && *(stateq+x+y*xlen+qplus*xylen+r*xyqlen)>0 && *(stater+x+y*xlen+qplus*xylen+r*xyqlen)>0)
			//
			// However, since the current height increased, the condition *(stateq+x+y*xlen+qplus*xylen+r*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+y*xlen+qplus*xylen+r*xyqlen)==0 && *(statex+x+y*xlen+qplus*xylen+r*xyqlen)>0 && *(statey+x+y*xlen+qplus*xylen+r*xyqlen)>0 && *(stater+x+y*xlen+qplus*xylen+r*xyqlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=x+y*xlen+qplus*xylen+r*xyqlen;
				*(map_out+x+y*xlen+qplus*xylen+r*xyqlen)=*index_out;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_out+x+y*xlen+q*xylen+rplus*xyqlen)==0 && *(statex+x+y*xlen+q*xylen+rplus*xyqlen)>0 && *(statey+x+y*xlen+q*xylen+rplus*xyqlen)>0 && *(stateq+x+y*xlen+q*xylen+rplus*xyqlen)>0 && *(stater+x+y*xlen+q*xylen+rplus*xyqlen)>0)
			//
			// However, since the current height increased, the condition *(stater+x+y*xlen+q*xylen+rplus*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_out+x+y*xlen+q*xylen+rplus*xyqlen)==0 && *(statex+x+y*xlen+q*xylen+rplus*xyqlen)>0 && *(statey+x+y*xlen+q*xylen+rplus*xyqlen)>0 && *(stateq+x+y*xlen+q*xylen+rplus*xyqlen)>0)
			{
				(*index_out)++;
				*(index_out+*index_out)=x+y*xlen+q*xylen+rplus*xyqlen;
				*(map_out+x+y*xlen+q*xylen+rplus*xyqlen)=*index_out;
			}

			// If any one of the four neighboring spins at (xminus, y, q, r), (x, yminus, q, r),
			// (x, y, qminus, r), or (x, y, q, rminus) became inactive, disable it.
			if (*(map_in+xminus+y*xlen+q*xylen+r*xyqlen)!=0 && (*(statex+x+y*xlen+q*xylen+r*xyqlen)==0 || *(statey+xminus+yplus*xlen+q*xylen+r*xyqlen)==0 || *(stateq+xminus+y*xlen+qplus*xylen+r*xyqlen)==0 || *(stater+xminus+y*xlen+q*xylen+rplus*xyqlen)==0))
			{
				*(index_in+*(map_in+xminus+y*xlen+q*xylen+r*xyqlen))=0;
				holes_in[holes_in_count++]=*(map_in+xminus+y*xlen+q*xylen+r*xyqlen);
				*(map_in+xminus+y*xlen+q*xylen+r*xyqlen)=0;
			}
			if (*(map_in+x+yminus*xlen+q*xylen+r*xyqlen)!=0 && (*(statex+xplus+yminus*xlen+q*xylen+r*xyqlen)==0 || *(statey+x+y*xlen+q*xylen+r*xyqlen)==0 || *(stateq+x+yminus*xlen+qplus*xylen+r*xyqlen)==0 || *(stater+x+yminus*xlen+q*xylen+rplus*xyqlen)==0))
			{
				*(index_in+*(map_in+x+yminus*xlen+q*xylen+r*xyqlen))=0;
				holes_in[holes_in_count++]=*(map_in+x+yminus*xlen+q*xylen+r*xyqlen);
				*(map_in+x+yminus*xlen+q*xylen+r*xyqlen)=0;
			}
			if (*(map_in+x+y*xlen+qminus*xylen+r*xyqlen)!=0 && (*(statex+xplus+y*xlen+qminus*xylen+r*xyqlen)==0 || *(statey+x+yplus*xlen+qminus*xylen+r*xyqlen)==0 || *(stateq+x+y*xlen+q*xylen+r*xyqlen)==0 || *(stater+x+y*xlen+qminus*xylen+rplus*xyqlen)==0))
			{
				*(index_in+*(map_in+x+y*xlen+qminus*xylen+r*xyqlen))=0;
				holes_in[holes_in_count++]=*(map_in+x+y*xlen+qminus*xylen+r*xyqlen);
				*(map_in+x+y*xlen+qminus*xylen+r*xyqlen)=0;
			}
			if (*(map_in+x+y*xlen+q*xylen+rminus*xyqlen)!=0 && (*(statex+xplus+y*xlen+q*xylen+rminus*xyqlen)==0 || *(statey+x+yplus*xlen+q*xylen+rminus*xyqlen)==0 || *(stateq+x+y*xlen+qplus*xylen+rminus*xyqlen)==0 || *(stater+x+y*xlen+q*xylen+r*xyqlen)==0))
			{
				*(index_in+*(map_in+x+y*xlen+q*xylen+rminus*xyqlen))=0;
				holes_in[holes_in_count++]=*(map_in+x+y*xlen+q*xylen+rminus*xyqlen);
				*(map_in+x+y*xlen+q*xylen+rminus*xyqlen)=0;
			}
		}
		else
		{
			rng_struct->rng_next(rng_struct);
			random_number=(rng_struct->current_double);
		
			j=1+(int)((*index_in)*random_number);
			
			a=*(index_in+j);

			x=a%xlen;
			y=(a/xlen)%ylen;
			q=(a/xylen)%qlen;
			r=a/xyqlen;
			
			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;
			if (q==0) qminus=qlen-1;
			else qminus=q-1;
			if (q==qlen-1) qplus=0;
			else qplus=q+1;
			if (r==0) rminus=rlen-1;
			else rminus=r-1;
			if (r==rlen-1) rplus=0;
			else rplus=r+1;

			// Decrease the selected height.
			// If the height is already the minimum, then set the new height to dh*units-1.
			if (*(height+a)>0) (*(height+a))--;
			else *(height+a)=hlen-1;
			
			(*(statex+a))++;
			(*(statey+a))++;
			(*(stateq+a))++;
			(*(stater+a))++;

			(*(statex+xplus+y*xlen+q*xylen+r*xyqlen))--;
			(*(statey+x+yplus*xlen+q*xylen+r*xyqlen))--;
			(*(stateq+x+y*xlen+qplus*xylen+r*xyqlen))--;
			(*(stater+x+y*xlen+qplus*xylen+r*xyqlen))--;

			// Save info on the spin that flipped.
			parameters->coordinate=a;
			parameters->move=1;

			// If the flipped spin became active, enable it.
			if (*(map_out+a)==0)
			{
				(*index_out)++;
				*(index_out+*index_out)=a;
				*(map_out+a)=*index_out;
			}
			// If the flipped spin became inactive, disable it.
			if (*(statex+xplus+y*xlen+q*xylen+r*xyqlen)==0 || *(statey+x+yplus*xlen+q*xylen+r*xyqlen)==0 || *(stateq+x+y*xlen+qplus*xylen+r*xyqlen)==0 || *(stater+x+y*xlen+q*xylen+rplus*xyqlen)==0)
			{
				*(index_in+j)=0;
				holes_in[holes_in_count++]=j;
				*(map_in+a)=0;
			}

			// If any one of the four neighboring spins at (xminus, y, q, r), (x, yminus, q, r),
			// (x, y, qminus, r), or (x, y, q, rminus) became active, enable it.
			//
			// Note that the full form for the if statement is
			// if (*(map_in+xminus+y*xlen+q*xylen+r*xyqlen)==0 && *(statex+x+y*xlen+q*xylen+r*xyqlen)>0 && *(statey+xminus+yplus*xlen+q*xylen+r*xyqlen)>0 && *(stateq+xminus+y*xlen+qplus*xylen+r*xyqlen)>0 && *(stater+xminus+y*xlen+q*xylen+rplus*xyqlen)>0)
			//
			// However, since the current height decreased, the condition *(statex+x+y*xlen+q*xylen+r*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+xminus+y*xlen+q*xylen+r*xyqlen)==0 && *(statey+xminus+yplus*xlen+q*xylen+r*xyqlen)>0 && *(stateq+xminus+y*xlen+qplus*xylen+r*xyqlen)>0 && *(stater+xminus+y*xlen+q*xylen+rplus*xyqlen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=xminus+y*xlen+q*xylen+r*xyqlen;
				*(map_in+xminus+y*xlen+q*xylen+r*xyqlen)=*index_in;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+yminus*xlen+q*xylen+r*xyqlen)==0 && *(statex+xplus+yminus*xlen+q*xylen+r*xyqlen)>0 && *(statey+x+y*xlen+q*xylen+r*xyqlen)>0 && *(stateq+x+yminus*xlen+qplus*xylen+r*xyqlen)>0 && *(stater+x+yminus*xlen+q*xylen+rplus*xyqlen)>0)
			//
			// However, since the current height decreased, the condition *(statey+x+y*xlen+q*xylen+r*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+x+yminus*xlen+q*xylen+r*xyqlen)==0 && *(statex+xplus+yminus*xlen+q*xylen+r*xyqlen)>0 && *(stateq+x+yminus*xlen+qplus*xylen+r*xyqlen)>0 && *(stater+x+yminus*xlen+q*xylen+rplus*xyqlen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=x+yminus*xlen+q*xylen+r*xyqlen;
				*(map_in+x+yminus*xlen+q*xylen+r*xyqlen)=*index_in;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+y*xlen+qminus*xylen+r*xyqlen)==0 && *(statex+xplus+y*xlen+qminus*xylen+r*xyqlen)>0 && *(statey+x+yplus*xlen+qminus*xylen+r*xyqlen)>0 && *(stateq+x+y*xlen+q*xylen+r*xyqlen)>0 && *(stater+x+y*xlen+qminus*xylen+rplus*xyqlen)>0)
			//
			// However, since the current height decreased, the condition *(stateq+x+y*xlen+q*xylen+r*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+x+y*xlen+qminus*xylen+r*xyqlen)==0 && *(statex+xplus+y*xlen+qminus*xylen+r*xyqlen)>0 && *(statey+x+yplus*xlen+qminus*xylen+r*xyqlen)>0 && *(stater+x+y*xlen+qminus*xylen+rplus*xyqlen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=x+y*xlen+qminus*xylen+r*xyqlen;
				*(map_in+x+y*xlen+qminus*xylen+r*xyqlen)=*index_in;
			}
			//
			// Note that the full form for the if statement is
			// if (*(map_in+x+y*xlen+q*xylen+rminus*xyqlen)==0 && *(statex+xplus+y*xlen+q*xylen+rminus*xyqlen)>0 && *(statey+x+yplus*xlen+q*xylen+rminus*xyqlen)>0 && *(stateq+x+y*xlen+qplus*xylen+rminus*xyqlen)>0 && *(stater+x+y*xlen+q*xylen+r*xyqlen)>0)
			//
			// However, since the current height decreased, the condition *(stater+x+y*xlen+q*xylen+r*xyqlen)>0 is
			// automatically satisfied.  Therefore, this condition is omitted to speed up the simulation.
			//
			if (*(map_in+x+y*xlen+q*xylen+rminus*xyqlen)==0 && *(statex+xplus+y*xlen+q*xylen+rminus*xyqlen)>0 && *(statey+x+yplus*xlen+q*xylen+rminus*xyqlen)>0 && *(stateq+x+y*xlen+qplus*xylen+rminus*xyqlen)>0)
			{
				(*index_in)++;
				*(index_in+*index_in)=x+y*xlen+q*xylen+rminus*xyqlen;
				*(map_in+x+y*xlen+q*xylen+rminus*xyqlen)=*index_in;
			}

			// If any one of the four neighboring spins at (xplus, y, q, r), (x, yplus, q, r),
			// (x, y, qplus, r), or (x, y, q, rplus) became inactive, disable it.
			if (*(map_out+xplus+y*xlen+q*xylen+r*xyqlen)!=0 && (*(statex+xplus+y*xlen+q*xylen+r*xyqlen)==0 || *(statey+xplus+y*xlen+q*xylen+r*xyqlen)==0 || *(stateq+xplus+y*xlen+q*xylen+r*xyqlen)==0 || *(stater+xplus+y*xlen+q*xylen+r*xyqlen)==0))
			{
				*(index_out+*(map_out+xplus+y*xlen+q*xylen+r*xyqlen))=0;
				holes_out[holes_out_count++]=*(map_out+xplus+y*xlen+q*xylen+r*xyqlen);
				*(map_out+xplus+y*xlen+q*xylen+r*xyqlen)=0;
			}
			if (*(map_out+x+yplus*xlen+q*xylen+r*xyqlen)!=0 && (*(statex+x+yplus*xlen+q*xylen+r*xyqlen)==0 || *(statey+x+yplus*xlen+q*xylen+r*xyqlen)==0 || *(stateq+x+yplus*xlen+q*xylen+r*xyqlen)==0 || *(stater+x+yplus*xlen+q*xylen+r*xyqlen)==0))
			{
				*(index_out+*(map_out+x+yplus*xlen+q*xylen+r*xyqlen))=0;
				holes_out[holes_out_count++]=*(map_out+x+yplus*xlen+q*xylen+r*xyqlen);
				*(map_out+x+yplus*xlen+q*xylen+r*xyqlen)=0;
			}
			if (*(map_out+x+y*xlen+qplus*xylen+r*xyqlen)!=0 && (*(statex+x+y*xlen+qplus*xylen+r*xyqlen)==0 || *(statey+x+y*xlen+qplus*xylen+r*xyqlen)==0 || *(stateq+x+y*xlen+qplus*xylen+r*xyqlen)==0 || *(stater+x+y*xlen+qplus*xylen+r*xyqlen)==0))
			{
				*(index_out+*(map_out+x+y*xlen+qplus*xylen+r*xyqlen))=0;
				holes_out[holes_out_count++]=*(map_out+x+y*xlen+qplus*xylen+r*xyqlen);
				*(map_out+x+y*xlen+qplus*xylen+r*xyqlen)=0;
			}
			if (*(map_out+x+y*xlen+q*xylen+rplus*xyqlen)!=0 && (*(statex+x+y*xlen+q*xylen+rplus*xyqlen)==0 || *(statey+x+y*xlen+q*xylen+rplus*xyqlen)==0 || *(stateq+x+y*xlen+q*xylen+rplus*xyqlen)==0 || *(stater+x+y*xlen+q*xylen+rplus*xyqlen)==0))
			{
				*(index_out+*(map_out+x+y*xlen+q*xylen+rplus*xyqlen))=0;
				holes_out[holes_out_count++]=*(map_out+x+y*xlen+q*xylen+rplus*xyqlen);
				*(map_out+x+y*xlen+q*xylen+rplus*xyqlen)=0;
			}
		}

		// Fill in holes in the active spins array for heights that can increase, if necessary.		
		holes_out_count--;
		while (holes_out_count>=0)
		{
			if (*(index_out+*index_out)!=0 || (*(index_out+*index_out)==0 && *map_out==*index_out))
			{
				*(index_out+holes_out[holes_out_count])=*(index_out+*index_out);
				*(map_out+*(index_out+*index_out))=holes_out[holes_out_count];
				*(index_out+*index_out)=0;
				holes_out[holes_out_count]=0;
			}
			else if (holes_out[0]==*index_out) holes_out[0]=holes_out[1];
			
			(*index_out)--;
			holes_out_count--;

			if (*index_out==0) break;
		}
		holes_out_count=0;

		// Fill in holes in the active spins array for heights that can decrease, if necessary.		
		holes_in_count--;
		while (holes_in_count>=0)
		{
			if (*(index_in+*index_in)!=0 || (*(index_in+*index_in)==0 && *map_in==*index_in))
			{
				*(index_in+holes_in[holes_in_count])=*(index_in+*index_in);
				*(map_in+*(index_in+*index_in))=holes_in[holes_in_count];
				*(index_in+*index_in)=0;
				holes_in[holes_in_count]=0;
			}
			else if (holes_in[0]==*index_in) holes_in[0]=holes_in[1];
			
			(*index_in)--;
			holes_in_count--;

			if (*index_in==0) break;
		}
		holes_in_count=0;
	
		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}

		parameters->move=0;
	}
	while (evolve_continue==1 && 
		(*index_out>0 || (fullbias==0 && *index_in>0)) &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));
	//
	// The simulation stops if:
	// 	- the # of active spins is zero,
	// 	- the number of steps exceeds the maximum allowed number of steps, or
	// 	- the time exceeds the maximum allowed time.

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int run_integrate_1 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters)
{
	int length=int_parameters->length;
	int maxtime=int_parameters->maxtime;
	double *height=int_parameters->height;
	
	int i, j, x, y, area=length*length;
	
	double dzdx, dzdy, dzdt;
	
	memset(height, 0, sizeof(double)*area);

	double *height_add=(double*)malloc(sizeof(double)*area);
		
	for (i=0; i<maxtime; i++)
	{
		for (y=0; y<length; y++)
		{
			if (y>0 && *(height+(y-1)*length)==0) {memset (height_add+y*length, 0, sizeof(double)*(length-y)*length); break;}

			for (x=0; x<length; x++)
			{
				if (x>0 && y>0)
				{
					dzdx=*(height+x+y*length)-*(height+(x-1)+y*length);
					dzdy=*(height+x+y*length)-*(height+x+(y-1)*length);
					if (dzdx!=0 || dzdy!=0) dzdt=(dzdx*dzdy)/(dzdx*dzdy-dzdx-dzdy);
					else dzdt=0;
				}
				else if (x==0 && y>0)
				{
					dzdy=*(height+x+y*length)-*(height+x+(y-1)*length);
					dzdt=dzdy/(dzdy-1);
				}
				else if (x>0 && y==0)
				{
					dzdx=*(height+x+y*length)-*(height+(x-1)+y*length);
					dzdt=dzdx/(dzdx-1);
				}
				else dzdt=1;

				if (dzdt==0) {memset (height_add+x+y*length, 0, sizeof(double)*(length-x)); break;}

				*(height_add+x+y*length)=dzdt;
			}
		}
		
		for (j=0; j<area; j++)
		{
			*(height+j)+=*(height_add+j);
		}
	}

	free(height_add);
	
	return 0;
}

short int run_integrate_4d_1 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters)
{
	int length=int_parameters->length;
	int maxtime=int_parameters->maxtime;
	double *height=int_parameters->height;
	
	int i, j, x, y, q, length2=length*length, length3=length2*length;
	
	double dzdx, dzdy, dzdq, dzdt;
	
	memset(height, 0, sizeof(double)*length3);

	double *height_add=(double*)malloc(sizeof(double)*length3);
		
	for (i=0; i<maxtime; i++)
	{
		for (q=0; q<length; q++)
		{
			if (q>0 && *(height+(q-1)*length2)==0) {memset (height_add+q*length2, 0, sizeof(double)*(length-q)*length2); break;}

			for (y=0; y<length; y++)
			{
				if (y>0 && *(height+(y-1)*length)==0) {memset (height_add+y*length, 0, sizeof(double)*(length-y)*length); break;}

				for (x=0; x<length; x++)
				{
					if (x>0 && y>0 && q>0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						if (dzdx!=0 || dzdy!=0 || dzdq!=0) dzdt=1/(1-1/dzdx-1/dzdy-1/dzdq);
						else dzdt=0;
					}
					else if (x==0 && y>0 && q>0)
					{
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						dzdt=1/(1-1/dzdy-1/dzdq);
					}
					else if (x>0 && y==0 && q>0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						dzdt=1/(1-1/dzdx-1/dzdq);
					}
					else if (x>0 && y>0 && q==0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdt=1/(1-1/dzdx-1/dzdy);
					}
					else if (x==0 && y==0 && q>0)
					{
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						dzdt=1/(1-1/dzdq);
					}
					else if (x==0 && y>0 && q==0)
					{
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdt=1/(1-1/dzdy);
					}
					else if (x>0 && y==0 && q==0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdt=1/(1-1/dzdx);
					}
					else dzdt=1;

					if (dzdt==0) {memset (height_add+x+y*length+q*length2, 0, sizeof(double)*(length-x)); break;}

					*(height_add+x+y*length+q*length2)=dzdt;
				}
			}
		}
		
		for (j=0; j<length3; j++)
		{
			*(height+j)+=*(height_add+j);
		}
	}

	free(height_add);
	
	return 0;
}

short int run_integrate_2 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters)
{
	int length=int_parameters->length;
	int maxtime=int_parameters->maxtime;
	double *height=int_parameters->height;
	
	int i, j, x, y, area=length*length;
	
	double dzdx, dzdy, dzdt;
	
	memset(height, 0, sizeof(double)*area);

	double *height_add=(double*)malloc(sizeof(double)*area);
		
	for (i=0; i<maxtime; i++)
	{
		for (y=0; y<length; y++)
		{
			if (y>0 && *(height+(y-1)*length)==0) {memset (height_add+y*length, 0, sizeof(double)*(length-y)*length); break;}

			for (x=0; x<length; x++)
			{
				if (x>0 && y>0)
				{
					dzdx=*(height+x+y*length)-*(height+(x-1)+y*length);
					dzdy=*(height+x+y*length)-*(height+x+(y-1)*length);
					if (dzdx!=0 || dzdy!=0) dzdt=dzdx*dzdy*(dzdx+dzdy-1)/(dzdx-1)/(dzdy-1)/(dzdx+dzdy);
					else dzdt=0;
				}
				else if (x==0 && y>0)
				{
					dzdy=*(height+x+y*length)-*(height+x+(y-1)*length);
					dzdt=dzdy/(dzdy-1);
				}
				else if (x>0 && y==0)
				{
					dzdx=*(height+x+y*length)-*(height+(x-1)+y*length);
					dzdt=dzdx/(dzdx-1);
				}
				else dzdt=1;

				if (dzdt==0) {memset (height_add+x+y*length, 0, sizeof(double)*(length-x)); break;}

				*(height_add+x+y*length)=dzdt;
			}
		}
		
		for (j=0; j<area; j++)
		{
			*(height+j)+=*(height_add+j);
		}
	}

	free(height_add);
	
	return 0;
}

short int run_integrate_4d_2 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters)
{
	int length=int_parameters->length;
	int maxtime=int_parameters->maxtime;
	double *height=int_parameters->height;
	
	int i, j, x, y, q, length2=length*length, length3=length2*length;
	
	double dzdx, dzdy, dzdq, dzdt;
	
	memset(height, 0, sizeof(double)*length3);

	double *height_add=(double*)malloc(sizeof(double)*length3);
		
	for (i=0; i<maxtime; i++)
	{
		for (q=0; q<length; q++)
		{
			if (q>0 && *(height+(q-1)*length2)==0) {memset (height_add+q*length2, 0, sizeof(double)*(length-q)*length2); break;}

			for (y=0; y<length; y++)
			{
				if (y>0 && *(height+(y-1)*length)==0) {memset (height_add+y*length, 0, sizeof(double)*(length-y)*length); break;}

				for (x=0; x<length; x++)
				{
					if (x>0 && y>0 && q>0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						if (dzdx!=0 || dzdy!=0 || dzdq!=0) dzdt=(1-1/(dzdx+dzdy))*(1-1/(dzdy+dzdq))*(1-1/(dzdq+dzdx))/(1-1/dzdx)/(1-1/dzdy)/(1-1/dzdq)/(1-1/(dzdx+dzdy+dzdq));
						else dzdt=0;
					}
					else if (x==0 && y>0 && q>0)
					{
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						dzdt=(1-1/(dzdy+dzdq))/(1-1/dzdy)/(1-1/dzdq);
					}
					else if (x>0 && y==0 && q>0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						dzdt=(1-1/(dzdx+dzdq))/(1-1/dzdx)/(1-1/dzdq);
					}
					else if (x>0 && y>0 && q==0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdt=(1-1/(dzdx+dzdy))/(1-1/dzdx)/(1-1/dzdy);
					}
					else if (x==0 && y==0 && q>0)
					{
						dzdq=*(height+x+y*length+q*length2)-*(height+x+y*length+(q-1)*length2);
						dzdt=1/(1-1/dzdq);
					}
					else if (x==0 && y>0 && q==0)
					{
						dzdy=*(height+x+y*length+q*length2)-*(height+x+(y-1)*length+q*length2);
						dzdt=1/(1-1/dzdy);
					}
					else if (x>0 && y==0 && q==0)
					{
						dzdx=*(height+x+y*length+q*length2)-*(height+(x-1)+y*length+q*length2);
						dzdt=1/(1-1/dzdx);
					}
					else dzdt=1;

					if (dzdt==0) {memset (height_add+x+y*length+q*length2, 0, sizeof(double)*(length-x)); break;}

					*(height_add+x+y*length+q*length2)=dzdt;
				}
			}
		}
		
		for (j=0; j<length3; j++)
		{
			*(height+j)+=*(height_add+j);
		}
	}

	free(height_add);
	
	return 0;
}

short int run_integrate_3 (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct integrate_parameters *int_parameters)
{
	int length=int_parameters->length;
	int maxtime=int_parameters->maxtime;
	double *height=int_parameters->height;
	double delta_t=int_parameters->delta_t;
	double delta_x=int_parameters->delta_x;
	
	int i, j, x, y, area=length*length;
	
	double dzdx, dzdy, dzdt;
	double d2zdx2, d2zdy2, d2zdxdy;
	
	memset(height, 0, sizeof(double)*area);

	double *height_add=(double*)malloc(sizeof(double)*area);

	for (i=0; i<(int)(maxtime/delta_t); i++)
	{
		for (y=0; y<length-1; y++)
		{
			if (y>0 && *(height+(y-1)*length)==0) {memset (height_add+y*length, 0, sizeof(double)*(length-1-y)*length); break;}

			for (x=0; x<length-1; x++)
			{
				if (x>0 && y>0)
				{
					dzdx=(*(height+(x+1)+y*length)-*(height+(x-1)+y*length))/(2*delta_x);
					dzdy=(*(height+x+(y+1)*length)-*(height+x+(y-1)*length))/(2*delta_x);
					d2zdx2=(*(height+(x-1)+y*length)+*(height+(x+1)+y*length)-2*(*(height+x+y*length)))/(delta_x*delta_x);
					d2zdy2=(*(height+x+(y-1)*length)+*(height+x+(y+1)*length)-2*(*(height+x+y*length)))/(delta_x*delta_x);
					d2zdxdy=(*(height+(x+1)+(y+1)*length)+*(height+(x-1)+(y-1)*length)-*(height+(x+1)+(y-1)*length)-*(height+(x-1)+(y+1)*length))/(4*delta_x*delta_x);
					
					if (dzdx!=0 && dzdy!=0) dzdt=(1-1/(dzdx+dzdy))/(1-1/dzdx)/(1-1/dzdy)*(1-1/(dzdx+dzdy))/(1-1/dzdx)/(1-1/dzdy)*(d2zdx2/dzdx/dzdx+d2zdy2/dzdy/dzdy-d2zdxdy/dzdx/dzdy);
					else dzdt=0;
				}
				else if (x==0 && y>0)
				{
					dzdy=(*(height+x+(y+1)*length)-*(height+x+(y-1)*length))/(2*delta_x);
					d2zdy2=(*(height+x+(y-1)*length)+*(height+x+(y+1)*length)-2*(*(height+x+y*length)))/(delta_x*delta_x);
					
					dzdt=d2zdy2/(dzdy-1)/(dzdy-1);
				}
				else if (x>0 && y==0)
				{
					dzdx=(*(height+(x+1)+y*length)-*(height+(x-1)+y*length))/(2*delta_x);
					d2zdx2=(*(height+(x-1)+y*length)+*(height+(x+1)+y*length)-2*(*(height+x+y*length)))/(delta_x*delta_x);
					
					dzdt=d2zdx2/(dzdx-1)/(dzdx-1);
				}
				
				if (dzdt<0) dzdt=0;

				if (x>0 && y>0 && dzdt==0) {memset (height_add+x+y*length, 0, sizeof(double)*(length-1-x)); break;}

				*(height_add+x+y*length)=dzdt;
			}
		}

		for (j=0; j<area; j++)
		{
			*(height+j)+=*(height_add+j)*delta_t;
		}
		
		// The linear scale of the interface grows diffusively.
		// At y/x=1000 on the boundaries, y/sqrt(t)~=3.44864.
		*height=3.44864*sqrt((i+1)*delta_t);
		
		for (y=0; y<length-1; y++)
		{
			for (x=(int)(*(height+y*length))+1; x<length-1; x++)
			{
				*(height+x+y*length)=0;
			}
		}
		for (x=0; x<length-1; x++)
		{
			for (y=(int)(*(height+x))+1; y<length-1; y++)
			{
				*(height+x+y*length)=0;
			}
		}
		
		if ((int)(i*delta_t)%1000==0) printf("time elapsed = %d\n",i*delta_t);
	}

	free(height_add);
	
	return 0;
}

