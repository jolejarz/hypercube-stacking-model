/**************************************************************
***************************************************************
*** 
***   save.h
***
***   Jason Olejarz
***   2010-07-21
***
***   This is the header file for save.c.
***
***************************************************************
**************************************************************/

struct save_diagonal_area_data
{
	short int data_index_parameters;
	short int data_index_diagonal_area;
	int size;
};
struct save_diagonal_area_parameters
{
	int size;
	int index;
};

struct save_seed_data
{
	short int data_index_rng;
	short int data_index_low;
	short int data_index_high;
	int size;
};
struct save_seed_parameters
{
	int size;
	int index;
};

struct save_volume_data
{
	short int data_index_parameters;
	short int data_index_volume;
	int size;
};
struct save_volume_parameters
{
	int size;
	int index;
};

short int save_diagonal_area(short int command, struct save_diagonal_area_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_seed(short int command, struct save_seed_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_volume(short int command, struct save_volume_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_volume_4d(short int command, struct save_volume_data *command_data, void *data[], void *data_parameters[], short int data_size);

