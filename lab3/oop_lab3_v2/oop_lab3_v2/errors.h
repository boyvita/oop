#pragma once
#include <exception>
#include <stdexcept>

//WAV_ERRORS
class wav_error : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class io_error : public wav_error {
public:
	using wav_error::wav_error;
	io_error() : wav_error("IO_ERROR") {}
};

class bad_format : public wav_error {
public:
	using wav_error::wav_error;
	bad_format() : wav_error("BAD_FORMAT") {}
};

class unsupported_format : public wav_error {
public:
	using wav_error::wav_error;
	unsupported_format() : wav_error("UNSUPPORTED_FORMAT") {}
};

class bad_params : public wav_error {
public:
	using wav_error::wav_error;
	bad_params() : wav_error("BAD_PARAMS") {}
};

class data_size_error : public wav_error {
public:
	using wav_error::wav_error;
	data_size_error() : wav_error("DATA_SIZE_ERROR") {}
};


//HEADERS

class wav_header_error : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class header_riff_error : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_riff_error() : wav_header_error("HEADER_RIFF_ERROR") {}
};

class header_file_size_error : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_file_size_error() : wav_header_error("HEADER_FILE_SIZE_ERROR") {}
};


class header_wave_error : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_wave_error() : wav_header_error("HEADER_WAVE_ERROR") {}
};

class header_fmt_error : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_fmt_error() : wav_header_error("HEADER_FMT_ERROR") {}
};

class header_not_pcm : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_not_pcm() : wav_header_error("HEADER_NOT_PCM") {}
};

class header_subchunk1_error : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_subchunk1_error() : wav_header_error("HEADER_SUBCHUNK1_ERROR") {}
};

class header_bytes_rate_error : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_bytes_rate_error() : wav_header_error("HEADER_BYTES_RATE_ERROR") {}
};


class header_block_align_error : public wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_block_align_error() : wav_header_error("HEADER_BLOCK_ALIGN_ERROR") {}
};


class header_subchunk2_size_error : public  wav_header_error {
public:
	using wav_header_error::wav_header_error;
	header_subchunk2_size_error() : wav_header_error("HEADER_SUBCHUNK2_SIZE_ERROR") {}
};
