/**************************************************************
***************************************************************
*** 
***   cmd-plot.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains graphics commands.
***
***************************************************************
**************************************************************/

void cmd_an2d()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	int steps_interval=arg_int[0];
	double time_delay=arg_double[0];
	short int save_frames=arg_short_int[2];

	struct run_parameters *parameters;
	int *parameters_size;

	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return result_error;
	if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");
	FILE *plot=fopen(arg_string[1], "w");

	animate_2d (parameters, flips, steps_interval, time_delay, save_frames, arg_string[0], data, plot);
	
	fclose (plot);
	fclose (data);

	return;
}

void cmd_an3d()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	double edge_alpha=arg_double[0];
	int steps_interval=arg_int[0];
	double time_delay=arg_double[1];
	short int save_frames=arg_short_int[2];
	
	struct run_parameters *parameters;
	int *parameters_size;

	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return result_error;
	if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");
	FILE *plot=fopen(arg_string[1], "w");

	animate_3d (parameters, flips, edge_alpha, steps_interval, time_delay, save_frames, arg_string[0], data, plot);
	
	fclose (plot);
	fclose (data);

	return;
}

void cmd_an3drt()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	double edge_alpha=arg_double[0];
	short int data_index_time=arg_short_int[2];
	double beginning_time=arg_double[1];
	double ending_time=arg_double[2];
	double time_interval=arg_double[3];
	double time_delay=arg_double[4];
	short int save_frames=arg_short_int[3];

	struct run_parameters *parameters;
	int *parameters_size;

	int *flips;
	int *flips_size;
	
	double *time;
	int *time_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return result_error;
	if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;
	if (data_input(data, data_parameters, &time, &time_size, data_index_time, data_num)==1) return result_error;

	// Determine the step that corresponds with 'beginning_time'.
	int beginning_step=0;
	while (1)
	{
		if (*(time+beginning_step)>=beginning_time) break;
		beginning_step++;
	}

	// Determine the step that corresponds with 'ending_time'.
	// If 'ending_time' is longer than the simulation lasted, then
	// 'ending_step' is set to the maximum number of steps.
	int ending_step=beginning_step;
	while (1)
	{
		if (*(time+ending_step)>=ending_time || ending_step==*time_size/sizeof(double)-1) break;
		ending_step++;
	}

	// Save a plot of the spins at 'beginning_step'.
	arg_short_int[0]=data_index_parameters;
	arg_short_int[1]=data_index_flips;
	arg_int[0]=beginning_step;
	cmd_plot3d();

	FILE *data=fopen(arg_string[2], "w");
	FILE *plot=fopen(arg_string[3], "w");

	animate_3d_real_time (parameters, flips, edge_alpha, time, beginning_step, ending_step-beginning_step, beginning_time, ending_time, time_interval, time_delay, save_frames, arg_string[2], data, plot);
	
	fclose (plot);
	fclose (data);

	return;
}

void cmd_an3dstep()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	double edge_alpha=arg_double[0];
	int beginning_step=arg_int[0];
	int ending_step=arg_int[1];

	struct run_parameters *parameters;
	int *parameters_size;

	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return result_error;
	if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");
	FILE *plot=fopen(arg_string[1], "w");

	animate_3d_step (parameters, flips, edge_alpha, beginning_step, ending_step, arg_string[0], data, plot);
	
	fclose (plot);
	fclose (data);

	return;
}

void cmd_diag()
{
	short int data_index=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	int steps=arg_int[0];

	struct run_parameters *parameters;
	int *parameters_size;
	
	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	if (steps>0)
	{
		if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;
	}
	else flips=0;

	FILE *data=fopen(arg_string[0], "w");

	plot_diag (parameters, flips, steps, data);

	fclose (data);

	return;
}

void cmd_diag4d()
{
	short int data_index=arg_short_int[0];

	struct run_parameters_4d *parameters;
	int *parameters_size;
	
	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_diag_4d (parameters, data);

	fclose (data);

	return;
}

void cmd_diag4d3()
{
	short int data_index=arg_short_int[0];

	struct run_parameters_4d *parameters;
	int *parameters_size;
	
	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_diag_4d3 (parameters, data);

	fclose (data);

	return;
}

void cmd_diagint()
{
	short int data_index=arg_short_int[0];

	struct integrate_parameters *int_parameters;
	int *int_parameters_size;
	
	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_diag_integrate (int_parameters, data);

	fclose (data);

	return;
}

void cmd_diagint4d()
{
	short int data_index=arg_short_int[0];

	struct integrate_parameters *int_parameters;
	int *int_parameters_size;
	
	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_diag_integrate_4d (int_parameters, data);

	fclose (data);

	return;
}

void cmd_plane()
{
	short int data_index=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	int steps=arg_int[0];
	int y_value=arg_int[1];

	struct run_parameters *parameters;
	int *parameters_size;
	
	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	if (steps>0)
	{
		if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;
	}
	else flips=0;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane (parameters, flips, steps, y_value, data);

	fclose (data);

	return;
}

void cmd_plane3da()
{
	short int data_index=arg_short_int[0];
	double r_value=arg_double[0];
	double s_value=arg_double[1];

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane_3d_a (parameters, data, r_value, s_value);

	fclose (data);

	return;
}

void cmd_plane3db()
{
	short int data_index=arg_short_int[0];
	double r_value=arg_double[0];
	double s_value=arg_double[1];

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane_3d_b (parameters, data, r_value, s_value);

	fclose (data);

	return;
}

void cmd_plane3dx()
{
	short int data_index=arg_short_int[0];
	double r_value=arg_double[0];
	double s_value=arg_double[1];

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane_3d_x (parameters, data, r_value, s_value);

	fclose (data);

	return;
}

void cmd_plane3dy()
{
	short int data_index=arg_short_int[0];
	double r_value=arg_double[0];
	double s_value=arg_double[1];

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane_3d_y (parameters, data, r_value, s_value);

	fclose (data);

	return;
}

void cmd_plane4d()
{
	short int data_index=arg_short_int[0];
	int y_value=arg_int[0];
	int q_value=arg_int[1];

	struct run_parameters_4d *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane_4d (parameters, y_value, q_value, data);

	fclose (data);

	return;
}

void cmd_planeint()
{
	short int data_index=arg_short_int[0];
	int y_value=arg_int[0];

	struct integrate_parameters *int_parameters;
	int *int_parameters_size;
	
	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane_integrate (int_parameters, y_value, data);

	fclose (data);

	return;
}

void cmd_planei4d()
{
	short int data_index=arg_short_int[0];
	int y_value=arg_int[0];
	int q_value=arg_int[1];

	struct integrate_parameters *int_parameters;
	int *int_parameters_size;
	
	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, data_index, data_num)==1) return result_error;

	FILE *data=fopen(arg_string[0], "w");

	plot_plane_integrate_4d (int_parameters, y_value, q_value, data);

	fclose (data);

	return;
}

void cmd_plot2d()
{
	short int data_index=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	int steps=arg_int[0];

	struct run_parameters *parameters;
	int *parameters_size;
	
	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	if (steps>0)
	{
		if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;
	}
	else flips=0;

	FILE *data=fopen(arg_string[0], "w");
	FILE *plot=fopen(arg_string[1], "w");

	plot_2d (parameters, flips, steps, arg_string[0], data, plot);

	fclose (plot);
	fclose (data);

	return;
}

void cmd_plot3d()
{
	short int data_index=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	double edge_alpha=arg_double[0];
	int steps=arg_int[0];

	struct run_parameters *parameters;
	int *parameters_size;

	int *flips;
	int *flips_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;
	
	if (steps>0)
	{
		if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return result_error;
	}
	else flips=0;

	FILE *data=fopen(arg_string[0], "w");
	FILE *plot=fopen(arg_string[1], "w");

	plot_3d (parameters, flips, edge_alpha, steps, arg_string[0], data, plot);

	fclose (plot);
	fclose (data);

	return;
}

