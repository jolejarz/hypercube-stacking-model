/**************************************************************
***************************************************************
*** 
***   cmd-save.c
***
***   Jason Olejarz
***   2011-03-09
***
***   This file contains commands for setting up postprocessing.
***
***************************************************************
**************************************************************/

void cmd_svdiaga()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_diagonal_area=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_diagonal_area_data diagareadata;

	diagareadata.data_index_parameters=data_index_parameters;
	diagareadata.data_index_diagonal_area=data_index_diagonal_area;
	diagareadata.size=num;
	save_diagonal_area(command_insert, &diagareadata, data, data_parameters, data_num);

	list_insert(save, save_index, save_diagonal_area, save_num);
	
	return;
}

void cmd_svseed()
{
	short int save_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	short int data_index_low=arg_short_int[2];
	short int data_index_high=arg_short_int[3];
	int num=arg_int[0];
	
	struct save_seed_data seeddata;

	seeddata.data_index_rng=data_index_rng;
	seeddata.data_index_low=data_index_low;
	seeddata.data_index_high=data_index_high;
	seeddata.size=num;
	save_seed(command_insert, &seeddata, data, data_parameters, data_num);

	list_insert(save, save_index, save_seed, save_num);
	
	return;
}

void cmd_svvol()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_volume=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_volume_data volumedata;

	volumedata.data_index_parameters=data_index_parameters;
	volumedata.data_index_volume=data_index_volume;
	volumedata.size=num;
	save_volume(command_insert, &volumedata, data, data_parameters, data_num);

	list_insert(save, save_index, save_volume, save_num);
	
	return;
}

void cmd_svvol4d()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_volume=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_volume_data volumedata;

	volumedata.data_index_parameters=data_index_parameters;
	volumedata.data_index_volume=data_index_volume;
	volumedata.size=num;
	save_volume_4d(command_insert, &volumedata, data, data_parameters, data_num);

	list_insert(save, save_index, save_volume_4d, save_num);
	
	return;
}

