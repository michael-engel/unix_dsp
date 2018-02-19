#ifndef AUDIO_H
#define AUDIO_H


// Reads sample-frequency from a WAV file using FFmpeg
int audio_get_fs(char* name);

// Reads samples from a WAV file using FFmpeg
int audio_read(int16_t* data, char* name, int N);

// Outputs the length of file with name
int audio_length(char* name);

// Creates a wav file by piping raw samples to ffmpeg
void audio_write(int16_t* data, char* name, double fs, int N);

// Creates a sine-wave
void sine(int16_t* data, int A, int f, double Fs, int N);

// Opens file with name
void audio_play(char* name);

// Writes csv-file
void write_csv(int32_t* x_data, int16_t* y_data, char* name, int N, int scale);

// Writes csv-file
void write_csv_float(int32_t* x_data, float* y_data, char* name, int N, int scale);

// Opens gnuplot from csv-file with name
void csv_to_gnuplot(char* name);

// Creates eps-file with name from csv-file with name
void csv_to_gnuplot_eps(char* name);

// Attenuates 785 Hz frequencies
short myNotchFilter(short xn);

// DFT algorithm
float dft(int16_t* x, int32_t m, int32_t N);

// Shifts in_array' of size' shift' number of times and outputs to out_array'
void shift_arr(int32_t* in_array, int32_t* out_array, int32_t size, int32_t shift);

// Convolution of signal x with signal y
void convolution(int32_t* x, int32_t* y, int32_t* out_array, int32_t size_x, int32_t size_y);

// BROKEN: X-correlation made with same algorithm as convolution
void x_corr_1(int32_t* x, int32_t* y, int32_t* out_array, int32_t size_x, int32_t size_y);

// X-correlation of signal x and y
void x_corr(int32_t* x, int32_t* y, int32_t* out_array, int32_t size);

#endif //AUDIO_H