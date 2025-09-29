/**************************************************************
***************************************************************
*** 
***   plot.h
***
***   Jason Olejarz
***   2011-01-27
***
***   This is the header file for plot.c.
***
***************************************************************
**************************************************************/

void animate_2d(struct run_parameters *parameters, int *flips, int steps_interval, double time_delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void animate_3d(struct run_parameters *parameters, int *flips, double edge_alpha, int steps_interval, double time_delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void animate_3d_real_time(struct run_parameters *parameters, int *flips, double edge_alpha, double *time, int beginning_step, int steps, double beginning_time, double ending_time, double time_interval, double time_delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void animate_3d_step(struct run_parameters *parameters, int *flips, double edge_alpha, int beginning_step, int ending_step, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void plot_2d(struct run_parameters *parameters, int *flips, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void plot_3d(struct run_parameters *parameters, int *flips, double edge_alpha, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void plot_diag(struct run_parameters *parameters, int *flips, int steps, FILE *data);
void plot_diag_4d(struct run_parameters_4d *parameters, FILE *data);
void plot_diag_4d3(struct run_parameters_4d *parameters, FILE *data);
void plot_diag_integrate(struct integrate_parameters *int_parameters, FILE *data);
void plot_diag_integrate_4d(struct integrate_parameters *int_parameters, FILE *data);
void plot_plane(struct run_parameters *parameters, int *flips, int steps, int y_value, FILE *data);
void plot_plane_3d_a(struct run_parameters *parameters, FILE *data, double r_value, double s_value);
void plot_plane_3d_b(struct run_parameters *parameters, FILE *data, double r_value, double s_value);
void plot_plane_3d_x(struct run_parameters *parameters, FILE *data, double r_value, double s_value);
void plot_plane_3d_y(struct run_parameters *parameters, FILE *data, double r_value, double s_value);
void plot_plane_4d(struct run_parameters_4d *parameters, int y_value, int q_value, FILE *data);
void plot_plane_integrate(struct integrate_parameters *int_parameters, int y_value, FILE *data);
void plot_plane_integrate_4d(struct integrate_parameters *int_parameters, int y_value, int q_value, FILE *data);

