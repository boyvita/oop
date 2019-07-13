#pragma once
#include <vector>
#include <stdint.h>
#include "errors.h"

// TODO: Implement all this in the form of a class.
// TODO: Use an exception system to control errors.
// TODO: Make support for 8-bit PSM, not just 16-bit.
// TODO: Write a function to change the tone of the voice. (Interestingly, it's not too difficult?)
class wav_header {
public:
	// Fills header with zeroes.
	wav_header() = default;

	// Reads file 'filename' and puts header's data to 'header_ptr' address.
	wav_header(const char* filename);

	// Prints header's data from 'header_ptr' address.
	void print_info() const;

	// Reads file 'filename' and puts PCM data (raw sound data) to 'channels_data'.
	friend std::vector<std::vector<short>> extract_data_int16(const char* filename);

	// Reads file 'filename' and puts PCM data (raw sound data) to 'channels_data'.
	friend std::vector<std::vector<short>> extract_data_int8(const char* filename);

	// Makes mono PCM data from stereo 'source'.
	friend void make_wav_file(const char* filename, int sample_rate, std::vector<std::vector<short>> const& channels_data);

private:

	// Checks header validity.
	void check_header(size_t file_size_bytes) const {
		// Go to wav_header.h for details

		if (chunkId[0] != 0x52 ||
			chunkId[1] != 0x49 ||
			chunkId[2] != 0x46 ||
			chunkId[3] != 0x46)
		{
			printf("HEADER_RIFF_ERROR\n");
			throw header_riff_error();
		}

		if (chunkSize != file_size_bytes - 8) {
			printf("HEADER_FILE_SIZE_ERROR\n");
			throw header_file_size_error();
		}

		if (format[0] != 0x57 ||
			format[1] != 0x41 ||
			format[2] != 0x56 ||
			format[3] != 0x45)
		{
			printf("HEADER_WAVE_ERROR\n");
			throw header_wave_error();
		}

		if (subchunk1Id[0] != 0x66 ||
			subchunk1Id[1] != 0x6d ||
			subchunk1Id[2] != 0x74 ||
			subchunk1Id[3] != 0x20)
		{
			printf("HEADER_FMT_ERROR\n");
			throw header_fmt_error();
		}

		if (audioFormat != 1) {
			printf("HEADER_NOT_PCM\n");
			throw header_not_pcm();
		}

		if (subchunk1Size != 16) {
			printf("HEADER_SUBCHUNK1_ERROR\n");
			throw header_subchunk1_error();
		}

		if (byteRate != sampleRate * numChannels * bitsPerSample / 8) {
			printf("HEADER_BYTES_RATE_ERROR\n");
			throw header_bytes_rate_error();
		}

		if (blockAlign != numChannels * bitsPerSample / 8) {
			printf("HEADER_BLOCK_ALIGN_ERROR\n");
			throw header_block_align_error();
		}

		if (subchunk2Id[0] != 0x64 ||
			subchunk2Id[1] != 0x61 ||
			subchunk2Id[2] != 0x74 ||
			subchunk2Id[3] != 0x61)
		{
			printf("HEADER_FMT_ERROR\n");
			throw header_fmt_error();
		}

		if (subchunk2Size != file_size_bytes - 44)
		{
			printf("HEADER_SUBCHUNK2_SIZE_ERROR\n");
			throw header_subchunk2_size_error();
		}
	}

	// Fills header information, using input parameters. This function calls prefill_header() itself.
	void fill_header(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan) {
		if (bits_per_sample != 16) {
			throw unsupported_format();
		}

		if (chan_count < 1) {
			throw bad_params();
		}
		prefill_header();

		int file_size_bytes = 44 + chan_count * (bits_per_sample / 8) * samples_count_per_chan;

		sampleRate = sample_rate;
		numChannels = chan_count;
		bitsPerSample = 16;

		chunkSize = file_size_bytes - 8;
		subchunk2Size = file_size_bytes - 44;

		byteRate = sampleRate * numChannels * bitsPerSample / 8;
		blockAlign = numChannels * bitsPerSample / 8;
	}

	// Fills 'header_ptr' with default values.
	void prefill_header()
	{
		// Go to wav_header.h for details

		chunkId[0] = 0x52;
		chunkId[1] = 0x49;
		chunkId[2] = 0x46;
		chunkId[3] = 0x46;

		format[0] = 0x57;
		format[1] = 0x41;
		format[2] = 0x56;
		format[3] = 0x45;

		subchunk1Id[0] = 0x66;
		subchunk1Id[1] = 0x6d;
		subchunk1Id[2] = 0x74;
		subchunk1Id[3] = 0x20;

		subchunk2Id[0] = 0x64;
		subchunk2Id[1] = 0x61;
		subchunk2Id[2] = 0x74;
		subchunk2Id[3] = 0x61;

		audioFormat = 1;
		subchunk1Size = 16;
		bitsPerSample = 16;

	}

	char chunkId[4];

	uint32_t chunkSize;

	char format[4];

	char subchunk1Id[4];

	uint32_t subchunk1Size;

	uint16_t audioFormat;

	uint16_t numChannels;

	uint32_t sampleRate;

	uint32_t byteRate;

	uint16_t blockAlign;

	uint16_t bitsPerSample;

	char subchunk2Id[4];

	uint32_t subchunk2Size;
};

wav_header::wav_header(const char* filename) {
	*this = wav_header(); // Fill header with zeroes.

	FILE* f = fopen(filename, "rb");
	if (!f) {
		throw io_error();
	}

	size_t blocks_read = fread(this, sizeof(wav_header), 1, f);
	if (blocks_read != 1) {
		// can't read header, because the file is too small.
		throw bad_format();
	}

	fseek(f, 0, SEEK_END); // seek to the end of the file
	size_t file_size = ftell(f); // current position is a file size!
	fclose(f);

	try {
		check_header(file_size);
	}
	catch (wav_header_error) {
		throw bad_format();
	}
}

void wav_header::print_info() const {
	printf(" audioFormat   %u\n", audioFormat);
	printf(" numChannels   %u\n", numChannels);
	printf(" sampleRate    %u\n", sampleRate);
	printf(" bitsPerSample %u\n", bitsPerSample);
	printf(" byteRate      %u\n", byteRate);
	printf(" blockAlign    %u\n", blockAlign);
	printf(" chunkSize     %u\n", chunkSize);
	printf(" subchunk1Size %u\n", subchunk1Size);
	printf(" subchunk2Size %u\n", subchunk2Size);
}

std::vector<std::vector<short>> extract_data_int16(const char* filename)
{
	wav_header header;
	try {
		// Problems with reading a header.
		header = wav_header(filename);
	}
	catch (wav_error e) {
		throw;
	}

	if (header.bitsPerSample != 16) {
		// Only 16-bit samples is supported.
		throw unsupported_format();
	}

	FILE* f = fopen(filename, "rb");
	if (!f) {
		throw io_error();
	}
	fseek(f, 44, SEEK_SET); // Seek to the begining of PCM data.

	int chan_count = header.numChannels;
	int samples_per_chan = (header.subchunk2Size / sizeof(short)) / chan_count;

	// 1. Reading all PCM data from file to a single vector.
	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_per_chan);
	size_t read_bytes = fread(all_channels.data(), 1, header.subchunk2Size, f);
	if (read_bytes != header.subchunk2Size) {
		char * error_text;
		sprintf(error_text, "extract_data_int16() read only %zu of %u\n", read_bytes, header.subchunk2Size);
		throw io_error(error_text);
	}
	fclose(f);


	// 2. Put all channels to its own vector.
	std::vector<std::vector<short>> channels_data(chan_count);
	for (auto & ch : channels_data) {
		ch.resize(samples_per_chan);
	}

	for (int ch = 0; ch < chan_count; ch++) {
		std::vector<short>& chdata = channels_data[ch];
		for (size_t i = 0; i < samples_per_chan; i++) {
			chdata[i] = all_channels[chan_count * i + ch];
		}
	}
	return channels_data;
}

std::vector<std::vector<short>> extract_data_int8(const char* filename)
{
	wav_header header;
	try {
		// Problems with reading a header.
		header = wav_header(filename);
	}
	catch (wav_error e) {
		throw;
	}

	if (header.bitsPerSample != 8) {
		// Only 16-bit samples is supported.
		throw unsupported_format();
	}

	FILE* f = fopen(filename, "rb");
	if (!f) {
		throw io_error();
	}
	fseek(f, 44, SEEK_SET); // Seek to the begining of PCM data.

	int chan_count = header.numChannels;
	int samples_per_chan = (header.subchunk2Size / sizeof(short)) / chan_count;

	// 1. Reading all PCM data from file to a single vector.
	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_per_chan);
	size_t read_bytes = fread(all_channels.data(), 1, header.subchunk2Size, f);
	if (read_bytes != header.subchunk2Size) {
		char * error_text;
		sprintf(error_text, "extract_data_int8() read only %zu of %u\n", read_bytes, header.subchunk2Size);
		throw io_error(error_text);
	}
	fclose(f);


	// 2. Put all channels to its own vector.
	std::vector<std::vector<short>> channels_data(chan_count);
	for (auto & ch : channels_data) {
		ch.resize(samples_per_chan);
	}

	for (int ch = 0; ch < chan_count; ch++) {
		std::vector<short>& chdata = channels_data[ch];
		for (size_t i = 0; i < samples_per_chan; i++) {
			chdata[i] = all_channels[chan_count * i + ch];
		}
	}
	return channels_data;
}

std::vector<std::vector<short>> make_mono(const std::vector<std::vector<short>> source) {
	int chan_count = (int) source.size();

	if (chan_count != 2) {
		throw bad_params();
	}

	int samples_count_per_chan = (int) source[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (source[ch].size() != (size_t) samples_count_per_chan) {
			throw bad_params();
		}
	}

	std::vector<short> mono(samples_count_per_chan);

	// Mono channel is an arithmetic mean of all (two) channels.
	for (size_t i = 0; i < samples_count_per_chan; i++) {
		mono[i] = (source[0][i] + source[1][i]) / 2;
	}
	return std::vector<std::vector<short>>(1, mono);
}


// Adds a reverberation (it's like an echo).
std::vector<std::vector<short>> make_reverb(std::vector<std::vector<short>> sounds, int sample_rate, double delay_seconds, float decay) {
	int chan_count = (int) sounds.size();

	if (chan_count < 1) {
		throw bad_params();
	}

	int samples_count_per_chan = (int) sounds[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (sounds[ch].size() != (size_t) samples_count_per_chan) {
			throw bad_params();
		}
	}

	int delay_samples = (int) (delay_seconds * sample_rate);


	for (size_t ch = 0; ch < chan_count; ch++) {
		std::vector<float> tmp;
		tmp.resize(sounds[ch].size());

		// Convert signal from short to float
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			tmp[i] = sounds[ch][i];
		}

		// Add a reverb
		for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
			tmp[i + delay_samples] += decay * tmp[i];
		}

		// Find maximum signal's magnitude
		float max_magnitude = 0.0f;
		for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
			if (abs(tmp[i]) > max_magnitude) {
				max_magnitude = abs(tmp[i]);
			}
		}

		// Signed short can keep values from -32768 to +32767,
		// After reverb, usually there are values large 32000.
		// So we must scale all values back to [ -32768 ... 32768 ]
		float norm_coef = 30000.0f / max_magnitude;
		printf("max_magnitude = %.1f, coef = %.3f\n", max_magnitude, norm_coef);

		// Scale back and transform floats to shorts.
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			sounds[ch][i] = (short) (norm_coef * tmp[i]);
		}
	}
	return sounds;

}

// Changes the tone of voice
std::vector<std::vector<short>> change_tone(std::vector<std::vector<short>> sounds, int sample_rate, float decay) {
	int chan_count = (int) sounds.size();

	if (chan_count < 1) {
		throw bad_params();
	}

	int samples_count_per_chan = (int) sounds[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (sounds[ch].size() != (size_t) samples_count_per_chan) {
			throw bad_params();
		}
	}

	for (size_t ch = 0; ch < chan_count; ch++) {
		std::vector<float> tmp;
		tmp.resize(sounds[ch].size());

		// Convert signal from short to float
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			tmp[i] = sounds[ch][i];
		}

		// Add a reverb
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			tmp[i] *= decay;
		}

		// Find maximum signal's magnitude
		float max_magnitude = 0.0f;
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			if (abs(tmp[i]) > max_magnitude) {
				max_magnitude = abs(tmp[i]);
			}
		}

		// Signed short can keep values from -32768 to +32767,
		// After reverb, usually there are values large 32000.
		// So we must scale all values back to [ -32768 ... 32768 ]
		float norm_coef = 30000.0f / max_magnitude;
		printf("max_magnitude = %.1f, coef = %.3f\n", max_magnitude, norm_coef);

		// Scale back and transform floats to shorts.
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			sounds[ch][i] = (short)(norm_coef * tmp[i]);
		}
	}
	return sounds;

}


// Creates a new WAV file 'filename', using 'sample_rate' and PCM data from 'channels_data'.
void make_wav_file(const char* filename, int sample_rate, std::vector<std::vector<short>> const& channels_data) {
	wav_header header;

	int chan_count = (int) channels_data.size();

	if (chan_count < 1) {
		throw bad_params();
	}

	int samples_count_per_chan = (int) channels_data[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (channels_data[ch].size() != (size_t) samples_count_per_chan) {
			throw bad_params();
		}
	}

	try {
		header.fill_header(chan_count, 16, sample_rate, samples_count_per_chan);
	}
	catch (wav_error e) {
		throw e;
	}

	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_count_per_chan);

	for (int ch = 0; ch < chan_count; ch++) {
		const std::vector<short>& chdata = channels_data[ch];
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			all_channels[chan_count * i + ch] = chdata[i];
		}
	}

	FILE* f = fopen(filename, "wb");
	fwrite(&header, sizeof(wav_header), 1, f);
	fwrite(all_channels.data(), sizeof(short), all_channels.size(), f);
	if (!f) {
		throw io_error();
	}
	fclose(f);
}

