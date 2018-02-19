#include "audio.h"

int main(void)
{
	// Read input file (x)
	int N = audio_length("tale_tone_48000.wav");
    int16_t x[N];
    int16_t y[N];
	int fs = audio_read(x, "tale_tone_48000.wav", N);

	// Run input x through filter into output y
	for (int n = 0; n < N; ++n)
		y[n] = myNotchFilter(x[n]);

	// Save and play output y
	audio_write(y, "tale_tone_out.wav", fs, N);
	audio_play("tale_tone_out.wav");

	// Create x-asis for plotting
	int32_t x_axis [N];
	for (uint32_t n = 0; n < N; ++n)
		x_axis[n] = n;

	// Create csv file and eps chart
	char* output_name = "Output";
	write_csv(x_axis,x, output_name, N, 20);
	csv_to_gnuplot_eps(output_name);
	return 0;
}
