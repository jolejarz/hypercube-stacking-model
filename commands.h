/**************************************************************
***************************************************************
*** 
***   commands.h
***
***   Jason Olejarz
***   2011-01-25
***
***   This file sets up the command arrays.
***
***************************************************************
**************************************************************/

enum {type_double, type_int, type_short_int, type_string, type_unsigned_long_int};

double arg_double[32];
int arg_int[32];
short int arg_short_int[32];
char arg_string[32][32];
unsigned long int arg_unsigned_long_int[32];

// data
void cmd_bins();
void cmd_coords2d();
void cmd_coords3d();
void cmd_dd();
void cmd_dents();
void cmd_dindex();
void cmd_disp();
void cmd_midint();
void cmd_outin();
void cmd_sd();
void cmd_volint();
void cmd_volint4d();
// file
void cmd_load();
void cmd_save();
void cmd_text();
void cmd_xy();
// list
void cmd_dl();
void cmd_sl();
// loop
void cmd_lp();
void cmd_lpseed();
void cmd_tsc();
// plot
void cmd_an2d();
void cmd_an3d();
void cmd_an3drt();
void cmd_an3dstep();
void cmd_diag();
void cmd_diag4d();
void cmd_diag4d3();
void cmd_diagint();
void cmd_diagint4d();
void cmd_plane();
void cmd_plane3da();
void cmd_plane3db();
void cmd_plane3dx();
void cmd_plane3dy();
void cmd_plane4d();
void cmd_planeint();
void cmd_planei4d();
void cmd_plot2d();
void cmd_plot3d();
// run
void cmd_run();
void cmd_runbias();
void cmd_run4d();
void cmd_run4d2();
void cmd_run4d3();
void cmd_run5d();
void cmd_runint1();
void cmd_runint4d1();
void cmd_runint2();
void cmd_runint4d2();
void cmd_runint3();
// save
void cmd_svdiaga();
void cmd_svseed();
void cmd_svvol();
void cmd_svvol4d();
// set
void cmd_cpcur();
void cmd_cpcur4d();
void cmd_rnglcg();
void cmd_rngmt();
void cmd_seed();
void cmd_set2d();
void cmd_set3d();
void cmd_set4d();
void cmd_set4d2();
void cmd_set4d3();
void cmd_set5d();
void cmd_setint();
void cmd_settsc();
// step
void cmd_area();
void cmd_avgvel();
void cmd_corner();
void cmd_corner4d();
void cmd_corner5d();
void cmd_crnin();
void cmd_crnin2();
void cmd_crnout();
void cmd_crnout2();
void cmd_d2hdx2mid();
void cmd_diagarea();
void cmd_flips();
void cmd_hmax();
void cmd_middle2d();
void cmd_middle3d();
void cmd_middle4d();
void cmd_middle4d3();
void cmd_middle5d();
void cmd_midstop();
void cmd_planetime();
void cmd_point2d();
void cmd_point3d();
void cmd_stopcrn();
void cmd_surv();
void cmd_time();
void cmd_totalvel();
void cmd_trigs();
void cmd_trigt();
void cmd_trigtonce();
void cmd_vel();
void cmd_vel4d();
void cmd_vel5d();
void cmd_volume();
void cmd_width();
void cmd_widthnew();
void cmd_xmeandyme();
void cmd_xmg();
void cmd_xmgorymg();
void cmd_ymg();

int main();

char *cmd_name[]=
{
	/* data */
	"bins",
	"coords2d",
	"coords3d",
	"dd",
	"dents",
	"dindex",
	"disp",
	"midint",
	"outin",
	"sd",
	"volint",
	"volint4d",
	/* file */
	"load",
	"save",
	"text",
	"xy",
	/* list */
	"dl",
	"sl",
	/* loop */
	"lp",
	"lpseed",
	"tsc",
	/* plot */
	"an2d",
	"an3d",
	"an3drt",
	"an3dstep",
	"diag",
	"diag4d",
	"diag4d3",
	"diagint",
	"diagint4d",
	"plane",
	"plane3da",
	"plane3db",
	"plane3dx",
	"plane3dy",
	"plane4d",
	"planeint",
	"planei4d",
	"plot2d",
	"plot3d",
	/* run */
	"run",
	"runbias",
	"run4d",
	"run4d2",
	"run4d3",
	"run5d",
	"runint1",
	"runint4d1",
	"runint2",
	"runint4d2",
	"runint3",
	/* save */
	"svdiaga",
	"svseed",
	"svvol",
	"svvol4d",
	/* set */
	"cpcur",
	"cpcur4d",
	"rnglcg",
	"rngmt",
	"seed",
	"set2d",
	"set3d",
	"set4d",
	"set4d2",
	"set4d3",
	"set5d",
	"setint",
	"settsc",
	/* step */
	"area",
	"avgvel",
	"corner",
	"corner4d",
	"corner5d",
	"crnin",
	"crnin2",
	"crnout",
	"crnout2",
	"d2hdx2mid",
	"diagarea",
	"flips",
	"hmax",
	"middle2d",
	"middle3d",
	"middle4d",
	"middle4d3",
	"middle5d",
	"midstop",
	"planetime",
	"point2d",
	"point3d",
	"stopcrn",
	"surv",
	"time",
	"totalvel",
	"trigs",
	"trigt",
	"trigtonce",
	"vel",
	"vel4d",
	"vel5d",
	"volume",
	"width",
	"widthnew",
	"xmeandyme",
	"xmg",
	"xmgorymg",
	"ymg",
	/* exit / quit */
	"exit",
	"quit"
};
void (*functions[])()=
{
	/* data */
	cmd_bins,
	cmd_coords2d,
	cmd_coords3d,
	cmd_dd,
	cmd_dents,
	cmd_dindex,
	cmd_disp,
	cmd_midint,
	cmd_outin,
	cmd_sd,
	cmd_volint,
	cmd_volint4d,
	/* file */
	cmd_load,
	cmd_save,
	cmd_text,
	cmd_xy,
	/* list */
	cmd_dl,
	cmd_sl,
	/* loop */
	cmd_lp,
	cmd_lpseed,
	cmd_tsc,
	/* plot */
	cmd_an2d,
	cmd_an3d,
	cmd_an3drt,
	cmd_an3dstep,
	cmd_diag,
	cmd_diag4d,
	cmd_diag4d3,
	cmd_diagint,
	cmd_diagint4d,
	cmd_plane,
	cmd_plane3da,
	cmd_plane3db,
	cmd_plane3dx,
	cmd_plane3dy,
	cmd_plane4d,
	cmd_planeint,
	cmd_planei4d,
	cmd_plot2d,
	cmd_plot3d,
	/* run */
	cmd_run,
	cmd_runbias,
	cmd_run4d,
	cmd_run4d2,
	cmd_run4d3,
	cmd_run5d,
	cmd_runint1,
	cmd_runint4d1,
	cmd_runint2,
	cmd_runint4d2,
	cmd_runint3,
	/* save */
	cmd_svdiaga,
	cmd_svseed,
	cmd_svvol,
	cmd_svvol4d,
	/* set */
	cmd_cpcur,
	cmd_cpcur4d,
	cmd_rnglcg,
	cmd_rngmt,
	cmd_seed,
	cmd_set2d,
	cmd_set3d,
	cmd_set4d,
	cmd_set4d2,
	cmd_set4d3,
	cmd_set5d,
	cmd_setint,
	cmd_settsc,
	/* step */
	cmd_area,
	cmd_avgvel,
	cmd_corner,
	cmd_corner4d,
	cmd_corner5d,
	cmd_crnin,
	cmd_crnin2,
	cmd_crnout,
	cmd_crnout2,
	cmd_d2hdx2mid,
	cmd_diagarea,
	cmd_flips,
	cmd_hmax,
	cmd_middle2d,
	cmd_middle3d,
	cmd_middle4d,
	cmd_middle4d3,
	cmd_middle5d,
	cmd_midstop,
	cmd_planetime,
	cmd_point2d,
	cmd_point3d,
	cmd_stopcrn,
	cmd_surv,
	cmd_time,
	cmd_totalvel,
	cmd_trigs,
	cmd_trigt,
	cmd_trigtonce,
	cmd_vel,
	cmd_vel4d,
	cmd_vel5d,
	cmd_volume,
	cmd_width,
	cmd_widthnew,
	cmd_xmeandyme,
	cmd_xmg,
	cmd_xmgorymg,
	cmd_ymg,
	/* exit / quit */
	0,
	0
};
int num_args[]=
{
	/* data */
		/* bins */	2,
		/* coords2d */	2,
		/* coords3d */	3,
		/* dd */	1,
		/* dents */	1,
		/* dindex */	0,
		/* disp */	3,
		/* midint */	1,
		/* outin */	1,
		/* sd */	0,
		/* volint */	1,
		/* volint4d */	1,
	/* file */
		/* load */	2,
		/* save */	2,
		/* text */	2,
		/* xy */	3,
	/* list */
		/* dl */	2,
		/* sl */	1,
	/* loop */
		/* lp */	3,
		/* lpseed */	5,
		/* tsc */	2,
	/* plot */
		/* an2d */	7,
		/* an3d */	8,
		/* an3drt */	13,
		/* an3dstep */	7,
		/* diag */	4,
		/* diag4d */	2,
		/* diag4d3 */	2,
		/* diagint */	2,
		/* diagint4d */	2,
		/* plane */	5,
		/* plane3da */	4,
		/* plane3db */	4,
		/* plane3dx */	4,
		/* plane3dy */	4,
		/* plane4d */	4,
		/* planeint */	3,
		/* planei4d */	4,
		/* plot2d */	5,
		/* plot3d */	6,
	/* run */
		/* run */	1,
		/* runbias */	1,
		/* run4d */	1,
		/* run4d2 */	1,
		/* run4d3 */	1,
		/* run5d */	1,
		/* runint1 */	1,
		/* runint4d1 */	1,
		/* runint2 */	1,
		/* runint4d2 */	1,
		/* runint3 */	1,
	/* save */
		/* svdiaga */	4,
		/* svseed */	5,
		/* svvol */	4,
		/* svvol4d */	4,
	/* set */
		/* cpcur */	1,
		/* cpcur4d */	1,
		/* rnglcg */	1,
		/* rngmt */	1,
		/* seed */	3,
		/* set2d */	9,
		/* set3d */	10,
		/* set4d */	11,
		/* set4d2 */	11,
		/* set4d3 */	11,
		/* set5d */	12,
		/* setint */	5,
		/* settsc */	1,
	/* step */
		/* area */	4,
		/* avgvel */	4,
		/* corner */	2,
		/* corner4d */	2,
		/* corner5d */	2,
		/* crnin */	4,
		/* crnin2 */	4,
		/* crnout */	4,
		/* crnout2 */	4,
		/* d2hdx2mid */	4,
		/* diagarea */	4,
		/* flips */	3,
		/* hmax */	4,
		/* middle2d */	4,
		/* middle3d */	4,
		/* middle4d */	4,
		/* middle4d3 */	4,
		/* middle5d */	4,
		/* midstop */	4,
		/* planetime */	4,
		/* point2d */	5,
		/* point3d */	6,
		/* stopcrn */	3,
		/* surv */	4,
		/* time */	4,
		/* totalvel */	4,
		/* trigs */	5,
		/* trigt */	5,
		/* trigtonce */	5,
		/* vel */	4,
		/* vel4d */	4,
		/* vel5d */	4,
		/* volume */	4,
		/* width */	4,
		/* widthnew */	5,
		/* xmeandyme */	4,
		/* xmg */	4,
		/* xmgorymg */	4,
		/* ymg */	4,
	/* exit / quit */
		/* exit */	0,
		/* quit */	0
};
int arg_types[][32]=
{
	/* data */
		/* bins */	{type_short_int, type_int},
		/* coords2d */	{type_short_int, type_double},
		/* coords3d */	{type_short_int, type_double, type_double},
		/* dd */	{type_short_int},
		/* dents */	{type_short_int},
		/* dindex */	{},
		/* disp */	{type_short_int, type_int, type_int},
		/* midint */	{type_short_int},
		/* outin */	{type_short_int},
		/* sd */	{},
		/* volint */	{type_short_int},
		/* volint4d */	{type_short_int},
	/* file */
		/* load */	{type_short_int, type_string},
		/* save */	{type_short_int, type_string},
		/* text */	{type_short_int, type_string},
		/* xy */	{type_short_int, type_short_int, type_string},
	/* list */
		/* dl */	{type_short_int, type_short_int},
		/* sl */	{type_short_int},
	/* loop */
		/* lp */	{type_short_int, type_int, type_short_int},
		/* lpseed */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* tsc */	{type_short_int, type_short_int},
	/* plot */
		/* an2d */	{type_short_int, type_short_int, type_int, type_double, type_short_int, type_string, type_string},
		/* an3d */	{type_short_int, type_short_int, type_double, type_int, type_double, type_short_int, type_string, type_string},
		/* an3drt */	{type_short_int, type_short_int, type_double, type_short_int, type_double, type_double, type_double, type_double, type_short_int, type_string, type_string, type_string, type_string},
		/* an3dstep */	{type_short_int, type_short_int, type_double, type_int, type_int, type_string, type_string},
		/* diag */	{type_short_int, type_short_int, type_int, type_string},
		/* diag4d */	{type_short_int, type_string},
		/* diag4d3 */	{type_short_int, type_string},
		/* diagint */	{type_short_int, type_string},
		/* diagint4d */	{type_short_int, type_string},
		/* plane */	{type_short_int, type_short_int, type_int, type_int, type_string},
		/* plane3da */	{type_short_int, type_string, type_double, type_double},
		/* plane3db */	{type_short_int, type_string, type_double, type_double},
		/* plane3dx */	{type_short_int, type_string, type_double, type_double},
		/* plane3dy */	{type_short_int, type_string, type_double, type_double},
		/* plane4d */	{type_short_int, type_int, type_int, type_string},
		/* planeint */	{type_short_int, type_int, type_string},
		/* planei4d */	{type_short_int, type_int, type_int, type_string},
		/* plot2d */	{type_short_int, type_short_int, type_int, type_string, type_string},
		/* plot3d */	{type_short_int, type_short_int, type_double, type_int, type_string, type_string},
	/* run */
		/* run */	{type_short_int},
		/* runbias */	{type_short_int},
		/* run4d */	{type_short_int},
		/* run4d2 */	{type_short_int},
		/* run4d3 */	{type_short_int},
		/* run5d */	{type_short_int},
		/* runint1 */	{type_short_int},
		/* runint4d1 */	{type_short_int},
		/* runint2 */	{type_short_int},
		/* runint4d2 */	{type_short_int},
		/* runint3 */	{type_short_int},
	/* save */
		/* svdiaga */	{type_short_int, type_short_int, type_short_int, type_int},
		/* svseed */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* svvol */	{type_short_int, type_short_int, type_short_int, type_int},
		/* svvol4d */	{type_short_int, type_short_int, type_short_int, type_int},
	/* set */
		/* cpcur */	{type_short_int},
		/* cpcur4d */	{type_short_int},
		/* rnglcg */	{type_short_int},
		/* rngmt */	{type_short_int},
		/* seed */	{type_short_int, type_double, type_double},
		/* set2d */	{type_short_int, type_short_int, type_int, type_int, type_int, type_unsigned_long_int, type_double, type_short_int, type_double},
		/* set3d */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_unsigned_long_int, type_double, type_short_int, type_double},
		/* set4d */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_unsigned_long_int, type_double, type_short_int, type_double},
		/* set4d2 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_unsigned_long_int, type_double, type_short_int, type_double},
		/* set4d3 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_unsigned_long_int, type_double, type_short_int, type_double},
		/* set5d */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int, type_unsigned_long_int, type_double, type_short_int, type_double},
		/* setint */	{type_short_int, type_int, type_int, type_double, type_double},
		/* settsc */	{type_short_int},
	/* step */
		/* area */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* avgvel */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* corner */	{type_short_int, type_short_int},
		/* corner4d */	{type_short_int, type_short_int},
		/* corner5d */	{type_short_int, type_short_int},
		/* crnin */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* crnin2 */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* crnout */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* crnout2 */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* d2hdx2mid */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* diagarea */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* flips */	{type_short_int, type_short_int, type_short_int},
		/* hmax */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* middle2d */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* middle3d */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* middle4d */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* middle4d3 */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* middle5d */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* midstop */	{type_short_int, type_short_int, type_int, type_short_int},
		/* planetime */	{type_short_int, type_short_int, type_short_int, type_double},
		/* point2d */	{type_short_int, type_short_int, type_short_int, type_short_int, type_double},
		/* point3d */	{type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double},
		/* stopcrn */	{type_short_int, type_short_int, type_int},
		/* surv */	{type_short_int, type_short_int, type_short_int, type_int},
		/* time */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* totalvel */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* trigs */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_short_int},
		/* trigt */	{type_short_int, type_short_int, type_short_int, type_double, type_short_int},
		/* trigtonce */	{type_short_int, type_short_int, type_short_int, type_double, type_int},
		/* vel */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* vel4d */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* vel5d */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* volume */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* width */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* widthnew */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* xmeandyme */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* xmg */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* xmgorymg */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* ymg */	{type_short_int, type_short_int, type_short_int, type_short_int},
	/* exit / quit */
		/* exit */	{},
		/* quit */	{}
};

