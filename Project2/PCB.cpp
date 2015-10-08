#include "PCB.h"
#include "file.h"
#include <iostream>
#include <string>
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
		string s_tau;
		s_tau = number_to_string(rhs.tau_n);

		ost  << left << setw(11) << rhs.pid << left << setw(11) << rhs.file_data << 
			    left << setw(11) << s_tau << "\n";
		return ost;
	}

	void PCB::print_disk_stats()
	{
		string s_tau; string s_mem;

		s_tau = number_to_string(tau_n); s_mem = number_to_string(mem_accessed);

		cout << left << setw(11) << pid << left << file_data
			 << left << setw(11) << s_tau <<  left << setw(11) << s_mem << endl;
	}
	
	void PCB::print_time_stats()
	{
		string s_total, s_avg, s_tau_c, s_tau;
		s_total = number_to_string(total_time_spent);
		s_avg = number_to_string(average_cpu_burst);
		s_tau_c = number_to_string(tau_current);
		s_tau = number_to_string(tau_n);

		cout << left << setw(11) << pid << left << setw(11) << s_total
			 << left << setw(11) << s_avg << left << setw(11) << s_tau_c
			 << left << setw(11) << s_tau << endl;
	}

	PCB::PCB()
	{
		pid = 0;
		operation = 'n';
		file_data = file();
		tau_n = 0.00;
		total_time_spent = 0.00;
		comparison_state = false;
		total_cpu_uses = 0;
		average_cpu_burst = 0;
		mem_accessed = 0;
		tau_current = 0;
	}

	PCB::PCB(int pid, float tau_prev)
	{
		this->pid = pid;
		operation = 'n';
		file_data = file();
		tau_n = tau_prev;
		total_time_spent = 0.00;
		comparison_state = false;
		total_cpu_uses = 0;
		average_cpu_burst = 0;
		mem_accessed = 0;

		tau_current = tau_n;

	}

	PCB::PCB(const PCB& inp)
	{
		pid = inp.pid;
		operation = inp.operation;
		file_data = inp.file_data;
		tau_n = inp.tau_n;
		total_time_spent = inp.total_time_spent;
		comparison_state = inp.comparison_state;
		total_cpu_uses = inp.total_cpu_uses;
		average_cpu_burst = inp.average_cpu_burst;
		mem_accessed = inp.mem_accessed;
		tau_current = inp.tau_current;
	}

	PCB &PCB::operator=( const PCB &inp)
	{
		pid = inp.pid;
		operation = inp.operation;
		file_data = inp.file_data;
		tau_n = inp.tau_n;
		total_time_spent = inp.total_time_spent;
		comparison_state = inp.comparison_state;
		total_cpu_uses = inp.total_cpu_uses;
		average_cpu_burst = inp.average_cpu_burst;
		tau_current = inp.tau_current;
		mem_accessed = inp.mem_accessed;

		return *this;
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


	void PCB::set_tau_n(float time)
	{
		tau_n = time;
		tau_current = time;
	}

	float PCB::return_tau_current() const
	{
		return tau_current;
	}

	void PCB::calc_next_burst(float alpha, float cpu_burst)//Used when sent to a device
	{
		tau_current -= cpu_burst;//process used the cpu a bit. factors into queue priority

		total_time_spent += cpu_burst;//total time spent goes up

		time_n += cpu_burst;//CURRENT CPU BURST GOES UP

		total_cpu_uses++;//cpu uses go up

		average_cpu_burst = total_time_spent / total_cpu_uses;//new average is calculated

		//!!!!!!!!!NEXT CPU TIME IS DECIDED HERE!!!!!!!!!!!

		tau_n = tau_n * alpha + (1.00 - alpha)*time_n; //The proc's next cpu burst is decided.

		tau_current = tau_n;//update remaining processes.

		time_n = 0; //The process has now spent exactly no time yet in the cpu since the last burst

		//!!!!!!!!!NEXT CPU TIME IS DECIDED HERE!!!!!!!!!!!


	}

	float PCB::return_total_time()
	{
		return total_time_spent;
	}

	void PCB::change_comparison()
	{
		comparison_state = ! comparison_state; //simple flip.
	}

	bool PCB::operator<( const PCB &rhs) const
	{
		if(! comparison_state ) return tau_current < rhs.tau_current;
		else return mem_accessed < rhs.mem_accessed;
	}

	bool PCB::operator>( const PCB &rhs ) const
	{
		if( ! comparison_state ) return tau_n > rhs.tau_n;
		else return mem_accessed > rhs.mem_accessed;
	}

	int PCB::return_file_loc()
	{
		return file_data.return_loc();
	}

	void PCB::termination_update()
	{
		cout << "Process " << pid << " stats:" << endl;
		cout << "Total time" << " " << "average cpu burst" << endl;
		cout << left << setw(11) << total_time_spent << left << setw(11) << average_cpu_burst << endl;
	}

	int PCB::return_mem()
	{
		return mem_accessed;
	}

	void PCB::set_mem( int mem_loc )
	{
		mem_accessed = mem_loc;
	}

	void PCB::increment_time( float cpu_burst )//used when interrupted, NOT when being sent to a device
	{
		tau_current -= cpu_burst;//process used the cpu a bit. factors into queue priority

		total_time_spent += cpu_burst;//total time spent goes up

		time_n += cpu_burst;//CURRENT CPU BURST GOES UP

		total_cpu_uses++;//cpu uses go up

		average_cpu_burst = total_time_spent / total_cpu_uses;//new average is calculated
	}

	template<class T>
    string number_to_string(const T& input)
    {
        ostringstream buffer;
        buffer << input;

        string output = buffer.str();

        if(output.length() > 10) return output.substr(0, 9);
        else return output;
    }
