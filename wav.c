#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct wav {
	char riff[4];
	int32_t flength;
	char wave[4];
	char fmt[4];
	int32_t chunk_size;
	int16_t format_tag;
	int16_t chan_num;
	int32_t sample_rate;
	int32_t bytes_per_second;
	int16_t bytes_per_sample;
	int16_t bits_per_sample;
	char data[4];
	int dlength;
} wav_header;

void genWav() {
	const int sample_rate = 8000;

	wav_header wavh;
	strncpy(wavh.riff, "RIFF", 4);
	strncpy(wavh.wave, "WAVE", 4);
	strncpy(wavh.fmt, "fmt ", 4);  // 'fmt ' should include the space
	strncpy(wavh.data, "data", 4);

	wavh.chunk_size = 16;
	wavh.format_tag = 1;
	wavh.chan_num = 1;
	wavh.sample_rate = sample_rate;
	wavh.bits_per_sample = 16;
	wavh.bytes_per_sample = (wavh.bits_per_sample / 8) * wavh.chan_num;
	wavh.bytes_per_second = sample_rate * wavh.bytes_per_sample;

	const int duration = 10;
	const int buffer_size = sample_rate * duration;

	wavh.dlength = buffer_size * wavh.bytes_per_sample;
	wavh.flength = 36 + wavh.dlength;  // Correct file length

	short int buffer[buffer_size];
	memset(buffer, 0, buffer_size * sizeof(short int));
	for (int i = 0; i < buffer_size; i++) {
		buffer[i] = (short int)((cos(2 * M_PI * 256 * i / sample_rate)) * 2000);  // Normalized cosine
	}

	FILE *fp = fopen("test.wav", "wb");
	fwrite(&wavh, 1, sizeof(wavh), fp);
	fwrite(buffer, sizeof(short int), buffer_size, fp);
	fclose(fp);
	printf("WAV file generated successfully.\n");
	return 0;
}
