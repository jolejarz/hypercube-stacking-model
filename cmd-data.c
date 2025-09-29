/**************************************************************
***************************************************************
*** 
***   cmd-data.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains commands for manipulating data.
***
***************************************************************
**************************************************************/

void cmd_bins()
{
	short int data_index=arg_short_int[0];
	int bin_size=arg_int[0];
	
	double *data_list_double;
	
	int *data_list_parameters;
	
	int data_number, i, j, num_bins;
	
	double *bin_averages;
	
	double average, stdev, sum_sq;
	
	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	data_number=*data_list_parameters/sizeof(double);
	
	if (data_number%bin_size!=0) {printf ("The data set must be able to be broken into an integer number of bins.\n"); return;}
	num_bins=data_number/bin_size;
	bin_averages=malloc (sizeof(double)*data_number/bin_size);
	
	for (j=0; j<num_bins; j++)
	{
		*(bin_averages+j)=0;
		for (i=0; i<bin_size; i++)
		{
			*(bin_averages+j)+=*(data_list_double+j*bin_size+i);
		}
		*(bin_averages+j)/=(double)bin_size;
	}
	
	average=0; sum_sq=0;
	for (j=0; j<num_bins; j++)
	{
		average+=*(bin_averages+j);
		sum_sq+=pow(*(bin_averages+j),2);
	}
	average/=(double)num_bins;
	stdev=sqrt(sum_sq/(double)num_bins-pow(average,2));
	
	printf ("bin averages = %.10lf,  standard error = %.10lf\n", average, stdev/sqrt(num_bins));
	
	free (bin_averages);
	
	return;
}

void cmd_coords2d()
{
	short int data_index=arg_short_int[0];
	double r_value=arg_double[0];

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return;

	int i, x;

	int xlen=parameters->xlen;

	// This loop finds the coordinates (x, z) at which (x, r*x) is a point on the interface.
	for (x=0; x<xlen; x++)
	{
		if (*(parameters->height+x)<(int)(r_value*x)) break;
	}

	printf("The middle of the interface is at (x, z) = (%d, %d).\n", x, (int)(r_value*x));

	return;
}

void cmd_coords3d()
{
	short int data_index=arg_short_int[0];
	double r_value=arg_double[0];
	double s_value=arg_double[1];

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return;

	int i, x;
	double y_double;

	int xlen=parameters->xlen;

	// This loop finds the coordinates (x, y, z) at which (x, r*x, s*x) is a point on the interface.
	y_double=0;
	for (x=0; x<xlen; x++)
	{
		if (*(parameters->height+x+((int)(y_double))*xlen)<(int)(s_value*x)) break;
		y_double+=r_value;
	}

	printf("The middle of the interface is at (x, y, z) = (%d, %d, %d).\n", x, (int)(y_double), (int)(*(parameters->height+x+((int)(y_double))*xlen)));

	return;
}

void cmd_dd()
{
	data_delete(data, data_parameters, arg_short_int[0], data_num);
	return;
}

void cmd_dents()
{
	short int data_index=arg_short_int[0];

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return;

	int i, x, y, xplus, yplus;
	
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	int hlen=parameters->hlen;
	int *height_initial=parameters->height_initial;
	int *statex_initial=parameters->statex_initial;
	int *statey_initial=parameters->statey_initial;
	int *index_in=parameters->index_in;
	int *map_in=parameters->map_in;	

	for (i=0; i<xlen*ylen; i++)
	{
		x=i%xlen;
		y=i/xlen;
		
		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==xlen-1) xplus=0;
		else xplus=x+1;
		if (y==ylen-1) yplus=0;
		else yplus=y+1;

		// Check if this is an inner corner.
		// If so, decrease the height by one unit.
		if (*(statex_initial+xplus+y*xlen)>0 && *(statey_initial+x+yplus*xlen)>0)
		{
			if (*(height_initial+i)>0) (*(height_initial+i))--;
			else *(height_initial+i)=hlen-1;

			(*(statex_initial+i))++;
			(*(statey_initial+i))++;

			(*(statex_initial+xplus+y*xlen))--;
			(*(statey_initial+x+yplus*xlen))--;
		}
	}

	return;
}

void cmd_dindex()
{
	// Delete all entries from the parallel index list
	parallel_index(0, 0, 0, data_num);
	return;
}

void cmd_disp()
{
	short int data_index=arg_short_int[0];
	int min=arg_int[0];
	int max=arg_int[1];
	
	double *data_list_double;
	
	void *data_list_parameters;
	
	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	
	int i;
	
	for (i=min; i<=max; i++)
	{
		printf("%lf\n", *(data_list_double+i));
	}
	
	return;
}

void cmd_midint()
{
	short int data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;
	
	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, data_index, data_num)==1) return;
	
	int i, x, length;
	
	length=int_parameters->length;
	
	for (x=0; x<length; x++)
	{
		if (*(int_parameters->height+x+x*length)<x*((double)(int_parameters->delta_x))) break;
	}

	printf("middle of the interface (from integration) = %d\n", x*((double)(int_parameters->delta_x)));
	
	return;
}

void cmd_outin()
{
	short int data_index=arg_short_int[0];
	
	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return;
	
	printf ("outer corners:  %d\n", *(parameters->index_out));
	printf ("inner corners:  %d\n", *(parameters->index_in));
	printf ("steps elapsed:  %lu\n", parameters->steps);
	printf ("time elapsed:   %lf\n", parameters->time);
		
	return;
}

void cmd_sd()
{
	int i;
	
	for (i=0; i<data_num; i++)
	{
		printf("%d  ", i);
		if (data[i]!=0) printf("occupied\n"); else printf("free\n");
	}
	
	return;
}

void cmd_volint()
{
	short int data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;
	
	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, data_index, data_num)==1) return;
	
	int i;
	double volume=0;
	
	for (i=0; i<(int_parameters->length)*(int_parameters->length); i++)
	{
		volume+=*(int_parameters->height+i);
	}
	
	printf("total eroded volume (from integration) = %lf\n", volume*(int_parameters->delta_x)*(int_parameters->delta_x));
	
	return;
}

void cmd_volint4d()
{
	short int data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;
	
	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, data_index, data_num)==1) return;
	
	int i;
	double volume=0;
	
	for (i=0; i<(int_parameters->length)*(int_parameters->length)*(int_parameters->length); i++)
	{
		volume+=*(int_parameters->height+i);
	}
	
	printf("total eroded volume (from integration) = %lf\n", volume);
	
	return;
}

