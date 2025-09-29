/**************************************************************
***************************************************************
*** 
***   step.h
***
***   Jason Olejarz
***   2011-01-27
***
***   This is the header file for step.c.
***
***************************************************************
**************************************************************/

struct step_area_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_area;
};
struct step_area_parameters
{
	int size;
	int index;
};

struct step_average_velocity_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int all_times;
};
struct step_average_velocity_parameters
{
	int size;
	int index;
};

struct step_corner_data
{
	short int data_index_parameters;
};
struct step_corner_parameters
{
};

struct step_inner_corners_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_inner_corners;
};
struct step_inner_corners_parameters
{
	int size;
	int index;
};

struct step_outer_corners_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_outer_corners;
};
struct step_outer_corners_parameters
{
	int size;
	int index;
};

struct step_d2hdx2_middle_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_curvature;
};
struct step_d2hdx2_middle_parameters
{
	int size;
	int index;
};

struct step_diagonal_area_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_diagonal_area;
};
struct step_diagonal_area_parameters
{
	int size;
	int index;
};

struct step_flips_data
{
	short int data_index_parameters;
	short int data_index_flips;
};
struct step_flips_parameters
{
	int size;
	int index;
};

struct step_hmax_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_hmax;
};
struct step_hmax_parameters
{
	int size;
	int index;
};

struct step_middle_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_middle;
};
struct step_middle_parameters
{
	int size;
	int index;
};

struct step_middle_stop_data
{
	short int data_index_parameters;
	int middle;
	short int data_index_time;
};
struct step_middle_stop_parameters
{
	int size;
	int index;
};

struct step_plane_time_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double y_over_t_increment;
};
struct step_plane_time_parameters
{
};

struct step_point2d_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_height;
	double r_value;
};
struct step_point2d_parameters
{
	int size;
	int index;
};

struct step_point3d_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_height;
	double r_value;
	double s_value;
};
struct step_point3d_parameters
{
	int size;
	int index;
};

struct step_stop_corners_data
{
	short int data_index_parameters;
	int outer_minus_inner;
};
struct step_stop_corners_parameters
{
};

struct step_survival_data
{
	short int data_index_parameters;
	short int data_index_time;
	int max_num_times;
};
struct step_survival_parameters
{
	int size;
	int index;
};

struct step_time_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_time;
};
struct step_time_parameters
{
	int size;
	int index;
};

struct step_total_velocity_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_velocity;
};
struct step_total_velocity_parameters
{
	int size;
	int index;
};

struct step_trigger_steps_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	unsigned long int steps_interval;
	short int progress;
};
struct step_trigger_time_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double time_interval;
	short int progress;
};
struct step_trigger_time_once_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double time;
	int runs;
};
// The parameters data index for a trigger is the same whether the trigger is after a number of steps
// have elapsed or after an amount of time has elapsed.
struct step_trigger_parameters
{
	int total;
	int current;
	unsigned long int steps;
	double time;
};

struct step_velocity_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_velocity;
};
struct step_velocity_parameters
{
	int size;
	int index;
};

struct step_volume_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_volume;
};
struct step_volume_parameters
{
	int size;
	int index;
};

struct step_width_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_width;
};
struct step_width_parameters
{
	int size;
	int index;
};

struct step_width_new_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_width;
	int runs;
};
struct step_width_new_parameters
{
	int size;
	int index;
};

struct step_xme_and_yme_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_prob;
};
struct step_xme_and_yme_parameters
{
	int size;
	int index;
};

struct step_x_minus_greater_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_prob;
};
struct step_x_minus_greater_parameters
{
	int size;
	int index;
};

struct step_xmg_or_ymg_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_prob;
};
struct step_xmg_or_ymg_parameters
{
	int size;
	int index;
};

struct step_y_minus_greater_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_prob;
};
struct step_y_minus_greater_parameters
{
	int size;
	int index;
};

short int step_area(short int command, struct step_area_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_average_velocity(short int command, struct step_average_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_corner(short int command, struct step_corner_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_corner_4d(short int command, struct step_corner_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_corner_5d(short int command, struct step_corner_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_inner_corners(short int command, struct step_inner_corners_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_inner_corners_test(short int command, struct step_inner_corners_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_outer_corners(short int command, struct step_outer_corners_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_outer_corners_test(short int command, struct step_outer_corners_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_d2hdx2_middle(short int command, struct step_d2hdx2_middle_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_diagonal_area(short int command, struct step_diagonal_area_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_flips(short int command, struct step_flips_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_hmax(short int command, struct step_hmax_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_middle2d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_middle3d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_middle4d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_middle4d3(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_middle5d(short int command, struct step_middle_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_middle_stop(short int command, struct step_middle_stop_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_plane_time(short int command, struct step_plane_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_point2d(short int command, struct step_point2d_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_point3d(short int command, struct step_point3d_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_stop_corners(short int command, struct step_stop_corners_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_survival(short int command, struct step_survival_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_time(short int command, struct step_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_total_velocity(short int command, struct step_total_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_steps(short int command, struct step_trigger_steps_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_time(short int command, struct step_trigger_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_time_once(short int command, struct step_trigger_time_once_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_velocity(short int command, struct step_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_velocity_4d(short int command, struct step_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_velocity_5d(short int command, struct step_velocity_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_volume(short int command, struct step_volume_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_width(short int command, struct step_width_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_width_new(short int command, struct step_width_new_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_xme_and_yme(short int command, struct step_xme_and_yme_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_x_minus_greater(short int command, struct step_x_minus_greater_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_xmg_or_ymg(short int command, struct step_xmg_or_ymg_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_y_minus_greater(short int command, struct step_y_minus_greater_data *command_data, void *data[], void *data_parameters[], short int data_size);

