#ifndef PCB_H
#define PCB_H

#include "PCB.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

/*class PCB{
public:
	PCB();
	PCB(int pid, bool is_in_CPU);
	PCB(const process& inp);
	operator=(const process& inp);
private:
	int pid;
	bool is_in_CPU;
	char operation;
	file file_data;

};
*/

	ostream& operator<<(ostream& ost, const PCB& rhs)
	{
		ost  << setw(16) << left << rhs.pid << setw(16) << left << rhs.file_data << "\n";
		return ost;
	}
	
	PCB::PCB()
	{
		pid = 0;
		is_in_CPU = false;
		operation = 'n';
		file_data = file();
	}

	PCB::PCB(int pid, bool is_in_CPU)
	{
		this->pid = pid;
		this->is_in_CPU = is_in_CPU;
		operation = 'n';
		file_data = file();
	}

	PCB::PCB(const PCB& inp)
	{
		pid = inp.pid;
		is_in_CPU = inp.is_in_CPU;
		operation = inp.operation;
		file_data = inp.file_data;
	}

	PCB &PCB::operator=(const PCB &inp)
	{
		pid = inp.pid;
		is_in_CPU = inp.is_in_CPU;
		operation = inp.operation;
		file_data = inp.file_data;
	}

	void PCB::change_CPU_state()
	{
		this->is_in_CPU = !(is_in_CPU);
	}

	void PCB::new_operation(char operation)
	{
		this->operation = operation;
	}

	void PCB::file_parameters(file new_data)
	{
		file_data = new_data;
	}

	void PCB::changeJob(const char inp)
	{
		operation = inp;
	}

	int PCB::get_pid()
	{
		return pid;
	}

#endif
