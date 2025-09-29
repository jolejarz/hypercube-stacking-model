/**************************************************************
***************************************************************
*** 
***   step.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains the routines used by 'evolve' for intermediate processing.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int step_area(short int command, struct step_area_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_area_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *area;
	static struct step_area_parameters *area_parameters;
	
	static int x, y, xlen;
	static int area_value;
	
	switch (command)
	{
		case command_info: {printf("step_area\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_area_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &area, &area_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_area_parameters), cmd_data.data_index_area, data_size)==1) return result_error;
			
			area_parameters->size=0;
			area_parameters->index=0;

			area_value=0;
			
			xlen=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (p->move==2)
			{
				x=(p->coordinate)%xlen;
				y=(p->coordinate)/xlen;
				
				// If the increased height was on either edge of the interface, update the total area.
				if (x==0) area_value++;
				if (y==0) area_value++;
				
				// If the increased height was initially at h=0, update the total area.
				if (*(p->height+p->coordinate)==1) area_value++;
			}
			else if (p->move==1)
			{
				x=(p->coordinate)%xlen;
				y=(p->coordinate)/xlen;
				
				// If the increased height was on either edge of the interface, update the total area.
				if (x==0) area_value--;
				if (y==0) area_value--;
				
				// If the increased height was initially at h=1, update the total area.
				if (*(p->height+p->coordinate)==0) area_value--;
			}

			if (*trigger==1)
			{
				*(area+(area_parameters->index))=(double)(area_value);
				
				(area_parameters->size)+=sizeof(double);
				(area_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_average_velocity(short int command, struct step_average_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_average_velocity_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	static unsigned long int old_steps;
	static double time_elapsed;
	
	static short int save_begun;
	
	switch (command)
	{
		case command_info: {printf("step_average_velocity\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_average_velocity_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;

			old_steps=0;
			time_elapsed=0;
			
			if (cmd_data.all_times==1)
			{
				old_steps=0;
				time_elapsed=0;

				save_begun=1;
			}
			else save_begun=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1 && save_begun==0)
			{
				old_steps=p->steps;
				time_elapsed=p->time;

				save_begun=1;
			}

			return result_continue;
		}
		case command_end:
		{
			printf("%lf\n", (p->steps-old_steps)/(double)((p->xlen)*(p->ylen))/((p->time)-time_elapsed));
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_corner(short int command, struct step_corner_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_corner_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	
	static int hlen, xlen, ylen;
	
	switch (command)
	{
		case command_info: {printf("step_corner\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_corner_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			// Disable the height at (xmax, ymax) from decreasing.
			// This gives proper behavior if the corner is evolved.
			*(p->index_in)=0;
			
			hlen=p->hlen;
			xlen=p->xlen;
			ylen=p->ylen;
			
			return result_continue;
		}
		case command_run:
		{
			// If the interface erodes beyond the allocated dimensions,
			// then stop the simulation.
			if (*(p->height)==hlen-1 || *(p->height+xlen-2)==1 || *(p->height+(ylen-2)*xlen)==1)
			{
				printf("\nError: The interface attempted to evolve beyond the allocated dimensions.\n");
				return result_done;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_corner_4d(short int command, struct step_corner_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_corner_data cmd_data;
	
	static struct run_parameters_4d *p;
	static int *p_size;
	
	static int hlen, xlen, ylen, qlen;
	
	switch (command)
	{
		case command_info: {printf("step_corner_4d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_corner_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			// Disable the height at (xmax, ymax, qmax) from decreasing.
			// This gives proper behavior if the corner is evolved.
			*(p->index_in)=0;
			
			hlen=p->hlen;
			xlen=p->xlen;
			ylen=p->ylen;
			qlen=p->qlen;
			
			return result_continue;
		}
		case command_run:
		{
			// If the interface erodes beyond the allocated dimensions,
			// then stop the simulation.
			if (*(p->height)==hlen-1 || *(p->height+xlen-2)==1 || *(p->height+(ylen-2)*xlen)==1 || *(p->height+(qlen-2)*xlen*ylen)==1)
			{
				printf("\nError: The interface attempted to evolve beyond the allocated dimensions.\n");
				return result_done;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_corner_5d(short int command, struct step_corner_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_corner_data cmd_data;
	
	static struct run_parameters_5d *p;
	static int *p_size;
	
	static int hlen, xlen, ylen, qlen, rlen;
	
	switch (command)
	{
		case command_info: {printf("step_corner_5d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_corner_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			// Disable the height at (xmax, ymax, qmax, rmax) from decreasing.
			// This gives proper behavior if the corner is evolved.
			*(p->index_in)=0;
			
			hlen=p->hlen;
			xlen=p->xlen;
			ylen=p->ylen;
			qlen=p->qlen;
			rlen=p->rlen;
			
			return result_continue;
		}
		case command_run:
		{
			// If the interface erodes beyond the allocated dimensions,
			// then stop the simulation.
			if (*(p->height)==hlen-1 || *(p->height+xlen-2)==1 || *(p->height+(ylen-2)*xlen)==1 || *(p->height+(qlen-2)*xlen*ylen)==1 || *(p->height+(rlen-2)*xlen*ylen*qlen)==1)
			{
				printf("\nError: The interface attempted to evolve beyond the allocated dimensions.\n");
				return result_done;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_inner_corners(short int command, struct step_inner_corners_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_inner_corners_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *inner_corners;
	static struct step_inner_corners_parameters *inner_corners_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_inner_corners\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_inner_corners_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &inner_corners, &inner_corners_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_inner_corners_parameters), cmd_data.data_index_inner_corners, data_size)==1) return result_error;
			
			inner_corners_parameters->size=0;
			inner_corners_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(inner_corners+(inner_corners_parameters->index))=(double)(*(p->index_in));
				
				(inner_corners_parameters->size)+=sizeof(double);
				(inner_corners_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_inner_corners_test(short int command, struct step_inner_corners_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_inner_corners_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *inner_corners;
	static struct step_inner_corners_parameters *inner_corners_parameters;
	
	static int x, y, xplus, yplus, xlen, ylen, inner;
	
	switch (command)
	{
		case command_info: {printf("step_inner_corners_test\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_inner_corners_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &inner_corners, &inner_corners_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_inner_corners_parameters), cmd_data.data_index_inner_corners, data_size)==1) return result_error;
			
			inner_corners_parameters->size=0;
			inner_corners_parameters->index=0;

			xlen=p->xlen;
			ylen=p->ylen;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				inner=0;
				
				for (y=0; y<ylen; y++)
				{
					for (x=0; x<xlen; x++)
					{
						// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
						if (x==xlen-1) xplus=0;
						else xplus=x+1;
						if (y==ylen-1) yplus=0;
						else yplus=y+1;
						
						// Check if this is an inner corner.
						if (*(p->statex+xplus+y*xlen)>0 && *(p->statey+x+yplus*xlen)>0) inner++;
					}
				}
				
				*(inner_corners+(inner_corners_parameters->index))=(double)inner;
				
				(inner_corners_parameters->size)+=sizeof(double);
				(inner_corners_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_outer_corners(short int command, struct step_outer_corners_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_outer_corners_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *outer_corners;
	static struct step_outer_corners_parameters *outer_corners_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_outer_corners\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_outer_corners_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &outer_corners, &outer_corners_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_outer_corners_parameters), cmd_data.data_index_outer_corners, data_size)==1) return result_error;
			
			outer_corners_parameters->size=0;
			outer_corners_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(outer_corners+(outer_corners_parameters->index))=(double)(*(p->index_out));
				
				(outer_corners_parameters->size)+=sizeof(double);
				(outer_corners_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_outer_corners_test(short int command, struct step_outer_corners_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_outer_corners_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *outer_corners;
	static struct step_outer_corners_parameters *outer_corners_parameters;
	
	static int x, y, xminus, yminus, xlen, ylen, outer;

	switch (command)
	{
		case command_info: {printf("step_outer_corners_test\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_outer_corners_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &outer_corners, &outer_corners_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_outer_corners_parameters), cmd_data.data_index_outer_corners, data_size)==1) return result_error;
			
			outer_corners_parameters->size=0;
			outer_corners_parameters->index=0;

			xlen=p->xlen;
			ylen=p->ylen;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				outer=0;
				
				for (y=0; y<ylen; y++)
				{
					for (x=0; x<xlen; x++)
					{
						// Check if this is an outer corner.
						if (*(p->statex+x+y*xlen)>0 && *(p->statey+x+y*xlen)>0) outer++;
					}
				}
				
				*(outer_corners+(outer_corners_parameters->index))=(double)outer;
				
				(outer_corners_parameters->size)+=sizeof(double);
				(outer_corners_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_d2hdx2_middle(short int command, struct step_d2hdx2_middle_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_d2hdx2_middle_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *curvature;
	static struct step_d2hdx2_middle_parameters *curvature_parameters;

	static int x, xlen;
	static double value;
	
	switch (command)
	{
		case command_info: {printf("step_d2hdx2_middle\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_d2hdx2_middle_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &curvature, &curvature_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_d2hdx2_middle_parameters), cmd_data.data_index_curvature, data_size)==1) return result_error;
			
			curvature_parameters->size=0;
			curvature_parameters->index=0;

			xlen=p->xlen;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				if (p->time<10000) value=0;
				else
				{
					// This loop finds the value of x for which x=y=z is close to being a point on the interface.
					for (x=0; x<xlen; x++)
					{
						if (*(p->height+x+x*xlen)<x) break;
					}
					value=(*(p->height+(x-10)+x*xlen)+*(p->height+(x+10)+x*xlen)-2*(*(p->height+x+x*xlen)))/(double)10;
				}
				
				*(curvature+(curvature_parameters->index))=value;
				
				(curvature_parameters->size)+=sizeof(double);
				(curvature_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_diagonal_area(short int command, struct step_diagonal_area_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_diagonal_area_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *diagonal_area;
	static struct step_diagonal_area_parameters *diagonal_area_parameters;
	
	static int i, xlen;
	static int diagonal_area_value;
	
	switch (command)
	{
		case command_info: {printf("step_diagonal_area\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_diagonal_area_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &diagonal_area, &diagonal_area_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_diagonal_area_parameters), cmd_data.data_index_diagonal_area, data_size)==1) return result_error;
			
			diagonal_area_parameters->size=0;
			diagonal_area_parameters->index=0;

			xlen=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				diagonal_area_value=0;
				for (i=0; i<xlen; i++)
				{
					if (*(p->height+i+i*xlen)==0) break;
					diagonal_area_value+=*(p->height+i+i*xlen);
				}
			
				*(diagonal_area+(diagonal_area_parameters->index))=(double)diagonal_area_value;
				
				(diagonal_area_parameters->index)++;
				diagonal_area_parameters->size+=sizeof(double);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_flips(short int command, struct step_flips_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_flips_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static int *flips;
	static struct step_flips_parameters *flips_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_flips\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_flips_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &flips, &flips_parameters, (int)(sizeof(int)*2*(p->maxsteps)), sizeof(struct step_flips_parameters), cmd_data.data_index_flips, data_size)==1) return result_error;

			flips_parameters->size=0;
			flips_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			*(flips+(flips_parameters->index)++)=p->coordinate;
			*(flips+(flips_parameters->index)++)=p->move;

			flips_parameters->size+=2*sizeof(int);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_hmax(short int command, struct step_hmax_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_hmax_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *hmax;
	static struct step_hmax_parameters *hmax_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_hmax\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_hmax_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &hmax, &hmax_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_hmax_parameters), cmd_data.data_index_hmax, data_size)==1) return result_error;
			
			hmax_parameters->size=0;
			hmax_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(hmax+(hmax_parameters->index))=(double)(*(p->height));
				
				(hmax_parameters->size)+=sizeof(double);
				(hmax_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_middle2d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_middle_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *middle;
	static struct step_middle_parameters *middle_parameters;
	
	static int x, xlen;
	
	switch (command)
	{
		case command_info: {printf("step_middle2d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_middle_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &middle, &middle_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_middle_parameters), cmd_data.data_index_middle, data_size)==1) return result_error;
			
			middle_parameters->size=0;
			middle_parameters->index=0;

			xlen=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// This loop finds the value of x for which x=y is close to being a point on the interface.
				for (x=0; x<xlen; x++)
				{
					if (*(p->height+x)<=x) break;
				}
				
				*(middle+(middle_parameters->index))=(double)x;
				
				(middle_parameters->size)+=sizeof(double);
				(middle_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_middle3d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_middle_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *middle;
	static struct step_middle_parameters *middle_parameters;
	
	static int x, xlen;
	
	switch (command)
	{
		case command_info: {printf("step_middle3d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_middle_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &middle, &middle_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_middle_parameters), cmd_data.data_index_middle, data_size)==1) return result_error;
			
			middle_parameters->size=0;
			middle_parameters->index=0;

			xlen=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// This loop finds the value of x for which x=y=z is close to being a point on the interface.
				for (x=0; x<xlen; x++)
				{
					if (*(p->height+x+x*xlen)<=x) break;
				}
				
				*(middle+(middle_parameters->index))=(double)x;
				
				(middle_parameters->size)+=sizeof(double);
				(middle_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_middle4d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_middle_data cmd_data;
	
	static struct run_parameters_4d *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *middle;
	static struct step_middle_parameters *middle_parameters;
	
	static int x, xlen, xlen2;
	
	switch (command)
	{
		case command_info: {printf("step_middle4d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_middle_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &middle, &middle_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_middle_parameters), cmd_data.data_index_middle, data_size)==1) return result_error;
			
			middle_parameters->size=0;
			middle_parameters->index=0;

			xlen=p->xlen;
			xlen2=xlen*xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// This loop finds the value of x for which x=y=q=z is close to being a point on the interface.
				for (x=0; x<xlen; x++)
				{
					if (*(p->height+x+x*xlen+x*xlen2)<=x) break;
				}
				
				*(middle+(middle_parameters->index))=(double)x;
				
				(middle_parameters->size)+=sizeof(double);
				(middle_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_middle4d3(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_middle_data cmd_data;
	
	static struct run_parameters_4d *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *middle;
	static struct step_middle_parameters *middle_parameters;
	
	static unsigned long int x, length;
	
	static unsigned long int offset;
	
	switch (command)
	{
		case command_info: {printf("step_middle4d3\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_middle_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &middle, &middle_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_middle_parameters), cmd_data.data_index_middle, data_size)==1) return result_error;
			
			middle_parameters->size=0;
			middle_parameters->index=0;

			length=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// This loop finds the value of x for which x=y=q=z is close to being a point on the interface.
				for (x=0; x<length; x++)
				{
					offset=(x*(x-1)*(2*x-1)/6-x*(x-1)*(2*length+1)/2+x*length*(length+1))/2+x*(length-x)+(x-1)*x/2+x;
					if (*(p->height+offset)<=x) break;
				}
				
				*(middle+(middle_parameters->index))=(double)x;
				
				(middle_parameters->size)+=sizeof(double);
				(middle_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_middle5d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_middle_data cmd_data;
	
	static struct run_parameters_5d *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *middle;
	static struct step_middle_parameters *middle_parameters;
	
	static int x, xlen, xlen2, xlen3;
	
	switch (command)
	{
		case command_info: {printf("step_middle5d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_middle_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &middle, &middle_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_middle_parameters), cmd_data.data_index_middle, data_size)==1) return result_error;
			
			middle_parameters->size=0;
			middle_parameters->index=0;

			xlen=p->xlen;
			xlen2=xlen*xlen;
			xlen3=xlen2*xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// This loop finds the value of x for which x=y=q=r=z is close to being a point on the interface.
				for (x=0; x<xlen; x++)
				{
					if (*(p->height+x+x*xlen+x*xlen2+x*xlen3)<=x) break;
				}
				
				*(middle+(middle_parameters->index))=(double)x;
				
				(middle_parameters->size)+=sizeof(double);
				(middle_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_middle_stop(short int command, struct step_middle_stop_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_middle_stop_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *time;
	static struct step_time_parameters *time_parameters;
	
	static int x, xlen;
	
	switch (command)
	{
		case command_info: {printf("step_middle_stop\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_middle_stop_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &time, &time_parameters, sizeof(double), sizeof(struct step_middle_stop_parameters), cmd_data.data_index_time, data_size)==1) return result_error;
			
			time_parameters->size=0;
			time_parameters->index=0;
			
			x=cmd_data.middle;
			xlen=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*(p->height+x+x*xlen)==x)
			{
				*(time+(time_parameters->index))=p->time;
				
				(time_parameters->size)+=sizeof(double);
				(time_parameters->index)++;

				return result_done;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_plane_time(short int command, struct step_plane_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_plane_time_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static struct step_plane_time_parameters *plane_time_parameters;
	
	static int y_value;

	static char plane_time_data_filename[32];
	static FILE *plane_time_data;
	
	switch (command)
	{
		case command_info: {printf("step_plane_time\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_plane_time_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				for (y_value=0; y_value<(int)p->time; y_value+=(int)(cmd_data.y_over_t_increment*trigger_parameters->time))
				{
					sprintf (plane_time_data_filename, "plane_%d_%.3lf.dat", (int)p->time, y_value/p->time);
					plane_time_data=fopen(plane_time_data_filename, "w");
					plot_plane_time (p, y_value, plane_time_data);
					fclose (plane_time_data);
				}
			}
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_point2d(short int command, struct step_point2d_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_point2d_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *height;
	static struct step_middle_parameters *height_parameters;
	
	static int x, xlen;
	
	switch (command)
	{
		case command_info: {printf("step_point2d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_point2d_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &height, &height_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_point3d_parameters), cmd_data.data_index_height, data_size)==1) return result_error;
			
			height_parameters->size=0;
			height_parameters->index=0;

			xlen=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// This loop finds the coordinates (x, z) at which (x, r*x) is a point on the interface.
				for (x=0; x<xlen; x++)
				{
					if (*(p->height+x)<(int)(cmd_data.r_value*x)) break;
				}
				
				*(height+(height_parameters->index))=(double)(*(p->height+x));
				
				(height_parameters->size)+=sizeof(double);
				(height_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_point3d(short int command, struct step_point3d_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_point3d_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *height;
	static struct step_middle_parameters *height_parameters;
	
	static int x, xlen;
	
	static double y_double;
	
	switch (command)
	{
		case command_info: {printf("step_point3d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_point3d_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &height, &height_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_point3d_parameters), cmd_data.data_index_height, data_size)==1) return result_error;
			
			height_parameters->size=0;
			height_parameters->index=0;

			xlen=p->xlen;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// This loop finds the coordinates (x, y, z) at which (x, r*x, s*x) is a point on the interface.
				y_double=0;
				for (x=0; x<xlen; x++)
				{
					if (*(p->height+x+((int)(y_double))*xlen)<(int)(cmd_data.s_value*x)) break;
					y_double+=cmd_data.r_value;
				}
				
				*(height+(height_parameters->index))=(double)(*(p->height+x+((int)(y_double))*xlen));
				
				(height_parameters->size)+=sizeof(double);
				(height_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_stop_corners(short int command, struct step_stop_corners_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_stop_corners_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	
	switch (command)
	{
		case command_info: {printf("step_stop_corners\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_stop_corners_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_run:
		{
			if (*(p->index_out)-*(p->index_in)<cmd_data.outer_minus_inner)
			{
				printf("%d %d\n",cmd_data.outer_minus_inner,*(p->index_out)-*(p->index_in));
				return result_done;
			}
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_survival(short int command, struct step_survival_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_survival_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *time;
	static struct step_survival_parameters *time_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_survival_corners\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_survival_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &time, &time_parameters, sizeof(double)*(cmd_data.max_num_times), sizeof(struct step_time_parameters), cmd_data.data_index_time, data_size)==1) return result_error;
			
			time_parameters->size=0;
			time_parameters->index=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (*(p->height)==1 && p->move==2 && p->coordinate==0)
			{
				*(time+(time_parameters->index))=p->time;

				(time_parameters->size)+=sizeof(double);
				(time_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_time(short int command, struct step_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_time_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *time;
	static struct step_time_parameters *time_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_time\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_time_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &time, &time_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_time_parameters), cmd_data.data_index_time, data_size)==1) return result_error;
			
			time_parameters->size=0;
			time_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(time+(time_parameters->index))=p->time;
				
				(time_parameters->size)+=sizeof(double);
				(time_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_total_velocity(short int command, struct step_total_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_total_velocity_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *velocity;
	static struct step_total_velocity_parameters *velocity_parameters;

	static int area;
		
	switch (command)
	{
		case command_info: {printf("step_total_velocity\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_total_velocity_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &velocity, &velocity_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_velocity_parameters), cmd_data.data_index_velocity, data_size)==1) return result_error;
			
			velocity_parameters->size=0;
			velocity_parameters->index=0;

			area=(p->xlen)*(p->ylen);
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(velocity+(velocity_parameters->index))=(p->steps)/(double)area/(p->time);
				
				(velocity_parameters->size)+=sizeof(double);
				(velocity_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_steps(short int command, struct step_trigger_steps_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_steps_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	switch (command)
	{
		case command_begin:
		{
			if (cmd_data.progress==1)
			{
				printf("Processing...      ");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_end:
		{
			if (cmd_data.progress==1)
			{
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDone                    \n");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_info: {printf("step_trigger_steps\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_steps_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=(p->maxsteps)/cmd_data.steps_interval;
			trigger_parameters->current=0;
			trigger_parameters->steps=0;
			
			return result_continue;
		}
		case command_run:
		{
			if ((p->steps)%cmd_data.steps_interval==0)
			{
				*trigger=1;
				(trigger_parameters->current)++;
				trigger_parameters->steps=p->steps;

				if (cmd_data.progress==1)
				{
					printf("\b\b\b\b\b");
					printf("%4.1lf%%", 100*(trigger_parameters->current)/(double)(trigger_parameters->total));
					fflush(stdout);
				}
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_time(short int command, struct step_trigger_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_time_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	static double time_elapsed;
	
	switch (command)
	{
		case command_begin:
		{
			if (cmd_data.progress==1)
			{
				printf("Processing...      ");
				fflush(stdout);
			}

			trigger_parameters->current=0;
			trigger_parameters->time=0;
			
			time_elapsed=0;

			return result_continue;
		}
		case command_end:
		{
			if (cmd_data.progress==1)
			{
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDone                    \n");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_info: {printf("step_trigger_time\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_time_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=(int)((p->maxtime)/cmd_data.time_interval);
			
			return result_continue;
		}
		case command_run:
		{
			if (p->time-time_elapsed>cmd_data.time_interval)
			{
				*trigger=1;
				(trigger_parameters->current)++;
				time_elapsed+=cmd_data.time_interval;
				trigger_parameters->time=time_elapsed;

				if (cmd_data.progress==1)
				{
					printf("\b\b\b\b\b");
					printf("%4.1lf%%", 100*(trigger_parameters->current)/(double)(trigger_parameters->total));
					fflush(stdout);
				}
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_time_once(short int command, struct step_trigger_time_once_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_time_once_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	static short int already_triggered;

	switch (command)
	{
		case command_info: {printf("step_trigger_time_once\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_time_once_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=cmd_data.runs;
			trigger_parameters->current=0;
			
			return result_continue;
		}
		case command_begin:
		{
			already_triggered=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (p->time>=cmd_data.time && already_triggered==0)
			{
				*trigger=1;
				(trigger_parameters->current)++;

				already_triggered=1;
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_velocity(short int command, struct step_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_velocity_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *velocity;
	static struct step_velocity_parameters *velocity_parameters;
	
	static unsigned long int old_steps;
	static double time_elapsed;
	
	switch (command)
	{
		case command_info: {printf("step_velocity\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_velocity_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &velocity, &velocity_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_velocity_parameters), cmd_data.data_index_velocity, data_size)==1) return result_error;
			
			velocity_parameters->size=0;
			velocity_parameters->index=0;

			old_steps=0;
			time_elapsed=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(velocity+(velocity_parameters->index))=(p->steps-old_steps)/(double)((p->xlen)*(p->ylen))/((p->time)-time_elapsed);
				
				old_steps=p->steps;
				time_elapsed=p->time;
				
				(velocity_parameters->size)+=sizeof(double);
				(velocity_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_velocity_4d(short int command, struct step_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_velocity_data cmd_data;
	
	static struct run_parameters_4d *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *velocity;
	static struct step_velocity_parameters *velocity_parameters;
	
	static unsigned long int old_steps;
	static double time_elapsed;
	
	switch (command)
	{
		case command_info: {printf("step_velocity_4d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_velocity_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &velocity, &velocity_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_velocity_parameters), cmd_data.data_index_velocity, data_size)==1) return result_error;
			
			velocity_parameters->size=0;
			velocity_parameters->index=0;

			old_steps=0;
			time_elapsed=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(velocity+(velocity_parameters->index))=(p->steps-old_steps)/(double)((p->xlen)*(p->ylen)*(p->qlen))/((p->time)-time_elapsed);
				
				old_steps=p->steps;
				time_elapsed=p->time;
				
				(velocity_parameters->size)+=sizeof(double);
				(velocity_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_velocity_5d(short int command, struct step_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_velocity_data cmd_data;
	
	static struct run_parameters_5d *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *velocity;
	static struct step_velocity_parameters *velocity_parameters;
	
	static unsigned long int old_steps;
	static double time_elapsed;
	
	switch (command)
	{
		case command_info: {printf("step_velocity_5d\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_velocity_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &velocity, &velocity_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_velocity_parameters), cmd_data.data_index_velocity, data_size)==1) return result_error;
			
			velocity_parameters->size=0;
			velocity_parameters->index=0;

			old_steps=0;
			time_elapsed=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(velocity+(velocity_parameters->index))=(p->steps-old_steps)/(double)((p->xlen)*(p->ylen)*(p->qlen)*(p->rlen))/((p->time)-time_elapsed);
				
				old_steps=p->steps;
				time_elapsed=p->time;
				
				(velocity_parameters->size)+=sizeof(double);
				(velocity_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_volume(short int command, struct step_volume_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_volume_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *volume;
	static struct step_volume_parameters *volume_parameters;
	
	static unsigned long int volume_value;
	
	switch (command)
	{
		case command_info: {printf("step_volume\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_volume_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &volume, &volume_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_volume_parameters), cmd_data.data_index_volume, data_size)==1) return result_error;
			
			volume_parameters->size=0;
			volume_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			volume_value=0;

			return result_continue;
		}
		case command_run:
		{
			if (p->move==2) volume_value++;
			else if (p->move==1) volume_value--;

			if (*trigger==1)
			{
				*(volume+(volume_parameters->index))=(double)(volume_value);
				
				(volume_parameters->size)+=sizeof(double);
				(volume_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_width(short int command, struct step_width_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_width_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *width;
	static struct step_width_parameters *width_parameters;
	
	static double *total_height;
	static double *flat_slope;
	
	static int i, area;
	static double stdev;
	
	static unsigned long int old_steps;
	
	switch (command)
	{
		case command_info: {printf("step_width\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_width_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &width, &width_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_width_parameters), cmd_data.data_index_width, data_size)==1) return result_error;
			
			width_parameters->size=0;
			width_parameters->index=0;

			area=(p->xlen)*(p->ylen);
			
			total_height=(double*)malloc(sizeof(double)*area);
			flat_slope=(double*)malloc(sizeof(double)*area);
			
			old_steps=0;
			
			return result_continue;
		}
		case command_begin:
		{
			for (i=0; i<area; i++)
			{
				*(total_height+i)=(double)(*(p->height_initial+i));
				*(flat_slope+i)=(double)(*(p->height_initial+i));
			}

			return result_continue;
		}
		case command_run:
		{
			if (p->move==2) (*(total_height+p->coordinate))++;
			else if (p->move==1) (*(total_height+p->coordinate))--;

			if (*trigger==1)
			{
				stdev=0;
				for (i=0; i<area; i++)
				{
					*(flat_slope+i)+=((p->steps)-old_steps)/(double)area;
					stdev+=(*(total_height+i)-*(flat_slope+i))*(*(total_height+i)-*(flat_slope+i));
				}
				stdev=sqrt(stdev/(double)area);
				
				old_steps=p->steps;
				
				*(width+(width_parameters->index))=stdev;
				
				(width_parameters->size)+=sizeof(double);
				(width_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_width_new(short int command, struct step_width_new_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_width_new_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *width;
	static struct step_width_new_parameters *width_parameters;
	
	static double *total_height;
	static double *flat_slope;
	
	static int i, area;
	static double stdev;
	
	static unsigned long int old_steps, count;
	
	switch (command)
	{
		case command_info: {printf("step_width\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_width_new_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &width, &width_parameters, sizeof(double)*(cmd_data.runs), sizeof(struct step_width_new_parameters), cmd_data.data_index_width, data_size)==1) return result_error;
			
			width_parameters->size=0;
			width_parameters->index=0;

			area=(p->xlen)*(p->ylen);
			
			total_height=(double*)malloc(sizeof(double)*area);
			flat_slope=(double*)malloc(sizeof(double)*area);
			
			return result_continue;
		}
		case command_begin:
		{
			for (i=0; i<area; i++)
			{
				*(total_height+i)=(double)(*(p->height_initial+i));
				*(flat_slope+i)=(double)(*(p->height_initial+i));
			}
			
			old_steps=0;
			stdev=0;
			count=0;

			return result_continue;
		}
		case command_run:
		{
			if (p->move==2) (*(total_height+p->coordinate))++;
			else if (p->move==1) (*(total_height+p->coordinate))--;

			if (*trigger==1)
			{
				for (i=0; i<area; i++)
				{
					*(flat_slope+i)+=((p->steps)-old_steps)/(double)area;
					stdev+=(*(total_height+i)-*(flat_slope+i))*(*(total_height+i)-*(flat_slope+i));
					count++;
				}
				
				old_steps=p->steps;
			}

			return result_continue;
		}
		case command_end:
		{
			*(width+(width_parameters->index))=sqrt(stdev/(double)count);
			
			(width_parameters->size)+=sizeof(double);
			(width_parameters->index)++;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_xme_and_yme(short int command, struct step_xme_and_yme_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_xme_and_yme_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *prob;
	static struct step_xme_and_yme_parameters *prob_parameters;
	
	static int i, count;
	
	switch (command)
	{
		case command_info: {printf("step_xme_and_yme\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_xme_and_yme_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &prob, &prob_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_xme_and_yme_parameters), cmd_data.data_index_prob, data_size)==1) return result_error;
			
			prob_parameters->size=0;
			prob_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// Loop through the height configuration.
				// At each height, check if both the height at (x-1, y)
				// and the height at (x, y-1) are equal to the height at (x, y).
				count=0;
				for (i=0; i<(p->xlen)*(p->ylen); i++)
				{
					if (*(p->statex+i)==0 && *(p->statey+i)==0) count++;
				}
				
				*(prob+(prob_parameters->index))=count/(double)((p->xlen)*(p->ylen));
				
				(prob_parameters->size)+=sizeof(double);
				(prob_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_x_minus_greater(short int command, struct step_x_minus_greater_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_x_minus_greater_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *prob;
	static struct step_x_minus_greater_parameters *prob_parameters;
	
	static int i, count;
	
	switch (command)
	{
		case command_info: {printf("step_x_minus_greater\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_x_minus_greater_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &prob, &prob_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_x_minus_greater_parameters), cmd_data.data_index_prob, data_size)==1) return result_error;
			
			prob_parameters->size=0;
			prob_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// Loop through the height configuration.
				// At each height, check if the height at (x-1, y) is greater than
				// the height at (x, y).
				count=0;
				for (i=0; i<(p->xlen)*(p->ylen); i++)
				{
					if (*(p->statex+i)>0) count++;
				}
				
				*(prob+(prob_parameters->index))=count/(double)((p->xlen)*(p->ylen));
				
				(prob_parameters->size)+=sizeof(double);
				(prob_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_xmg_or_ymg(short int command, struct step_xmg_or_ymg_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_xmg_or_ymg_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *prob;
	static struct step_xmg_or_ymg_parameters *prob_parameters;
	
	static int i, count;
	
	switch (command)
	{
		case command_info: {printf("step_xmg_or_ymg\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_xmg_or_ymg_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &prob, &prob_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_xmg_or_ymg_parameters), cmd_data.data_index_prob, data_size)==1) return result_error;
			
			prob_parameters->size=0;
			prob_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// Loop through the height configuration.
				// At each height, check if the height at (x-1, y) or the height
				// at (x, y-1) is greater than the height at (x, y).
				count=0;
				for (i=0; i<(p->xlen)*(p->ylen); i++)
				{
					if (*(p->statex+i)>0 || *(p->statey+i)>0) count++;
				}
				
				*(prob+(prob_parameters->index))=count/(double)((p->xlen)*(p->ylen));
				
				(prob_parameters->size)+=sizeof(double);
				(prob_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_y_minus_greater(short int command, struct step_y_minus_greater_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_y_minus_greater_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *prob;
	static struct step_y_minus_greater_parameters *prob_parameters;
	
	static int i, count;
	
	switch (command)
	{
		case command_info: {printf("step_y_minus_greater\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_y_minus_greater_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &prob, &prob_parameters, sizeof(double)*(trigger_parameters->total), sizeof(struct step_y_minus_greater_parameters), cmd_data.data_index_prob, data_size)==1) return result_error;
			
			prob_parameters->size=0;
			prob_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				// Loop through the height configuration.
				// At each height, check if the height at (x, y-1) is greater than
				// the height at (x, y).
				count=0;
				for (i=0; i<(p->xlen)*(p->ylen); i++)
				{
					if (*(p->statey+i)>0) count++;
				}
				
				*(prob+(prob_parameters->index))=count/(double)((p->xlen)*(p->ylen));
				
				(prob_parameters->size)+=sizeof(double);
				(prob_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

