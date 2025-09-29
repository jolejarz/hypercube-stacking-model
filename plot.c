/**************************************************************
***************************************************************
*** 
***   plot.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains all graphics functions for ising3d.
***
***************************************************************
**************************************************************/

#include "includes.h"

void animate_2d(struct run_parameters *parameters, int *flips, int steps_interval, double time_delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int steps=parameters->steps;
	
	int area=xlen;
	
	int x, y, i;
	
	fprintf(matlab_data, "%d\n", hlen);
	fprintf(matlab_data, "%d\n", xlen);
	fprintf(matlab_data, "%d\n", steps);
	fprintf(matlab_data, "%d\n", (int)save_frames);

	for (i=0; i<steps; i++)
	{
		fprintf(matlab_data, "%d\n", (int)(*(flips+2*i)));
		fprintf(matlab_data, "%d\n", (int)(*(flips+2*i+1)));
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "save_frames=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "imagecount=0;\n");
	fprintf(matlab_plot, "if save_frames==1\n");
	fprintf(matlab_plot, "	print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	x=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if flip==2\n");
	fprintf(matlab_plot, "		delete(hx(x+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		delete(hy(x_array(x+1)+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		t=x_array(x+1);\n");
	fprintf(matlab_plot, "		if x<xlen-1\n");
	fprintf(matlab_plot, "			hy(x_array(x+1)+1)=line([x+1 x+1], [t t+1], 'Color', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			hy(x_array(x+1)+1)=line([0 0], [t t+1], 'Color', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if x_array(x+1)<hlen-1\n");
	fprintf(matlab_plot, "			x_array(x+1)=x_array(x+1)+1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			x_array(x+1)=0;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		hx(x+1)=line([x x+1], [x_array(x+1) x_array(x+1)], 'Color', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if flip==1\n");
	fprintf(matlab_plot, "		delete(hx(x+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if x_array(x+1)>0\n");
	fprintf(matlab_plot, "			delete(hy(x_array(x+1)));\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			delete(hy(hlen));\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if x_array(x+1)>0\n");
	fprintf(matlab_plot, "			t=x_array(x+1)-1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			t=hlen-1;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		hy(t+1)=line([x x], [t t+1], 'Color', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if x_array(x+1)>0\n");
	fprintf(matlab_plot, "			x_array(x+1)=x_array(x+1)-1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			x_array(x+1)=hlen-1;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		hx(x+1)=line([x x+1], [x_array(x+1) x_array(x+1)], 'Color', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if mod(i,%d)==0\n", steps_interval);
	fprintf(matlab_plot, "		pause(%lf)\n", time_delay);
	fprintf(matlab_plot, "		drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		imagecount=imagecount+1;\n");
	fprintf(matlab_plot, "		if save_frames==1\n");
	fprintf(matlab_plot, "			print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	return;
}

void animate_3d(struct run_parameters *parameters, int *flips, double edge_alpha, int steps_interval, double time_delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	int steps=parameters->steps;
	
	int area=xlen;
	
	int x, y, i;
	
	fprintf(matlab_data, "%d\n", hlen);
	fprintf(matlab_data, "%d\n", xlen);
	fprintf(matlab_data, "%d\n", ylen);
	fprintf(matlab_data, "%d\n", steps);
	fprintf(matlab_data, "%d\n", (int)save_frames);

	for (i=0; i<steps; i++)
	{
		fprintf(matlab_data, "%d\n", (int)(*(flips+2*i)));
		fprintf(matlab_data, "%d\n", (int)(*(flips+2*i+1)));
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ylen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "save_frames=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "imagecount=0;\n");
	fprintf(matlab_plot, "if save_frames==1\n");
	fprintf(matlab_plot, "	print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	j=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	x=mod(j,xlen);\n");
	fprintf(matlab_plot, "	y=floor(j/xlen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if flip==2\n");
	fprintf(matlab_plot, "		delete(hxy(x+1, y+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		delete(hyz(y+1, xy_array(x+1, y+1)+1));\n");
	fprintf(matlab_plot, "		delete(hzx(xy_array(x+1, y+1)+1, x+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		t=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "		if x<xlen-1\n");
	fprintf(matlab_plot, "			hyz(y+1, xy_array(x+1, y+1)+1)=patch([x+1 x+1 x+1 x+1], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			hyz(y+1, xy_array(x+1, y+1)+1)=patch([0 0 0 0], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		if y<ylen-1\n");
	fprintf(matlab_plot, "			hzx(xy_array(x+1, y+1)+1, x+1)=patch([x x x+1 x+1], [y+1 y+1 y+1 y+1], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			hzx(xy_array(x+1, y+1)+1, x+1)=patch([x x x+1 x+1], [0 0 0 0], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)<hlen-1\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=xy_array(x+1, y+1)+1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=0;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		z=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if flip==1\n");
	fprintf(matlab_plot, "		delete(hxy(x+1, y+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "			delete(hyz(y+1, xy_array(x+1, y+1)));\n");
	fprintf(matlab_plot, "			delete(hzx(xy_array(x+1, y+1), x+1));\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			delete(hyz(y+1, hlen));\n");
	fprintf(matlab_plot, "			delete(hzx(hlen, x+1));\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "			t=xy_array(x+1, y+1)-1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			t=hlen-1;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		hyz(y+1, t+1)=patch([x x x x], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		hzx(t+1, x+1)=patch([x x x+1 x+1], [y y y y], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=xy_array(x+1, y+1)-1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=hlen-1;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		z=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if mod(i,%d)==0\n", steps_interval);
	fprintf(matlab_plot, "		pause(%lf)\n", time_delay);
	fprintf(matlab_plot, "		drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		imagecount=imagecount+1;\n");
	fprintf(matlab_plot, "		if save_frames==1\n");
	fprintf(matlab_plot, "			print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	return;
}

void animate_3d_real_time(struct run_parameters *parameters, int *flips, double edge_alpha, double *time, int beginning_step, int steps, double beginning_time, double ending_time, double time_interval, double time_delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	
	int area=xlen;
	
	int x, y, xminus, xplus, yminus, yplus, i;

	int *plot_height=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_height, parameters->height_initial, sizeof(int)*xlen*ylen);
	int *plot_statex=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statex, parameters->statex_initial, sizeof(int)*xlen*ylen);
	int *plot_statey=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statey, parameters->statey_initial, sizeof(int)*xlen*ylen);
	
	int coordinate;
	short int move;
	
	// If a 'flips' array was specified, then evolve the initial state for the specified number of steps.
	if (flips>0)
	{
		for (i=0; i<steps; i++)
		{
			coordinate=(int)(*(flips+2*i));
			move=(int)(*(flips+2*i+1));

			x=coordinate%xlen;
			y=coordinate/xlen;

			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			if (move==2)
			{
				if (*(plot_height+coordinate)<hlen-1) (*(plot_height+coordinate))++;
				else *(plot_height+coordinate)=0;

				(*(plot_statex+coordinate))--;
				(*(plot_statey+coordinate))--;

				(*(plot_statex+xplus+y*xlen))++;
				(*(plot_statey+x+yplus*xlen))++;
			}
			else if (move==1)
			{
				if (*(plot_height+coordinate)>0) (*(plot_height+coordinate))--;
				else *(plot_height+coordinate)=hlen-1;

				(*(plot_statex+coordinate))++;
				(*(plot_statey+coordinate))++;

				(*(plot_statex+xplus+y*xlen))--;
				(*(plot_statey+x+yplus*xlen))--;
			}
		}
	}

	fprintf(matlab_data, "%d\n", hlen);
	fprintf(matlab_data, "%d\n", xlen);
	fprintf(matlab_data, "%d\n", ylen);
	fprintf(matlab_data, "%d\n", beginning_step);
	fprintf(matlab_data, "%d\n", steps);
	fprintf(matlab_data, "%d\n", (int)save_frames);

	for (i=beginning_step; i<beginning_step+steps; i++)
	{
		fprintf(matlab_data, "%d\n", (int)(*(flips+2*i)));
		fprintf(matlab_data, "%d\n", (int)(*(flips+2*i+1)));
		fprintf(matlab_data, "%f\n", (float)(*(time+i)));
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ylen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "beginning_step=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "save_frames=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "last_time=0;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "title('time = 0', 'FontSize', 20)\n");
	fprintf(matlab_plot, "drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "imagecount=0;\n");
	fprintf(matlab_plot, "if save_frames==1\n");
	fprintf(matlab_plot, "	print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	j=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	time=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	x=mod(j,xlen);\n");
	fprintf(matlab_plot, "	y=floor(j/xlen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if flip==2\n");
	fprintf(matlab_plot, "		delete(hxy(x+1, y+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		delete(hyz(y+1, xy_array(x+1, y+1)+1));\n");
	fprintf(matlab_plot, "		delete(hzx(xy_array(x+1, y+1)+1, x+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		t=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "		if x<xlen-1\n");
	fprintf(matlab_plot, "			hyz(y+1, xy_array(x+1, y+1)+1)=patch([x+1 x+1 x+1 x+1], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			hyz(y+1, xy_array(x+1, y+1)+1)=patch([0 0 0 0], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		if y<ylen-1\n");
	fprintf(matlab_plot, "			hzx(xy_array(x+1, y+1)+1, x+1)=patch([x x x+1 x+1], [y+1 y+1 y+1 y+1], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			hzx(xy_array(x+1, y+1)+1, x+1)=patch([x x x+1 x+1], [0 0 0 0], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)<hlen-1\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=xy_array(x+1, y+1)+1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=0;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		z=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if flip==1\n");
	fprintf(matlab_plot, "		delete(hxy(x+1, y+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "			delete(hyz(y+1, xy_array(x+1, y+1)));\n");
	fprintf(matlab_plot, "			delete(hzx(xy_array(x+1, y+1), x+1));\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			delete(hyz(y+1, hlen));\n");
	fprintf(matlab_plot, "			delete(hzx(hlen, x+1));\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "			t=xy_array(x+1, y+1)-1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			t=hlen-1;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		hyz(y+1, t+1)=patch([x x x x], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		hzx(t+1, x+1)=patch([x x x+1 x+1], [y y y y], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=xy_array(x+1, y+1)-1;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			xy_array(x+1, y+1)=hlen-1;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		z=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if time-last_time>=%lf || i==steps\n",time_interval);
	fprintf(matlab_plot, "		title(['time = ', num2str(time)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "		pause(%lf)\n", time_delay);
	fprintf(matlab_plot, "		drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		last_time=last_time+%lf;\n",time_interval);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		imagecount=imagecount+1;\n");
	fprintf(matlab_plot, "		if save_frames==1\n");
	fprintf(matlab_plot, "			print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	free (plot_statey);
	free (plot_statex);
	free (plot_height);

	return;
}

void animate_3d_step(struct run_parameters *parameters, int *flips, double edge_alpha, int beginning_step, int ending_step, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;

	int *plot_height=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_height, parameters->height_initial, sizeof(int)*xlen*ylen);
	int *plot_statex=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statex, parameters->statex_initial, sizeof(int)*xlen*ylen);
	int *plot_statey=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statey, parameters->statey_initial, sizeof(int)*xlen*ylen);
	
	int coordinate;
	short int move;
	
	int x, y, xminus, xplus, yminus, yplus, i;
	
	int steps=ending_step-beginning_step;

	// If a 'flips' array was specified, then evolve the initial state up to the beginning step of the animation.
	if (flips>0)
	{
		for (i=0; i<beginning_step; i++)
		{
			coordinate=(int)(*(flips+2*i));
			move=(int)(*(flips+2*i+1));

			x=coordinate%xlen;
			y=coordinate/xlen;

			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			if (move==2)
			{
				if (*(plot_height+coordinate)<hlen-1) (*(plot_height+coordinate))++;
				else *(plot_height+coordinate)=0;

				(*(plot_statex+coordinate))--;
				(*(plot_statey+coordinate))--;

				(*(plot_statex+xplus+y*xlen))++;
				(*(plot_statey+x+yplus*xlen))++;
			}
			else if (move==1)
			{
				if (*(plot_height+coordinate)>0) (*(plot_height+coordinate))--;
				else *(plot_height+coordinate)=hlen-1;

				(*(plot_statex+coordinate))++;
				(*(plot_statey+coordinate))++;

				(*(plot_statex+xplus+y*xlen))--;
				(*(plot_statey+x+yplus*xlen))--;
			}
		}
	}
	
	fprintf(matlab_data, "%d\n", hlen);
	fprintf(matlab_data, "%d\n", xlen);
	fprintf(matlab_data, "%d\n", ylen);
	fprintf(matlab_data, "%d\n", beginning_step);
	fprintf(matlab_data, "%d\n", steps);

	for (i=0; i<xlen*ylen; i++)
	{
		fprintf(matlab_data, "%d\n", *(plot_height+i));
		fprintf(matlab_data, "%d\n", *(plot_statex+i));
		fprintf(matlab_data, "%d\n", *(plot_statey+i));
	}
	
	for (i=beginning_step; i<ending_step; i++)
	{
		x=(*(flips+2*i))%xlen;
		y=(*(flips+2*i))/xlen;

		fprintf(matlab_data, "%d %d\n", x, y);
		fprintf(matlab_data, "%d\n", *(flips+2*i+1));
	}

	fprintf(matlab_plot, "function animate_3d_step\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "global hlen xlen ylen beginning_step steps old_step xy_array hxy hyz hzx statex statey x_array y_array flip_array\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ylen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "beginning_step=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 xlen 0 ylen 0 hlen])\n");
	fprintf(matlab_plot, "set(gca,'Units','centimeters')\n");
	fprintf(matlab_plot, "position=get(gcf,'Position');\n");
	fprintf(matlab_plot, "position(4)=500;\n");
	fprintf(matlab_plot, "set(gcf,'Position',position)\n");
	fprintf(matlab_plot, "title(['lattice=(', num2str(xlen), ', ', num2str(ylen), ', ', num2str(hlen), '), steps (min, current, max)=(', num2str(beginning_step), ', ', num2str(beginning_step), ', ', num2str(beginning_step+steps), ')'])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "xy_array=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hxy=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "hyz=zeros(ylen, hlen);\n");
	fprintf(matlab_plot, "hzx=zeros(hlen, xlen);\n");
	fprintf(matlab_plot, "statex=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "statey=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "slider=uicontrol('Style','slider','Callback',@slider_callback,'Min',beginning_step,'Max',beginning_step+steps,'SliderStep',[1/steps 1],'Value',beginning_step,'Position',[64 400 384 20]);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=0:ylen-1\n");
	fprintf(matlab_plot, "	for x=0:xlen-1\n");
	fprintf(matlab_plot, "		z=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		i=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		j=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		xy_array(x+1, y+1)=z;\n");
	fprintf(matlab_plot, "		statex(x+1, y+1)=i;\n");
	fprintf(matlab_plot, "		statey(x+1, y+1)=j;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		t=z;\n");
	fprintf(matlab_plot, "		for k=0:statex(x+1, y+1)-1\n");
	fprintf(matlab_plot, "			if t+k==hlen\n");
	fprintf(matlab_plot, "				t=t-hlen;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			hyz(y+1, t+k+1)=patch([x x x x], [y y+1 y+1 y], [t+k t+k t+k+1 t+k+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		t=z;\n");
	fprintf(matlab_plot, "		for k=0:statey(x+1, y+1)-1\n");
	fprintf(matlab_plot, "			if t+k==hlen\n");
	fprintf(matlab_plot, "				t=t-hlen;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			hzx(t+k+1, x+1)=patch([x x x+1 x+1], [y y y y], [t+k t+k+1 t+k+1 t+k], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "x_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "y_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "flip_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "old_step=beginning_step;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	x_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	y_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "function slider_callback(h, eventdata)\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "global hlen xlen ylen beginning_step steps old_step xy_array hxy hyz hzx statex statey x_array y_array flip_array\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "set(h,'Value',cast(get(h,'Value'),'int32'))\n");
	fprintf(matlab_plot, "title(['lattice=(', num2str(xlen), ', ', num2str(ylen), ', ', num2str(hlen), '), steps (min, current, max)=(', num2str(beginning_step), ', ', num2str(get(h,'Value')), ', ', num2str(beginning_step+steps), ')'])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "if get(h,'Value')~=old_step\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if get(h,'Value')<old_step\n");
	fprintf(matlab_plot, "		loop_start=old_step;\n");
	fprintf(matlab_plot, "		loop_end=get(h,'Value')+1;\n");
	fprintf(matlab_plot, "		step_direction=-1;\n");	
	fprintf(matlab_plot, "	elseif get(h,'Value')>old_step\n");
	fprintf(matlab_plot, "		loop_start=old_step+1;\n");
	fprintf(matlab_plot, "		loop_end=get(h,'Value');\n");	
	fprintf(matlab_plot, "		step_direction=1;\n");	
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	for i=loop_start:step_direction:loop_end\n");
	fprintf(matlab_plot, "		x=x_array(i-beginning_step);\n");
	fprintf(matlab_plot, "		y=y_array(i-beginning_step);\n");
	fprintf(matlab_plot, "		flip=flip_array(i-beginning_step);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if (flip==2 && step_direction==1) || (flip==1 && step_direction==-1)\n");
	fprintf(matlab_plot, "			delete(hxy(x+1, y+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			delete(hyz(y+1, xy_array(x+1, y+1)+1));\n");
	fprintf(matlab_plot, "			delete(hzx(xy_array(x+1, y+1)+1, x+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			t=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "			if x<xlen-1\n");
	fprintf(matlab_plot, "				hyz(y+1, xy_array(x+1, y+1)+1)=patch([x+1 x+1 x+1 x+1], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "			else\n");
	fprintf(matlab_plot, "				hyz(y+1, xy_array(x+1, y+1)+1)=patch([0 0 0 0], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if y<ylen-1\n");
	fprintf(matlab_plot, "				hzx(xy_array(x+1, y+1)+1, x+1)=patch([x x x+1 x+1], [y+1 y+1 y+1 y+1], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "			else\n");
	fprintf(matlab_plot, "				hzx(xy_array(x+1, y+1)+1, x+1)=patch([x x x+1 x+1], [0 0 0 0], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			if xy_array(x+1, y+1)<hlen-1\n");
	fprintf(matlab_plot, "				xy_array(x+1, y+1)=xy_array(x+1, y+1)+1;\n");
	fprintf(matlab_plot, "			else\n");
	fprintf(matlab_plot, "				xy_array(x+1, y+1)=0;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			z=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if (flip==1 && step_direction==1) || (flip==2 && step_direction==-1)\n");
	fprintf(matlab_plot, "			delete(hxy(x+1, y+1));\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "				delete(hyz(y+1, xy_array(x+1, y+1)));\n");
	fprintf(matlab_plot, "				delete(hzx(xy_array(x+1, y+1), x+1));\n");
	fprintf(matlab_plot, "			else\n");
	fprintf(matlab_plot, "				delete(hyz(y+1, hlen));\n");
	fprintf(matlab_plot, "				delete(hzx(hlen, x+1));\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "				t=xy_array(x+1, y+1)-1;\n");
	fprintf(matlab_plot, "			else\n");
	fprintf(matlab_plot, "				t=hlen-1;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			hyz(y+1, t+1)=patch([x x x x], [y y+1 y+1 y], [t t t+1 t+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "			hzx(t+1, x+1)=patch([x x x+1 x+1], [y y y y], [t t+1 t+1 t], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			if xy_array(x+1, y+1)>0\n");
	fprintf(matlab_plot, "				xy_array(x+1, y+1)=xy_array(x+1, y+1)-1;\n");
	fprintf(matlab_plot, "			else\n");
	fprintf(matlab_plot, "				xy_array(x+1, y+1)=hlen-1;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			z=xy_array(x+1, y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "			hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "old_step=get(h,'Value');\n");

	free (plot_statey);
	free (plot_statex);
	free (plot_height);

	return;
}

void plot_2d(struct run_parameters *parameters, int *flips, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	
	int *plot_height=(int*)malloc(sizeof(int)*xlen);
	memcpy (plot_height, parameters->height_initial, sizeof(int)*xlen);
	int *plot_statex=(int*)malloc(sizeof(int)*xlen);
	memcpy (plot_statex, parameters->statex_initial, sizeof(int)*xlen);
	
	int coordinate;
	short int move;
	
	int x, xminus, xplus, i;

	// If a 'flips' array was specified, then evolve the initial state for the specified number of steps.
	if (flips>0)
	{
		for (i=0; i<steps; i++)
		{
			coordinate=*(flips+2*i);
			move=*(flips+2*i+1);

			x=coordinate;

			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;

			if (move==2)
			{
				if (*(plot_height+coordinate)<hlen-1) (*(plot_height+coordinate))++;
				else *(plot_height+coordinate)=0;

				(*(plot_statex+coordinate))--;

				(*(plot_statex+xplus))++;
			}
			if (move==1)
			{
				if (*(plot_height+coordinate)>0) (*(plot_height+coordinate))--;
				else *(plot_height+coordinate)=hlen-1;

				(*(plot_statex+coordinate))++;

				(*(plot_statex+xplus))--;
			}
		}
	}
	
	fprintf(matlab_data, "%d\n", hlen);
	fprintf(matlab_data, "%d\n", xlen);

	for (i=0; i<xlen; i++)
	{
		fprintf(matlab_data, "%d\n", *(plot_height+i));
		fprintf(matlab_data, "%d\n", *(plot_statex+i));
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 xlen 0 hlen])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "x_array=zeros(xlen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hx=zeros(xlen);\n");
	fprintf(matlab_plot, "hy=zeros(hlen);\n");
	fprintf(matlab_plot, "statex=zeros(xlen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for x=0:xlen-1\n");
	fprintf(matlab_plot, "	y=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	i=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	x_array(x+1)=y;\n");
	fprintf(matlab_plot, "	statex(x+1)=i;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	hx(x+1)=line([x x+1], [y y], 'Color', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	t=y;\n");
	fprintf(matlab_plot, "	for k=0:statex(x+1)-1\n");
	fprintf(matlab_plot, "		if t+k==hlen\n");
	fprintf(matlab_plot, "			t=t-hlen;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		hy(t+k+1)=line([x x], [t+k t+k+1], 'Color', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	free (plot_statex);
	free (plot_height);
	
	return;
}

void plot_3d(struct run_parameters *parameters, int *flips, double edge_alpha, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;

	int *plot_height=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_height, parameters->height_initial, sizeof(int)*xlen*ylen);
	int *plot_statex=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statex, parameters->statex_initial, sizeof(int)*xlen*ylen);
	int *plot_statey=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statey, parameters->statey_initial, sizeof(int)*xlen*ylen);
	
	int coordinate;
	short int move;
	
	int x, y, xminus, xplus, yminus, yplus, i;

	// If a 'flips' array was specified, then evolve the initial state for the specified number of steps.
	if (flips>0)
	{
		for (i=0; i<steps; i++)
		{
			coordinate=(int)(*(flips+2*i));
			move=(int)(*(flips+2*i+1));

			x=coordinate%xlen;
			y=coordinate/xlen;

			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			if (move==2)
			{
				if (*(plot_height+coordinate)<hlen-1) (*(plot_height+coordinate))++;
				else *(plot_height+coordinate)=0;

				(*(plot_statex+coordinate))--;
				(*(plot_statey+coordinate))--;

				(*(plot_statex+xplus+y*xlen))++;
				(*(plot_statey+x+yplus*xlen))++;
			}
			else if (move==1)
			{
				if (*(plot_height+coordinate)>0) (*(plot_height+coordinate))--;
				else *(plot_height+coordinate)=hlen-1;

				(*(plot_statex+coordinate))++;
				(*(plot_statey+coordinate))++;

				(*(plot_statex+xplus+y*xlen))--;
				(*(plot_statey+x+yplus*xlen))--;
			}
		}
	}
	
	fprintf(matlab_data, "%d\n", hlen);
	fprintf(matlab_data, "%d\n", xlen);
	fprintf(matlab_data, "%d\n", ylen);

	for (i=0; i<xlen*ylen; i++)
	{
		fprintf(matlab_data, "%d\n", *(plot_height+i));
		fprintf(matlab_data, "%d\n", *(plot_statex+i));
		fprintf(matlab_data, "%d\n", *(plot_statey+i));
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xlen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ylen=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 xlen 0 ylen 0 hlen])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "xy_array=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hxy=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "hyz=zeros(ylen, hlen);\n");
	fprintf(matlab_plot, "hzx=zeros(hlen, xlen);\n");
	fprintf(matlab_plot, "statex=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "statey=zeros(xlen, ylen);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=0:ylen-1\n");
	fprintf(matlab_plot, "	for x=0:xlen-1\n");
	fprintf(matlab_plot, "		z=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		i=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		j=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		xy_array(x+1, y+1)=z;\n");
	fprintf(matlab_plot, "		statex(x+1, y+1)=i;\n");
	fprintf(matlab_plot, "		statey(x+1, y+1)=j;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		hxy(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'r', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		t=z;\n");
	fprintf(matlab_plot, "		for k=0:statex(x+1, y+1)-1\n");
	fprintf(matlab_plot, "			if t+k==hlen\n");
	fprintf(matlab_plot, "				t=t-hlen;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			hyz(y+1, t+k+1)=patch([x x x x], [y y+1 y+1 y], [t+k t+k t+k+1 t+k+1], 'g', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		t=z;\n");
	fprintf(matlab_plot, "		for k=0:statey(x+1, y+1)-1\n");
	fprintf(matlab_plot, "			if t+k==hlen\n");
	fprintf(matlab_plot, "				t=t-hlen;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			hzx(t+k+1, x+1)=patch([x x x+1 x+1], [y y y y], [t+k t+k+1 t+k+1 t+k], 'b', 'EdgeAlpha', %lf);\n", edge_alpha);
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	free (plot_statey);
	free (plot_statex);
	free (plot_height);

	return;
}

void plot_diag(struct run_parameters *parameters, int *flips, int steps, FILE *data)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;

	int *plot_height=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_height, parameters->height_initial, sizeof(int)*xlen*ylen);
	int *plot_statex=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statex, parameters->statex_initial, sizeof(int)*xlen*ylen);
	int *plot_statey=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statey, parameters->statey_initial, sizeof(int)*xlen*ylen);
	
	int coordinate;
	short int move;
	
	int x, y, xminus, xplus, yminus, yplus, i;

	// If a 'flips' array was specified, then evolve the initial state for the specified number of steps.
	if (flips>0)
	{
		for (i=0; i<steps; i++)
		{
			coordinate=(int)(*(flips+2*i));
			move=(int)(*(flips+2*i+1));

			x=coordinate%xlen;
			y=coordinate/xlen;

			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			if (move==2)
			{
				if (*(plot_height+coordinate)<hlen-1) (*(plot_height+coordinate))++;
				else *(plot_height+coordinate)=0;

				(*(plot_statex+coordinate))--;
				(*(plot_statey+coordinate))--;

				(*(plot_statex+xplus+y*xlen))++;
				(*(plot_statey+x+yplus*xlen))++;
			}
			else if (move==1)
			{
				if (*(plot_height+coordinate)>0) (*(plot_height+coordinate))--;
				else *(plot_height+coordinate)=hlen-1;

				(*(plot_statex+coordinate))++;
				(*(plot_statey+coordinate))++;

				(*(plot_statex+xplus+y*xlen))--;
				(*(plot_statey+x+yplus*xlen))--;
			}
		}
	}

	x=0;
	for (i=0; i<xlen*ylen; i+=xlen+1)
	{
		fprintf (data, "%lf %d\n", sqrt(2)*((double)x), *(plot_height+i));
		x++;
	}

	free (plot_statey);
	free (plot_statex);
	free (plot_height);

	return;
}

void plot_diag_4d(struct run_parameters_4d *parameters, FILE *data)
{
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	int qlen=parameters->qlen;

	int x, i;

	x=0;
	for (i=0; i<xlen*ylen*qlen; i+=xlen*ylen+qlen+1)
	{
		fprintf (data, "%lf %d\n", sqrt(3)*((double)x), *(parameters->height+i));
		x++;
	}

	return;
}

void plot_diag_4d3(struct run_parameters_4d *parameters, FILE *data)
{
	unsigned long int length=parameters->xlen;

	unsigned long int x, i, offset, size;

	size=((length-1)*((length-1)-1)*(2*(length-1)-1)/6-(length-1)*((length-1)-1)*(2*length+1)/2+(length-1)*length*(length+1))/2+1;

	for (x=0; x<length; x++)
	{
		offset=(x*(x-1)*(2*x-1)/6-x*(x-1)*(2*length+1)/2+x*length*(length+1))/2+x*(length-x)+(x-1)*x/2+x;

		if (offset>size-1) break;
		
		fprintf (data, "%lf %d\n", sqrt(3)*((double)x), *(parameters->height+offset));
	}

	return;
}

void plot_diag_integrate(struct integrate_parameters *int_parameters, FILE *data)
{
	int length=int_parameters->length;

	double *plot_height=int_parameters->height;
	
	int i, x;

	x=0;
	for (i=0; i<length*length; i+=length+1)
	{
		fprintf (data, "%lf %lf\n", sqrt(2)*((double)x), *(plot_height+i));
		x++;
	}

	return;
}

void plot_diag_integrate_4d(struct integrate_parameters *int_parameters, FILE *data)
{
	int length=int_parameters->length;

	double *plot_height=int_parameters->height;
	
	int i, x;

	x=0;
	for (i=0; i<length*length*length; i+=length*length+length+1)
	{
		fprintf (data, "%lf %lf\n", sqrt(3)*((double)x), *(plot_height+i));
		x++;
	}

	return;
}

void plot_plane(struct run_parameters *parameters, int *flips, int steps, int y_value, FILE *data)
{
	int hlen=parameters->hlen;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;

	int *plot_height=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_height, parameters->height_initial, sizeof(int)*xlen*ylen);
	int *plot_statex=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statex, parameters->statex_initial, sizeof(int)*xlen*ylen);
	int *plot_statey=(int*)malloc(sizeof(int)*xlen*ylen);
	memcpy (plot_statey, parameters->statey_initial, sizeof(int)*xlen*ylen);
	
	int coordinate;
	short int move;
	
	int x, y, xminus, xplus, yminus, yplus, i;

	// If a 'flips' array was specified, then evolve the initial state for the specified number of steps.
	if (flips>0)
	{
		for (i=0; i<steps; i++)
		{
			coordinate=(int)(*(flips+2*i));
			move=(int)(*(flips+2*i+1));

			x=coordinate%xlen;
			y=coordinate/xlen;

			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			if (move==2)
			{
				if (*(plot_height+coordinate)<hlen-1) (*(plot_height+coordinate))++;
				else *(plot_height+coordinate)=0;

				(*(plot_statex+coordinate))--;
				(*(plot_statey+coordinate))--;

				(*(plot_statex+xplus+y*xlen))++;
				(*(plot_statey+x+yplus*xlen))++;
			}
			else if (move==1)
			{
				if (*(plot_height+coordinate)>0) (*(plot_height+coordinate))--;
				else *(plot_height+coordinate)=hlen-1;

				(*(plot_statex+coordinate))++;
				(*(plot_statey+coordinate))++;

				(*(plot_statex+xplus+y*xlen))--;
				(*(plot_statey+x+yplus*xlen))--;
			}
		}
	}

	for (i=y_value*xlen; i<y_value*xlen+xlen; i++)
	{
		fprintf (data, "%d %d\n", i-y_value*xlen, *(plot_height+i));
	}

	free (plot_statey);
	free (plot_statex);
	free (plot_height);

	return;
}

void plot_plane_3d_a(struct run_parameters *parameters, FILE *data, double r_value, double s_value)
{
	int x, y, a, i, x_value, y_value;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	
	double y_double;

	// This loop finds the coordinates (x, y, z) at which (x, r*x, s*x) is a point on the interface.
	y_double=0;
	for (x=0; x<xlen; x++)
	{
		if (*(parameters->height+x+((int)(y_double))*xlen)<(int)(s_value*x)) break;
		y_double+=r_value;
	}
	
	x_value=x;
	y_value=(int)(y_double);
	
	// Move to the lowest value of a.
	x=x_value;
	y=y_value;
	a=0;
	while (x>0 && y>0)
	{
		x--; y--; a--;
	}
	
	// Plot the plane.
	while (x<xlen && y<ylen)
	{
		if (*(parameters->height+x+y*xlen)>0) fprintf (data, "%lf %d\n", (double)a, *(parameters->height+x+y*xlen));
		x++; y++; a++;
	}

	return;
}

void plot_plane_3d_b(struct run_parameters *parameters, FILE *data, double r_value, double s_value)
{
	int x, y, b, i, x_value, y_value;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	
	double y_double;

	// This loop finds the coordinates (x, y, z) at which (x, r*x, s*x) is a point on the interface.
	y_double=0;
	for (x=0; x<xlen; x++)
	{
		if (*(parameters->height+x+((int)(y_double))*xlen)<(int)(s_value*x)) break;
		y_double+=r_value;
	}
	
	x_value=x;
	y_value=(int)(y_double);
	
	// Move to the lowest value of b.
	x=x_value;
	y=y_value;
	b=0;
	while (x>0 && y<ylen-1)
	{
		x--; y++; b--;
	}
	
	// Plot the plane.
	while (x<xlen && y>=0)
	{
		if (*(parameters->height+x+y*xlen)>0) fprintf (data, "%lf %d\n", (double)b, *(parameters->height+x+y*xlen));
		x++; y--; b++;
	}

	return;
}

void plot_plane_3d_x(struct run_parameters *parameters, FILE *data, double r_value, double s_value)
{
	int x, y, i, x_value, y_value;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	
	double y_double;

	// This loop finds the coordinates (x, y, z) at which (x, r*x, s*x) is a point on the interface.
	y_double=0;
	for (x=0; x<xlen; x++)
	{
		if (*(parameters->height+x+((int)(y_double))*xlen)<(int)(s_value*x)) break;
		y_double+=r_value;
	}
	
	x_value=x;
	y_value=(int)(y_double);
	
	// Plot the plane.
	
	for (x=0; x<xlen; x++)
	{
		if (*(parameters->height+x+y_value*xlen)>0) fprintf (data, "%lf %d\n", (double)(x-x_value), *(parameters->height+x+y_value*xlen));
	}
	
	return;
}

void plot_plane_3d_y(struct run_parameters *parameters, FILE *data, double r_value, double s_value)
{
	int x, y, i, x_value, y_value;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	
	double y_double;

	// This loop finds the coordinates (x, y, z) at which (x, r*x, s*x) is a point on the interface.
	y_double=0;
	for (x=0; x<xlen; x++)
	{
		if (*(parameters->height+x+((int)(y_double))*xlen)<(int)(s_value*x)) break;
		y_double+=r_value;
	}
	
	x_value=x;
	y_value=(int)(y_double);
	
	// Plot the plane.
	
	for (y=0; y<ylen; y++)
	{
		if (*(parameters->height+x_value+y*xlen)>0) fprintf (data, "%lf %d\n", (double)(y-y_value), *(parameters->height+x_value+y*xlen));
	}
	
	return;
}

void plot_plane_4d(struct run_parameters_4d *parameters, int y_value, int q_value, FILE *data)
{
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	
	int i;

	for (i=y_value*xlen+q_value*xlen*ylen; i<y_value*xlen+q_value*xlen*ylen+xlen; i++)
	{
		fprintf (data, "%d %d\n", i-y_value*xlen-q_value*xlen*ylen, *(parameters->height_initial+i));
	}

	return;
}

void plot_plane_integrate(struct integrate_parameters *int_parameters, int y_value, FILE *data)
{
	int length=int_parameters->length;

	double *plot_height=int_parameters->height;
	
	int i;

	for (i=y_value*length; i<y_value*length+length; i++)
	{
		fprintf (data, "%d %lf\n", i-y_value*length, *(plot_height+i));
	}

	return;
}

void plot_plane_integrate_4d(struct integrate_parameters *int_parameters, int y_value, int q_value, FILE *data)
{
	int length=int_parameters->length;

	double *plot_height=int_parameters->height;
	
	int i;

	for (i=y_value*length+q_value*length*length; i<y_value*length+q_value*length*length+length; i++)
	{
		fprintf (data, "%d %lf\n", i-y_value*length-q_value*length*length, *(plot_height+i));
	}

	return;
}

void plot_plane_time(struct run_parameters *parameters, int y_value, FILE *data)
{
	int xlen=parameters->xlen;
	
	int i;

	for (i=y_value*xlen; i<y_value*xlen+xlen; i++)
	{
		fprintf (data, "%d %d\n", i-y_value*xlen, *(parameters->height+i));
	}

	return;
}

