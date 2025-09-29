/**************************************************************
***************************************************************
*** 
***   cmd-set.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains commands for setting parameters.
***
***************************************************************
**************************************************************/

void cmd_cpcur()
{
	short int data_index=arg_short_int[0];
	
	static struct run_parameters *p;
	static int *p_size;
	
	if (data_input(data, data_parameters, &p, &p_size, data_index, data_num)==1) return result_error;
	
	memcpy (p->height_initial, p->height, sizeof(int)*(p->xlen)*(p->ylen));
	memcpy (p->statex_initial, p->statex, sizeof(int)*(p->xlen)*(p->ylen));
	memcpy (p->statey_initial, p->statey, sizeof(int)*(p->xlen)*(p->ylen));
	
	return;
}

void cmd_cpcur4d()
{
	short int data_index=arg_short_int[0];
	
	static struct run_parameters_4d *p;
	static int *p_size;
	
	if (data_input(data, data_parameters, &p, &p_size, data_index, data_num)==1) return result_error;
	
	memcpy (p->height_initial, p->height, sizeof(int)*(p->xlen)*(p->ylen)*(p->qlen));
	memcpy (p->statex_initial, p->statex, sizeof(int)*(p->xlen)*(p->ylen)*(p->qlen));
	memcpy (p->statey_initial, p->statey, sizeof(int)*(p->xlen)*(p->ylen)*(p->qlen));
	memcpy (p->stateq_initial, p->stateq, sizeof(int)*(p->xlen)*(p->ylen)*(p->qlen));
	
	return;
}

void cmd_rnglcg()
{
	short int data_index=arg_short_int[0];
	
	struct rng_struct *rng_struct;
	int *rng_struct_size;
	
	// Allocate a block of memory to store info for the random number generator.
	if (data_output(data, data_parameters, &rng_struct, &rng_struct_size, sizeof(struct rng_struct), sizeof(int), data_index, data_num)==1) return result_error;
	
	rng_struct->rng_alloc=rng_alloc_lcg;
	rng_struct->rng_seed=rng_seed_lcg;
	rng_struct->rng_next=rng_next_lcg;

	rng_struct->rng_alloc(rng_struct);
	
	return;
}

void cmd_rngmt()
{
	short int data_index=arg_short_int[0];
	
	struct rng_struct *rng_struct;
	int *rng_struct_size;
	
	// Allocate a block of memory to store info for the random number generator.
	if (data_output(data, data_parameters, &rng_struct, &rng_struct_size, sizeof(struct rng_struct), sizeof(int), data_index, data_num)==1) return result_error;
	
	rng_struct->rng_alloc=rng_alloc_mt;
	rng_struct->rng_seed=rng_seed_mt;
	rng_struct->rng_next=rng_next_mt;

	rng_struct->rng_alloc(rng_struct);
	
	return;
}

void cmd_seed()
{
	set_seed(arg_double[0], arg_double[1], data[arg_short_int[0]]);
	return;
}

void cmd_set2d()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int units=arg_int[0];
	int dh=arg_int[1];
	int dx=arg_int[2];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	short int fullbias=arg_short_int[2];
	double prob_outer_corner_flips=arg_double[1];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	int i, x;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->hlen=dh*units;
	parameters->xlen=dx*units;
	parameters->ylen=1;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->fullbias=fullbias;
	parameters->prob_outer_corner_flips=prob_outer_corner_flips;

	// Allocate memory.
	parameters->height_initial=(int*)malloc (sizeof(int)*dx*units);
	parameters->height=(int*)malloc (sizeof(int)*dx*units);
	parameters->statex_initial=(int*)malloc (sizeof(int)*dx*units);
	parameters->statex=(int*)malloc (sizeof(int)*dx*units);
	parameters->statey_initial=(int*)malloc (sizeof(int)*dx*units);
	parameters->statey=(int*)malloc (sizeof(int)*dx*units);
	parameters->map_out=(int*)malloc (sizeof(int)*dx*units);
	parameters->index_out=(int*)malloc (sizeof(int)*(1+dx*units));
	parameters->map_in=(int*)malloc (sizeof(int)*dx*units);
	parameters->index_in=(int*)malloc (sizeof(int)*(1+dx*units));

	// Load the initial state.
	for (i=0; i<dx*units; i++)
	{
		x=i;
		
		if (x%dx==0) *(parameters->statex_initial+i)=dh;
		else *(parameters->statex_initial+i)=0;
		*(parameters->statey_initial+i)=dh;
		
		*(parameters->height_initial+x)=dh*(units-1-(x/dx));
	}

	return;
}

void cmd_set3d()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int units=arg_int[0];
	int dh=arg_int[1];
	int dx=arg_int[2];
	int dy=arg_int[3];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	short int fullbias=arg_short_int[2];
	double prob_outer_corner_flips=arg_double[1];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	int i, x, y;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->hlen=dh*units;
	parameters->xlen=dx*units;
	parameters->ylen=dy*units;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->fullbias=fullbias;
	parameters->prob_outer_corner_flips=prob_outer_corner_flips;

	// Allocate memory.
	parameters->height_initial=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->height=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->statex_initial=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->statex=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->statey_initial=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->statey=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->map_out=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->index_out=(int*)malloc (sizeof(int)*(1+dx*units*dy*units));
	parameters->map_in=(int*)malloc (sizeof(int)*dx*units*dy*units);
	parameters->index_in=(int*)malloc (sizeof(int)*(1+dx*units*dy*units));

	// Load the initial state.
	for (i=0; i<dx*units*dy*units; i++)
	{
		x=i%(dx*units);
		y=i/(dx*units);
		
		if (x%dx==0) *(parameters->statex_initial+i)=dh;
		else *(parameters->statex_initial+i)=0;
		if (y%dy==0) *(parameters->statey_initial+i)=dh;
		else *(parameters->statey_initial+i)=0;
		
		*(parameters->height_initial+x+y*dx*units)=dh*(units-1-(x/dx)-(y/dy));
		
		if (*(parameters->height_initial+x+y*dx*units)<0) *(parameters->height_initial+x+y*dx*units)+=dh*units;
	}

	return;
}

void cmd_set4d()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int units=arg_int[0];
	int dh=arg_int[1];
	int dx=arg_int[2];
	int dy=arg_int[3];
	int dq=arg_int[4];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	short int fullbias=arg_short_int[2];
	double prob_outer_corner_flips=arg_double[1];
	
	static struct run_parameters_4d *parameters;
	static int *parameters_size;
	
	int i, x, y, q;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters_4d), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->hlen=dh*units;
	parameters->xlen=dx*units;
	parameters->ylen=dy*units;
	parameters->qlen=dq*units;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->fullbias=fullbias;
	parameters->prob_outer_corner_flips=prob_outer_corner_flips;

	// Allocate memory.
	parameters->height_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->height=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->statex_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->statex=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->statey_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->statey=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->stateq_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->stateq=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->map_out=(int*)malloc (sizeof(int)*dx*units*dy*units*dq*units);
	parameters->index_out=(int*)malloc (sizeof(int)*(1+dx*units*dy*units*dq*units));
	parameters->map_in=(int*)malloc (sizeof(int)*dx*units*dy*units*dq*units);
	parameters->index_in=(int*)malloc (sizeof(int)*(1+dx*units*dy*units*dq*units));

	// Load the initial state.
	for (i=0; i<dx*units*dy*units*dq*units; i++)
	{
		x=i%(dx*units);
		y=(i/(dx*units))%(dy*units);
		q=i/(dx*units*dy*units);
		
		if (x%dx==0) *(parameters->statex_initial+i)=dh;
		else *(parameters->statex_initial+i)=0;
		if (y%dy==0) *(parameters->statey_initial+i)=dh;
		else *(parameters->statey_initial+i)=0;
		if (q%dq==0) *(parameters->stateq_initial+i)=dh;
		else *(parameters->stateq_initial+i)=0;
		
		*(parameters->height_initial+x+y*dx*units+q*dx*units*dy*units)=dh*(units-1-(x/dx)-(y/dy)-(q/dq));
		
		if (*(parameters->height_initial+x+y*dx*units+q*dx*units*dy*units)<-dh*units) *(parameters->height_initial+x+y*dx*units+q*dx*units*dy*units)+=2*dh*units;
		else if (*(parameters->height_initial+x+y*dx*units+q*dx*units*dy*units)<0) *(parameters->height_initial+x+y*dx*units+q*dx*units*dy*units)+=dh*units;
	}

	return;
}

void cmd_set4d2()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int units=arg_int[0];
	int dh=arg_int[1];
	int dx=arg_int[2];
	int dy=arg_int[3];
	int dq=arg_int[4];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	short int fullbias=arg_short_int[2];
	double prob_outer_corner_flips=arg_double[1];
	
	static struct run_parameters_4d *parameters;
	static int *parameters_size;
	
	int i, x, y, q;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters_4d), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->hlen=dh*units;
	parameters->xlen=dx*units;
	parameters->ylen=dy*units;
	parameters->qlen=dq*units;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->fullbias=fullbias;
	parameters->prob_outer_corner_flips=prob_outer_corner_flips;

	// Allocate memory.
	parameters->height=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units);
	parameters->map_out=(int*)malloc (sizeof(int)*dx*units*dy*units*dq*units);
	parameters->index_out=(int*)malloc (sizeof(int)*(1+dx*units*dy*units*dq*units));

	// Load the initial state.
	for (i=0; i<dx*units*dy*units*dq*units; i++)
	{
		x=i%(dx*units);
		y=(i/(dx*units))%(dy*units);
		q=i/(dx*units*dy*units);
		
		*(parameters->height+x+y*dx*units+q*dx*units*dy*units)=dh*(units-1-(x/dx)-(y/dy)-(q/dq));
		
		if (*(parameters->height+x+y*dx*units+q*dx*units*dy*units)<-dh*units) *(parameters->height+x+y*dx*units+q*dx*units*dy*units)+=2*dh*units;
		else if (*(parameters->height+x+y*dx*units+q*dx*units*dy*units)<0) *(parameters->height+x+y*dx*units+q*dx*units*dy*units)+=dh*units;
	}

	return;
}

void cmd_set4d3()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int units=arg_int[0];
	int dh=arg_int[1];
	int dx=arg_int[2];
	int dy=arg_int[3];
	int dq=arg_int[4];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	short int fullbias=arg_short_int[2];
	double prob_outer_corner_flips=arg_double[1];
	
	static struct run_parameters_4d3 *parameters;
	static int *parameters_size;
	
	unsigned long int i, x, y, q, size, length, offset;
	
	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters_4d), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->hlen=dh*units;
	parameters->xlen=dx*units;
	parameters->ylen=dy*units;
	parameters->qlen=dq*units;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->fullbias=fullbias;
	parameters->prob_outer_corner_flips=prob_outer_corner_flips;

	// `run4d3' only evolves a corner.
	length=dx*units;
	
	// This is the total number of heights.
	size=((length-1)*((length-1)-1)*(2*(length-1)-1)/6-(length-1)*((length-1)-1)*(2*length+1)/2+(length-1)*length*(length+1))/2+1;

	// Allocate memory.
	parameters->height=(short int*)malloc (sizeof(short int)*size);
	parameters->map_out=(int*)malloc (sizeof(int)*size);
	parameters->index_out=(unsigned long int*)malloc (sizeof(unsigned long int)*(1+size));

	// Load the initial state.
	for (q=0; q<length; q++)
	{
		for (y=0; y<length-q; y++)
		{
			for (x=0; x<length-q-y; x++)
			{
				offset=(q*(q-1)*(2*q-1)/6-q*(q-1)*(2*length+1)/2+q*length*(length+1))/2+y*(length-q)+(y-1)*y/2+x;
				
				*(parameters->height+offset)=dh*(units-1-(x/dx)-(y/dy)-(q/dq));
				
				if (*(parameters->height+offset)<-dh*units) *(parameters->height+offset)+=2*dh*units;
				else if (*(parameters->height+offset)<0) *(parameters->height+offset)+=dh*units;
			}
		}
	}

	return;
}

void cmd_set5d()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int units=arg_int[0];
	int dh=arg_int[1];
	int dx=arg_int[2];
	int dy=arg_int[3];
	int dq=arg_int[4];
	int dr=arg_int[5];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	short int fullbias=arg_short_int[2];
	double prob_outer_corner_flips=arg_double[1];
	
	static struct run_parameters_5d *parameters;
	static int *parameters_size;
	
	int i, x, y, q, r;
	int xlen, ylen, qlen, rlen;
	int xylen, xyqlen, xyqrlen;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters_5d), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->hlen=dh*units;
	parameters->xlen=dx*units;
	parameters->ylen=dy*units;
	parameters->qlen=dq*units;
	parameters->rlen=dr*units;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->fullbias=fullbias;
	parameters->prob_outer_corner_flips=prob_outer_corner_flips;

	xlen=parameters->xlen;
	ylen=parameters->ylen;
	qlen=parameters->qlen;
	rlen=parameters->rlen;

	xylen=xlen*ylen;
	xyqlen=xylen*qlen;
	xyqrlen=xyqlen*rlen;

	// Allocate memory.
	parameters->height_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->height=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->statex_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->statex=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->statey_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->statey=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->stateq_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->stateq=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->stater_initial=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->stater=(short int*)malloc (sizeof(short int)*dx*units*dy*units*dq*units*dr*units);
	parameters->map_out=(int*)malloc (sizeof(int)*dx*units*dy*units*dq*units*dr*units);
	parameters->index_out=(int*)malloc (sizeof(int)*(1+dx*units*dy*units*dq*units*dr*units));
	parameters->map_in=(int*)malloc (sizeof(int)*dx*units*dy*units*dq*units*dr*units);
	parameters->index_in=(int*)malloc (sizeof(int)*(1+dx*units*dy*units*dq*units*dr*units));

	// Load the initial state.
	for (i=0; i<xyqrlen; i++)
	{
		x=i%xlen;
		y=(i/xlen)%ylen;
		q=(i/xylen)%qlen;
		r=i/xyqlen;
		
		if (x%dx==0) *(parameters->statex_initial+i)=dh;
		else *(parameters->statex_initial+i)=0;
		if (y%dy==0) *(parameters->statey_initial+i)=dh;
		else *(parameters->statey_initial+i)=0;
		if (q%dq==0) *(parameters->stateq_initial+i)=dh;
		else *(parameters->stateq_initial+i)=0;
		if (r%dr==0) *(parameters->stater_initial+i)=dh;
		else *(parameters->stater_initial+i)=0;
		
		*(parameters->height_initial+x+y*xlen+q*xylen+r*xyqlen)=dh*(units-1-(x/dx)-(y/dy)-(q/dq)-(r/dr));
		
		if (*(parameters->height_initial+x+y*xlen+q*xylen+r*xyqlen)<-2*dh*units) *(parameters->height_initial+x+y*xlen+q*xylen+r*xyqlen)+=3*dh*units;
		else if (*(parameters->height_initial+x+y*xlen+q*xylen+r*xyqlen)<-dh*units) *(parameters->height_initial+x+y*xlen+q*xylen+r*xyqlen)+=2*dh*units;
		else if (*(parameters->height_initial+x+y*xlen+q*xylen+r*xyqlen)<0) *(parameters->height_initial+x+y*xlen+q*xylen+r*xyqlen)+=dh*units;
	}

	return;
}

void cmd_setint()
{
	short int data_index=arg_short_int[0];
	int length=arg_int[0];
	int maxtime=arg_int[1];
	double delta_t=arg_double[0];
	double delta_x=arg_double[1];
	
	static struct integrate_parameters *int_parameters;
	static int *int_parameters_size;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &int_parameters, &int_parameters_size, sizeof(struct integrate_parameters), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	int_parameters->length=length;
	int_parameters->maxtime=maxtime;
	int_parameters->delta_t=delta_t;
	int_parameters->delta_x=delta_x;

	// Allocate memory.
	int_parameters->height=(double*)malloc(sizeof(double)*length*length);

	return;
}

void cmd_settsc()
{
	set_settsc(data[arg_short_int[0]]);
	return;
}

