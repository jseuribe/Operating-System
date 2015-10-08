#include "PCB.h"
#include "file.h"
#include "queue_hybrid_v_2.cpp"
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

		
		ost  << left << setw(7) << rhs.pid << left << setw(11) << rhs.file_data << 
			 left << setw(11) << s_tau << "\n";
		ost << "-------------" << endl;

		return ost;
	}

	void PCB::print_out()
	{
		string s_tau;
		s_tau = number_to_string(tau_n);

		
		cout  << left << setw(7) << pid << left << setw(11) << file_data << 
			 left << setw(11) << s_tau << "\n";
		cout << "-------------" << endl;
		print_page_table();
		cout << "-------------" << endl;


	}

	void PCB::print_disk_stats()
	{
		string s_tau; string s_mem;

		s_tau = number_to_string(tau_n); s_mem = number_to_string(mem_accessed);

			cout << left << setw(7) << pid << left << file_data
			 << left << setw(11) << s_tau <<  left << setw(11) << s_mem << endl;
		cout << "-------------" << endl;
		print_page_table();
				cout << "-------------" << endl;


	}
	
	void PCB::print_time_stats()
	{
		string s_total, s_avg, s_tau_c, s_tau;
		s_total = number_to_string(total_time_spent);
		s_avg = number_to_string(average_cpu_burst);
		s_tau_c = number_to_string(tau_current);
		s_tau = number_to_string(tau_n);


			cout << left << setw(7) << pid << left << setw(11) << s_total
			 << left << setw(11) << s_avg << left << setw(11) << s_tau_c
			 << left << setw(11) << s_tau << endl;

		cout << "-------------" << endl;
		print_page_table();
				cout << "-------------" << endl;


	}

	PCB::PCB()
	{
		pid = 0;
		operation = 'n';
		file_data = file();
		tau_n = 0.00;
		total_time_spent = 0.00;
		comparison_state = 1;
		total_cpu_uses = 0;
		average_cpu_burst = 0;
		mem_accessed = 0;
		tau_current = 0;
		page_table_size = 0;
		mem_alloc = 0;
		delta = 0;
	}

	PCB::PCB(int pid, float tau_prev, int mem_needed, int n_frames)
	{
		this->pid = pid;
		operation = 'n';
		file_data = file();
		tau_n = tau_prev;
		total_time_spent = 0.00;
		comparison_state = 1;
		total_cpu_uses = 0;
		average_cpu_burst = 0;
		mem_accessed = 0;
		tau_current = tau_n;
		mem_alloc = mem_needed;
		page_table_size = n_frames;
		delta = 0;

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
		page_table = inp.page_table;
		page_table_size = inp.page_table_size;
		mem_alloc = inp.mem_alloc;
		delta = inp.delta;
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
		page_table = inp.page_table;
		page_table_size = inp.page_table_size;
		mem_alloc = inp.mem_alloc;

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

	void PCB::set_pid(int inp)
	{
		pid = inp;
	}

	void PCB::set_tau_n(float time)
	{
		tau_n = time;
		tau_current = time;
	}

	float PCB::return_tau_current()
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

	void PCB::change_comparison(string state)
	{
		if( state == "time") comparison_state = 1;
		else if( state == "disk" ) comparison_state = 2;
		else if( state == "mem" ) comparison_state = 3; //
	}

	bool PCB::operator<( const PCB &rhs) const
	{
		if( comparison_state == 1 ) return tau_current < rhs.tau_current;
		else if( comparison_state == 2) return mem_accessed < rhs.mem_accessed;
		else if ( comparison_state == 3) return page_table_size < rhs.page_table_size;

	}

	bool PCB::operator>( const PCB &rhs ) const
	{
		if( comparison_state == 1 ) return tau_n > rhs.tau_n;
		else if( comparison_state == 2 ) return mem_accessed > rhs.mem_accessed;
		else if( comparison_state == 3 ) return page_table_size > rhs.page_table_size;
	}

	string PCB::return_file_loc()
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

		if(total_cpu_uses != 0) average_cpu_burst = total_time_spent / total_cpu_uses;//new average is calculated
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

	bool PCB::operator==(const PCB &inp) const
	{	
		return inp.pid == pid;
	}

	vector<int> PCB::get_page_table()//returns the frames, corresponding to their virtual location (index in pg tbl)
	{
		return page_table;
	}

	int PCB::get_mem_size()
	{
		return page_table_size;
	}

	void PCB::alloc_frame_list(LinkedList<int> &frame_list)
	{
		int curr_frame = 0, curr_t_pos;
		while( ! frame_list.IsEmpty() )
		{
			curr_frame = frame_list.deQueue();
			page_table.push_back( curr_frame );
			curr_t_pos++;
		}
		page_table_size = page_table.size();
	}

	int PCB::return_mem_alloc()
	{
		return mem_alloc;
	}

	void PCB::set_mem_alloc(int inp)
	{
		mem_alloc = inp;
	}

	int PCB::return_frames()
	{
		return page_table_size;
	}

	void PCB::key_decrease(float dec)
	{
		delta = tau_current;
		tau_current += dec;
	}

	void PCB::key_normalize()
	{
		tau_current = delta;
	}

	int PCB::return_comp_state()
	{
		return comparison_state;
	}

	int PCB::fetch_physical_frame(int logical_address)
	{
		return page_table[ logical_address ];
	}

	int PCB::return_highest_GCD(int n)
	{	
		int current = 1;
  		for(int i = 1; i <= 32; i++)
  		{
    		if( (n % i == 0)  ) current = i;
  		}
  		return current;
	}

	void PCB::print_page_table()
	{
		int rows = return_highest_GCD( page_table_size );
		int columns = page_table_size / rows;
		int temp;
		if( columns < 48 )
		{
			temp = columns;
			columns = rows;
			rows = temp;
		}

		cout << "PTbl(" << pid << ")" << endl;
		for(int i = 0; i < columns; i++)
		{
			cout << "L" << "   " << "P" << " |";
		}
		cout << endl;
		int external_counter = 0;
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				cout << left << setw(4) << external_counter << left << setw(4) << page_table[external_counter] << "|";
				external_counter++;
			}
			cout << endl;
		}
	}
