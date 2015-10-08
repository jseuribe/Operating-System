#ifndef PCB_H
#define PCB_H

#include "file.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class PCB;
ostream& operator<<(ostream& ost, const PCB& rhs);

template <class T>
string number_to_string(const T& input);

class PCB{
public:
	PCB();
	PCB(int pid, float tau_prev);
	PCB(const PCB& inp);
	PCB &operator=( const PCB &inp);

	void new_operation(char operation);

	void file_parameters(file new_data);

	void changeJob(const char inp);

	int get_pid();

	void set_tau_n( float time);

    float return_tau_current() const;

	void calc_next_burst(float alpha, float cpu_burst);// tau_n = tau_n * alpha + (1 - alpha) * burst

	float return_time();

	float return_total_time();

	void increment_time(float cpu_burst);//updates t_n and tau_current upon completing a cpu burst. Also updates avg

	void change_comparison();

	void termination_update();

	int return_mem();

	void set_mem( int mem_loc );

	bool operator<(const PCB& rhs) const;
	bool operator>(const PCB& rhs) const;

	int return_file_loc();

	void print_time_stats();

	void print_disk_stats();//A print, but for the case where we also print the mem_loc


	friend ostream& operator<<(ostream& ost, const PCB& rhs);

	template <class T>
	friend string number_to_string(const T& input);

private:
	int pid;

	int mem_accessed;//cylinder location

	int total_cpu_uses;//# of times CPU is used

	float tau_current;//Tau value decremented by cpu bursts.

	int average_cpu_burst;// CPU burst time / cpu uses

	float time_n;// t_prev, current cpu_burst

	float tau_n;// tau_prev-1; tau_prev+1 = t_prev(alpha) + (1 - alpha)tau_prev-1

	float total_time_spent; //compile all real burst times. Do it when you calc_next_burst

	char operation;

	bool comparison_state;

	file file_data;



};

#endif
