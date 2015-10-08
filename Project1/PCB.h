#include "file.h"
#include <iostream>
#include <sstream>

using namespace std;

class PCB;
ostream& operator<<(ostream& ost, const PCB& rhs);

class PCB{
public:
	PCB();
	PCB(int pid, bool is_in_CPU);
	PCB(const PCB& inp);
	PCB &operator=(const PCB &inp);
	void change_CPU_state();
	void new_operation(char operation);
	void file_parameters(file new_data);
	void changeJob(const char inp);
	int get_pid();
	friend ostream& operator<<(ostream& ost, const PCB& rhs);
private:
	int pid;
	bool is_in_CPU;
	char operation;
	file file_data;

};

