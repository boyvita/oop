#include <iostream>
#include "wav_core.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << "************** | WavCore | **************" << '\n';


	// ################  Tests for WavCore  ################

	const char* input_fname = "0.wav";
	const char* output_mono_fname = "mono.wav";
	const char* output_tone_fname = "tone.wav";
	const char* output_reverb_fname = "reverb.wav";
	wav_header header;
	// #### Opening WAV file, checking header.
	try {
		printf(">>>> wav_header(%s)\n", input_fname);
		header = wav_header(input_fname);
		printf(">>>> success\n");
	}
	catch (wav_error err) {
		cerr << "read_header() error: " << err.what() << '\n';
		return 0;
	}
	cout << "********************\n";


	// #### Printing header.
	printf(">>>> print_info()\n");
	header.print_info();
	cout << "********************\n";


	// #### Reading PCM data from file.
	vector<vector<short>> chans_data;
	try {
		printf(">>>> extract_data_int16()\n");
		chans_data = extract_data_int16(input_fname);
		printf(">>>> success\n");
	}
	catch (wav_header_error err) {
		cerr << "extract_data_int16() error: " << err.what() << '\n';
		return 0;
	}
	cout << "********************\n";

	// #### Making new WAV file using edited PCM data.
	try {
		printf(">>>> make_wav_file( %s )\n", output_mono_fname);
		make_wav_file(output_mono_fname, 44100, make_mono(chans_data));
		printf(">>>> success\n");
	}
	catch (wav_header_error err) {
		cerr << "make_wav_file() error: " << err.what() << '\n';
		return 0;
	}
	cout << "********************\n";

	// #### Making new WAV file with changed tone.
	try {
		printf(">>>> make_wav_file( %s )\n", output_tone_fname);
		make_wav_file(output_tone_fname, 44100, change_tone(chans_data, 44100, 0.2));
		printf(">>>> success\n");
	}
	catch (wav_header_error err) {
		cerr << "make_wav_file() error: " << err.what() << '\n';
		return 0;
	}
	cout << "********************\n";

	// #### Making new WAV file with reverberation.
	try {
		printf(">>>> make_wav_file( %s )\n", output_reverb_fname);
		make_wav_file(output_reverb_fname, 44100, make_reverb(chans_data, 44100, 1, 0.5));
		printf(">>>> success\n");
	}
	catch (wav_header_error err) {
		cerr << "make_wav_file() error: " << err.what() << '\n';
		return 0;
	}
	cout << "********************\n";
	
	return 0;
}
