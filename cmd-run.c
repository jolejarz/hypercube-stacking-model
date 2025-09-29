/**************************************************************
***************************************************************
*** 
***   cmd-run.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the command for running the simulations.
***
***************************************************************
**************************************************************/

void cmd_run()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run, parameters);

	return;
}

void cmd_runbias()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, runbias, parameters);

	return;
}

void cmd_run4d()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run4d, parameters);

	return;
}

void cmd_run4d2()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run4d2, parameters);

	return;
}

void cmd_run4d3()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	
	struct run_parameters_4d3 *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run4d3, parameters);

	return;
}

void cmd_run5d()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run5d, parameters);

	return;
}

void cmd_runint1()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int int_parameters_data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;

	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, int_parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run_integrate_1, int_parameters);

	return;
}

void cmd_runint4d1()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int int_parameters_data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;

	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, int_parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run_integrate_4d_1, int_parameters);

	return;
}

void cmd_runint2()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int int_parameters_data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;

	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, int_parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run_integrate_2, int_parameters);

	return;
}

void cmd_runint4d2()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int int_parameters_data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;

	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, int_parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run_integrate_4d_2, int_parameters);

	return;
}

void cmd_runint3()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int int_parameters_data_index=arg_short_int[0];
	
	struct integrate_parameters *int_parameters;
	int *int_parameters_size;

	if (data_input(data, data_parameters, &int_parameters, &int_parameters_size, int_parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run_integrate_3, int_parameters);

	return;
}

