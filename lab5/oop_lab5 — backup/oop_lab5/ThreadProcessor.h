#pragma once
#include <string>
#include <exception>
#include <fstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include "Calculatable.h"

using namespace std;

template <class C>
class ThreadProcessor {
public:
	ThreadProcessor();
	~ThreadProcessor();
	ThreadProcessor(string input_file, string output_file, int8_t count_threads, uint16_t pool_max_size);

	bool is_finished();
	bool is_paused();
	void start();
	void pause();
	void resume();
	void stop();

private:
	void trigger();
	void read();
	void write();
	void ThreadProcessor<C>::task(shared_ptr<C> f);
	void statistic();

	mutex mtx;
	thread read_thread;
	thread write_thread;
	thread trigger_thread;
	list<pair<shared_ptr<C>, thread>> thread_pool;

	bool started;
	bool all_read;
	bool pause_sig;
	bool exit_sig;
	bool finished;

	uint32_t count_threads;
	uint32_t count_busy_threads;
	uint32_t pool_max_size;

	uint32_t count_read;
	uint32_t count_calculated;
	uint32_t count_written;

	string input_file;
	string output_file;

	ifstream fin;
	ofstream fout;
};
#include "ThreadProcessor_impl.hpp"