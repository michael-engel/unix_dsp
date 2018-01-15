#include <stdio.h>
#include <stdint.h>
#include "audio.h"
int main(void)
{
	// Read input file (x)
	const int N = audio_length("tale_tone_48000.wav");

    int16_t x[] = {0,1,2,3,4};
    int16_t y[N];
	int fs = audio_read(x, "tale_tone_48000.wav", N);

	printf("N = %d\nfs = %d\n", N, fs);

	// Run input x through filter into output y
	for (int n = 0; n < N; ++n)
		y[n] = myNotchFilter(x[n]);

	// Save and play output y
	//audio_write(y, "tale_tone_out.wav", fs, N);
	//audio_play("tale_tone_out.wav");

	// Create x-asis for plotting
	int32_t x_axis [N];
	for (uint32_t n = 0; n < N; ++n)
		x_axis[n] = n;

	// DFT
	float Xlen[N];
	for (int m = 0; m < N; ++m)
	{
		Xlen[m] = dft(x, m, N);
	}

	// Create csv file and eps chart
	char* output_name = "Xlen";
	write_csv_float(x_axis, Xlen, output_name, N, 200);
	//csv_to_gnuplot_eps(output_name);
	return 0;
}
