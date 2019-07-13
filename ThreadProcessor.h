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
	ThreadProcessor() = delete;

	~ThreadProcessor() {
		while (!finished);
		if (!pause_sig) {
			if (read_thread.joinable())
				read_thread.join();
			if (write_thread.joinable())
				write_thread.join();
			}
		if (trigger_thread.joinable())
				trigger_thread.join();
		
	}
	ThreadProcessor(string input_file, string output_file, int8_t count_threads = 8, uint16_t pool_max_size = 100)
		: input_file(input_file), output_file(output_file),
		count_threads(count_threads), count_busy_threads(0), pool_max_size(pool_max_size),
		count_read(0), count_calculated(0), count_written(0),
		all_read(0), pause_sig(0), exit_sig(0), finished(0)
	{}

	bool is_finished() {
		return finished;
	}

	bool is_paused() {
		return pause_sig;
	}

	void start() {
		cerr << "input file: " << input_file << '\n'; 
		cerr << "output file: " << output_file << '\n';
		cerr << "max count of threads" << count_threads << '\n';
		cerr << "pool max size" << pool_max_size << '\n';
		fin.open(input_file.c_str());
		fout.open(output_file.c_str());
		read_thread = thread(&ThreadProcessor::read, this);
		write_thread = thread(&ThreadProcessor::write, this);
		trigger_thread = thread(&ThreadProcessor::trigger, this);
	}

	void pause() {
		pause_sig = true;
		if (read_thread.joinable())
			read_thread.join();
		if (write_thread.joinable())
			write_thread.join();
		cerr << "count read lines: " << count_read << '\n';
		cerr << "count calculated: " << count_calculated << '\n';
		cerr << "count written: " << count_written << '\n';
	}

	void resume() {
		if (pause_sig) {
			pause_sig = false;
			fout.open(output_file, ios_base::app);
			read_thread = thread(&ThreadProcessor::read, this);
			write_thread = thread(&ThreadProcessor::write, this);
		}
	}

	void stop() {
		exit_sig = true;
		if (pause_sig)
			finished = true;
		cerr << "Process stopped\n";
		cerr << "count read lines: " << count_read << '\n';
		cerr << "count calculated: " << count_calculated << '\n';
		cerr << "count written: " << count_written << '\n';

	}

private:
	void trigger() {
		while (!finished) {
			string s;
			getline(cin, s);
			cin.clear();
			if (s == "exit") {
				stop();
				return;
			}
			else if (s == "pause")
				pause();
			else if (s == "resume")
				resume();
		}
	}

	void task(shared_ptr<C> f) {
		f->calculate();
		count_calculated++;
		count_busy_threads--;
	}

	void read() {
		while (true) {
			if (pause_sig) {
				return;
			}
			mtx.lock();
			uint16_t pool_size = thread_pool.size();
			mtx.unlock();
			if (count_busy_threads <= count_threads && pool_size <= pool_max_size) {
				shared_ptr<C> f;
				f = make_shared<C>();
				try {
					f->read(fin);
					count_read++;
				}
				catch (exception err) {
					all_read = true;
					return;
				}
				mtx.lock();
				thread_pool.push_back(make_pair(f, thread(&ThreadProcessor::task, this, f)));
				if (exit_sig)
					return;
				mtx.unlock();

				count_busy_threads++;
			}
		}
	}

	void write() { 
		while (true) {
			mtx.lock();
			if (!thread_pool.empty()) {
				if (thread_pool.front().second.joinable())
					thread_pool.front().second.join();
				thread_pool.front().first->print_answer(fout);
				count_written++;
				thread_pool.pop_front();
				if (exit_sig) {
					mtx.unlock();
					fout.close();
					finished = true;
					for (auto it = thread_pool.begin(); it != thread_pool.end(); ++it)
						if (it->second.joinable())
							it->second.detach();
					return;
				}
				if (pause_sig) {
					mtx.unlock();
					fout.close();
					return;
				}
			}
			else if (all_read) {
				mtx.unlock();
				fout.close();
				finished = true;
				cerr << "Process finshed\nPush any button\n";
				return;
			}
			mtx.unlock();
		}
	}

	mutex mtx;
	thread read_thread;
	thread write_thread;
	thread trigger_thread;
	list<pair<shared_ptr<C>, thread>> thread_pool;

	bool all_read;
	bool pause_sig;
	bool exit_sig;
	bool finished;

	uint8_t count_threads;
	uint8_t count_busy_threads;
	uint8_t pool_max_size;

	uint16_t count_read;
	uint16_t count_calculated;
	uint16_t count_written;

	string input_file;
	string output_file;

	ifstream fin;
	ofstream fout;
};