/**************************************************************
***************************************************************
*** 
***   cmd-loop.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains commands for setting up preprocessing.
***
***************************************************************
**************************************************************/

void cmd_lp()
{
	short int loop_index=arg_short_int[0];
	int count=arg_int[0];
	short int progress=arg_short_int[1];
	
	struct loop_iterate_data command_data;
	
	command_data.count=count;
	command_data.progress=progress;
	loop_iterate(command_insert, &command_data, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_iterate, loop_num);
	
	return;
}

void cmd_lpseed()
{
	short int loop_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	short int data_index_low=arg_short_int[2];
	short int data_index_high=arg_short_int[3];
	int num=arg_int[0];
	
	struct loop_seed_data command_data;
	
	command_data.rng_struct=data[data_index_rng];
	command_data.data_index_low=data_index_low;
	command_data.data_index_high=data_index_high;
	command_data.num=num;
	loop_seed(command_insert, &command_data, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_seed, loop_num);
	
	return;
}

void cmd_tsc()
{
	short int loop_index=arg_short_int[0];
	short int data_index=arg_short_int[1];

	struct loop_seed_tsc_data command_data;

	command_data.rng_struct=data[data_index];
	loop_seed_tsc(command_insert, &command_data, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_seed_tsc, loop_num);

	return;
}

