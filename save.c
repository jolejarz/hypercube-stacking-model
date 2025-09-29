/**************************************************************
***************************************************************
*** 
***   save.c
***
***   Jason Olejarz
***   2010-07-21
***
***   This file contains the routines used by 'calc' to save data after each run of 'evolve'.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int save_diagonal_area(short int command, struct save_diagonal_area_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_diagonal_area_data cmd_data;
	
	static double *diagonal_area;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_diagonal_area_parameters *diagonal_area_parameters;

	static int i;
	static double diagonal_area_value;
	
	switch (command)
	{
		case command_info: {printf("save_diagonal_area\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_diagonal_area_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &diagonal_area, &diagonal_area_parameters, cmd_data.size*sizeof(double), sizeof(struct save_diagonal_area_parameters), cmd_data.data_index_diagonal_area, data_size)==1) return result_error;
			
			diagonal_area_parameters->index=0;
			diagonal_area_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			diagonal_area_value=0;
			for (i=0; i<(parameters->xlen); i++)
			{
				if (*(parameters->height+i+i*(parameters->xlen))==0) break;
				diagonal_area_value+=*(parameters->height+i+i*(parameters->xlen));
			}
		
			*(diagonal_area+(diagonal_area_parameters->index))=diagonal_area_value;
			
			(diagonal_area_parameters->index)++;
			diagonal_area_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_seed(short int command, struct save_seed_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_seed_data cmd_data;
	
	static double *seed_low;
	static double *seed_high;

	static long int num2pow32=pow(2,16)*pow(2,16);

	static struct rng_struct *rng_struct;
	static int *rng_struct_size;
	
	static struct save_seed_parameters *seed_low_parameters;
	static struct save_seed_parameters *seed_high_parameters;

	switch (command)
	{
		case command_info: {printf("save_seed\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_seed_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &rng_struct, &rng_struct_size, cmd_data.data_index_rng, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &seed_low, &seed_low_parameters, cmd_data.size*sizeof(double), sizeof(struct save_seed_parameters), cmd_data.data_index_low, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &seed_high, &seed_high_parameters, cmd_data.size*sizeof(double), sizeof(struct save_seed_parameters), cmd_data.data_index_high, data_size)==1) return result_error;
			
			seed_low_parameters->index=0;
			seed_low_parameters->size=0;
			
			seed_high_parameters->index=0;
			seed_high_parameters->size=0;

			return result_continue;
		}
		case command_run:
		{
			*(seed_low+(seed_low_parameters->index))=(double)((rng_struct->seed)%num2pow32);
			*(seed_high+(seed_high_parameters->index))=(double)((rng_struct->seed)/num2pow32);
			
			(seed_low_parameters->index)++;
			(seed_high_parameters->index)++;
			
			seed_low_parameters->size+=sizeof(double);
			seed_high_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_volume(short int command, struct save_volume_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_volume_data cmd_data;
	
	static double *volume;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_volume_parameters *volume_parameters;

	static int i;
	static unsigned long int volume_count;
	
	switch (command)
	{
		case command_info: {printf("save_volume\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_volume_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &volume, &volume_parameters, cmd_data.size*sizeof(double), sizeof(struct save_volume_parameters), cmd_data.data_index_volume, data_size)==1) return result_error;
			
			volume_parameters->index=0;
			volume_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			volume_count=0;
			for (i=0; i<(parameters->xlen)*(parameters->ylen); i++)
			{
				volume_count+=*(parameters->height+i);
			}
		
			*(volume+(volume_parameters->index))=(double)volume_count;
			
			(volume_parameters->index)++;
			volume_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_volume_4d(short int command, struct save_volume_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_volume_data cmd_data;
	
	static double *volume;

	static struct run_parameters_4d *parameters;
	static int *parameters_size;
	
	static struct save_volume_parameters *volume_parameters;

	static int i;
	static unsigned long int volume_count;
	
	switch (command)
	{
		case command_info: {printf("save_volume_4d\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_volume_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &volume, &volume_parameters, cmd_data.size*sizeof(double), sizeof(struct save_volume_parameters), cmd_data.data_index_volume, data_size)==1) return result_error;
			
			volume_parameters->index=0;
			volume_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			volume_count=0;
			for (i=0; i<(parameters->xlen)*(parameters->ylen)*(parameters->qlen); i++)
			{
				volume_count+=*(parameters->height+i);
			}
		
			*(volume+(volume_parameters->index))=(double)volume_count;
			
			(volume_parameters->index)++;
			volume_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

