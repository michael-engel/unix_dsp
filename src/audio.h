#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static char cmd[200];

// Reads sample-frequency from a WAV file using FFmpeg
int audio_get_fs(char* name)
{
	// Open WAV file with FFprobe and read sample-frequency via the pipe.
	char fs[10] = {0};
	FILE *pipein;
    sprintf(cmd, "ffprobe -v error -show_entries stream=sample_rate -of default=noprint_wrappers=1:nokey=1 ");
    strcat(cmd, name);
    pipein = popen(cmd, "r");
    fread(fs, 2, 10, pipein);
    pclose(pipein);
    return atoi(fs);
}

// Reads samples from a WAV file using FFmpeg
int audio_read(int16_t* data, char* name, int N)
{     
    // Open WAV file with FFmpeg and read raw samples via the pipe.
    FILE *pipein;
    sprintf(cmd, "ffmpeg -i ");
    strcat(cmd, name);
    strcat(cmd, " -f s16le -ac 1 -");
    pipein = popen(cmd, "r");
    fread(data, 2, N, pipein);
    pclose(pipein);
    return audio_get_fs(name);
}

int audio_length(char* name)
{
	char fs[10] = {0};
	FILE *pipein;
    sprintf(cmd, "ffprobe -v error -show_entries stream=duration_ts -of default=noprint_wrappers=1:nokey=1 ");
    strcat(cmd, name);
    pipein = popen(cmd, "r");
    fread(fs, 2, 10, pipein);
    pclose(pipein);
    return atoi(fs);
}

// Creates a wav file by piping raw samples to ffmpeg
void audio_write(int16_t* data, char* name, double fs, int N)
{
    // Pipe the audio data to ffmpeg, which writes it to a wav file
    FILE *pipeout;
	sprintf(cmd, "ffmpeg -y -f s16le -ar %f -ac 1 -i - ",fs);
	strcat(cmd, name);
    pipeout = popen(cmd, "w");
    fwrite(data, 2, N, pipeout);
    pclose(pipeout);
}

// Creates a sine-wave
void sine(int16_t* data, int A, int f, double Fs, int N)
{
	for (int n = 0; n < N ; ++n)
		data[n] = A * sin(n * f * 2.0 * M_PI / Fs);
}

// Opens file with name
void audio_play(char* name)
{
	sprintf(cmd, "open ");
	strcat(cmd, name);
	system(cmd);
}

// Writes csv-file
void write_csv(int32_t* x_data, int16_t* y_data, char* name, int N, int scale)
{
	// Print the sample values in the buffer to a CSV file
    if(scale < 1)
    {
        printf("Scale has to be bigger than 1.\n");
        return;
    }
    FILE *csvfile;
    strcpy(cmd, name);
    strcat(cmd, ".csv");
    csvfile = fopen(cmd, "w");
    for (int32_t n = 0 ; n < N ; n += scale)
    	fprintf(csvfile, "%d;%d\n", x_data[n], y_data[n]);
    fclose(csvfile);
}

// Opens gnuplot from csv-file with name
void csv_to_gnuplot(char* name)
{
    char plot_cmd[50];
    strcpy(plot_cmd, "plot '");
    strcat(plot_cmd, name);
    strcat(plot_cmd, ".csv' with linespoints ls 1");

    char title_cmd[50];
    strcpy(title_cmd, "set title \"");
    strcat(title_cmd, name);
    strcat(title_cmd, "\"");
    
    char * gnuplot_commands[] = 
    {
        "set datafile separator \";\"", 
        title_cmd, 
        "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0", 
        plot_cmd
    };

    char num_commands = (sizeof(gnuplot_commands)/sizeof(*gnuplot_commands));

    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");

    for (int i=0; i < num_commands; i++)
    {
        fprintf(gnuplotPipe, "%s \n", gnuplot_commands[i]); //Send commands to gnuplot one by one.
    }
}

// Creates eps-file with name from csv-file with name
void csv_to_gnuplot_eps(char* name)
{
    char plot_cmd[50];
    strcpy(plot_cmd, "plot '");
    strcat(plot_cmd, name);
    strcat(plot_cmd, ".csv' with linespoints ls 1");

    char output_cmd[50];
    strcpy(output_cmd, "set output \"");
    strcat(output_cmd, name);
    strcat(output_cmd, ".eps\"");

    char title_cmd[50];
    strcpy(title_cmd, "set title \"");
    strcat(title_cmd, name);
    strcat(title_cmd, "\"");
    
    char * gnuplot_commands[] = 
    {
        "set datafile separator \";\"", 
        "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0",
        title_cmd, 
        "set terminal postscript eps enhanced color font 'Helvetica,10'",
        output_cmd,
        plot_cmd
    };

    char num_commands = (sizeof(gnuplot_commands)/sizeof(*gnuplot_commands));

    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");

    for (int i=0; i < num_commands; i++)
    {
        fprintf(gnuplotPipe, "%s \n", gnuplot_commands[i]); //Send commands to gnuplot one by one.
    }
}

// Attenuates 785 Hz frequencies
short myNotchFilter(short xn)
{
	#define Ncnt 3
	#define Mcnt Ncnt
	#define F 10000.0
	static const short a[Ncnt] = {-1.0*F, 1.89*F, -0.9025*F};
	static const short b[Mcnt] = {1.0*F, -1.9894*F, 1.0*F};
	static short x[Mcnt] = {0};
	static int y[Ncnt] = {0};
	static short n = 0;
	int ff = 0;
	int fb = 0;
    x[n] = xn;

	for (short k = 0; k <= Mcnt-1; ++k)
		ff += b[k] * x[(Mcnt+n-k) % Mcnt];

	for (short l = 1; l <= Ncnt-1; ++l)
		fb += a[l] * y[(Ncnt+n-l) % Ncnt];

	int yn = (ff + fb) / F;
	y[n] = yn;
	n = (n+1) % Ncnt;
	return yn;
}
