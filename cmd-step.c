/**************************************************************
***************************************************************
*** 
***   cmd-step.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains commands for intermediate processing.
***
***************************************************************
**************************************************************/

void cmd_area()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_area=arg_short_int[3];
	
	struct step_area_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_area=data_index_area;

	step_area(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_area, step_num);
	
	return;
}

void cmd_avgvel()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int all_times=arg_short_int[3];
	
	struct step_average_velocity_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.all_times=all_times;

	step_average_velocity(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_average_velocity, step_num);
	
	return;
}

void cmd_corner()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	
	struct step_corner_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;

	step_corner(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_corner, step_num);
	
	return;
}

void cmd_corner4d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	
	struct step_corner_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;

	step_corner_4d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_corner_4d, step_num);
	
	return;
}

void cmd_corner5d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	
	struct step_corner_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;

	step_corner_5d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_corner_5d, step_num);
	
	return;
}

void cmd_crnin()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_inner_corners=arg_short_int[3];
	
	struct step_inner_corners_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_inner_corners=data_index_inner_corners;

	step_inner_corners(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_inner_corners, step_num);
	
	return;
}

void cmd_crnin2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_inner_corners=arg_short_int[3];
	
	struct step_inner_corners_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_inner_corners=data_index_inner_corners;

	step_inner_corners_test(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_inner_corners_test, step_num);
	
	return;
}

void cmd_crnout()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_outer_corners=arg_short_int[3];
	
	struct step_outer_corners_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_outer_corners=data_index_outer_corners;

	step_outer_corners(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_outer_corners, step_num);
	
	return;
}

void cmd_crnout2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_outer_corners=arg_short_int[3];
	
	struct step_outer_corners_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_outer_corners=data_index_outer_corners;

	step_outer_corners_test(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_outer_corners_test, step_num);
	
	return;
}

void cmd_d2hdx2mid()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_curvature=arg_short_int[3];
	
	struct step_d2hdx2_middle_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_curvature=data_index_curvature;

	step_d2hdx2_middle(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_d2hdx2_middle, step_num);
	
	return;
}

void cmd_diagarea()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_diagonal_area=arg_short_int[3];
	
	struct step_diagonal_area_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_diagonal_area=data_index_diagonal_area;

	step_diagonal_area(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_diagonal_area, step_num);
	
	return;
}

void cmd_flips()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_flips=arg_short_int[2];
	
	struct step_flips_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_flips=data_index_flips;

	step_flips(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_flips, step_num);
	
	return;
}

void cmd_hmax()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_hmax=arg_short_int[3];
	
	struct step_hmax_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_hmax=data_index_hmax;

	step_hmax(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_hmax, step_num);
	
	return;
}

void cmd_middle2d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_middle=arg_short_int[3];
	
	struct step_middle_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_middle=data_index_middle;

	step_middle2d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_middle2d, step_num);
	
	return;
}

void cmd_middle3d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_middle=arg_short_int[3];
	
	struct step_middle_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_middle=data_index_middle;

	step_middle3d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_middle3d, step_num);
	
	return;
}

void cmd_middle4d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_middle=arg_short_int[3];
	
	struct step_middle_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_middle=data_index_middle;

	step_middle4d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_middle4d, step_num);
	
	return;
}

void cmd_middle4d3()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_middle=arg_short_int[3];
	
	struct step_middle_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_middle=data_index_middle;

	step_middle4d3(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_middle4d3, step_num);
	
	return;
}

void cmd_middle5d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_middle=arg_short_int[3];
	
	struct step_middle_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_middle=data_index_middle;

	step_middle5d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_middle5d, step_num);
	
	return;
}

void cmd_midstop()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int middle=arg_int[0];
	short int data_index_time=arg_short_int[2];
	
	struct step_middle_stop_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.middle=middle;
	command_data.data_index_time=data_index_time;

	step_middle_stop(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_middle_stop, step_num);
	
	return;
}

void cmd_planetime()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double y_over_t_increment=arg_double[0];
	
	struct step_plane_time_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.y_over_t_increment=y_over_t_increment;

	step_plane_time(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_plane_time, step_num);
	
	return;
}

void cmd_point2d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_height=arg_short_int[3];
	double r_value=arg_double[0];
	
	struct step_point2d_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_height=data_index_height;
	command_data.r_value=r_value;

	step_point2d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_point2d, step_num);
	
	return;
}

void cmd_point3d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_height=arg_short_int[3];
	double r_value=arg_double[0];
	double s_value=arg_double[1];
	
	struct step_point3d_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_height=data_index_height;
	command_data.r_value=r_value;
	command_data.s_value=s_value;

	step_point3d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_point3d, step_num);
	
	return;
}

void cmd_stopcrn()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int outer_minus_inner=arg_int[0];
	
	struct step_stop_corners_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.outer_minus_inner=outer_minus_inner;
	
	step_stop_corners(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_stop_corners, step_num);
	
	return;
}

void cmd_surv()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_time=arg_short_int[2];
	int max_num_times=arg_int[0];
	
	struct step_survival_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_time=data_index_time;
	command_data.max_num_times=max_num_times;
	
	step_survival(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_survival, step_num);
	
	return;
}

void cmd_time()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_time=arg_short_int[3];
	
	struct step_time_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_time=data_index_time;
	
	step_time(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_time, step_num);
	
	return;
}

void cmd_totalvel()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_velocity=arg_short_int[3];
	
	struct step_total_velocity_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_velocity=data_index_velocity;

	step_total_velocity(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_total_velocity, step_num);
	
	return;
}

void cmd_trigs()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	unsigned long int steps_interval=arg_unsigned_long_int[0];
	short int progress=arg_short_int[3];
	
	struct step_trigger_steps_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.steps_interval=steps_interval;
	command_data.progress=progress;

	step_trigger_steps(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_steps, step_num);
	
	return;
}

void cmd_trigt()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double time_interval=arg_double[0];
	short int progress=arg_short_int[3];
	
	struct step_trigger_time_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.time_interval=time_interval;
	command_data.progress=progress;

	step_trigger_time(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_time, step_num);
	
	return;
}

void cmd_trigtonce()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double time=arg_double[0];
	int runs=arg_int[0];
	
	struct step_trigger_time_once_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.time=time;
	command_data.runs=runs;

	step_trigger_time_once(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_time_once, step_num);
	
	return;
}

void cmd_vel()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_velocity=arg_short_int[3];
	
	struct step_velocity_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_velocity=data_index_velocity;
	command_data.data_index_trigger=data_index_trigger;

	step_velocity(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_velocity, step_num);
	
	return;
}

void cmd_vel4d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_velocity=arg_short_int[3];
	
	struct step_velocity_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_velocity=data_index_velocity;
	command_data.data_index_trigger=data_index_trigger;

	step_velocity_4d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_velocity_4d, step_num);
	
	return;
}

void cmd_vel5d()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_velocity=arg_short_int[3];
	
	struct step_velocity_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_velocity=data_index_velocity;
	command_data.data_index_trigger=data_index_trigger;

	step_velocity_5d(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_velocity_5d, step_num);
	
	return;
}

void cmd_volume()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_volume=arg_short_int[3];
	
	struct step_volume_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_volume=data_index_volume;

	step_volume(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_volume, step_num);
	
	return;
}

void cmd_width()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_width=arg_short_int[3];
	
	struct step_width_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_width=data_index_width;

	step_width(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_width, step_num);
	
	return;
}

void cmd_widthnew()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_width=arg_short_int[3];
	int runs=arg_int[0];
	
	struct step_width_new_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_width=data_index_width;
	command_data.runs=runs;

	step_width_new(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_width_new, step_num);
	
	return;
}

void cmd_xmeandyme()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_prob=arg_short_int[3];
	
	struct step_xme_and_yme_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_prob=data_index_prob;

	step_xme_and_yme(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_xme_and_yme, step_num);
	
	return;
}

void cmd_xmg()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_prob=arg_short_int[3];
	
	struct step_x_minus_greater_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_prob=data_index_prob;

	step_x_minus_greater(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_x_minus_greater, step_num);
	
	return;
}

void cmd_xmgorymg()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_prob=arg_short_int[3];
	
	struct step_xmg_or_ymg_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_prob=data_index_prob;

	step_xmg_or_ymg(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_xmg_or_ymg, step_num);
	
	return;
}

void cmd_ymg()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_prob=arg_short_int[3];
	
	struct step_y_minus_greater_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_prob=data_index_prob;

	step_y_minus_greater(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_y_minus_greater, step_num);
	
	return;
}


