#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <queue>
#include <vector>
#include <cmath>
#include <stack>
#include <map>

#include "BinaryHeap.h"
#include "queue_hybrid_v_2.cpp"
#include "PCB.h"
#include "disk.h"

using namespace std;

template <class T>
void generate_new_PCB(T &ready_queue, int &pid_tracker, float tau_prev, priority_queue<PCB> &job_pool, vector<int> &sys_mem, 
                      queue<int> &free_frames, int total_mem, int page_size, int max_size);
template <class T>
void print_heap( T the_queue );

void print_queue(LinkedList<PCB> &the_queue);

void print_queue_array(LinkedList<PCB> *queue_array, const string &device_name, int array_size);

void print_disk_array(disk *disks, string device, int num);

void admit_into_queue(LinkedList<PCB> &queue, PCB &new_admission);

void device_resolution(string &input, char &case_switch, int &device_num);

template <class T>
void system_call_handler(LinkedList<PCB> *device_array, T &ready_queue, 
                          int device_num, string device_type, int array_size, float alpha);

template <class T>
void interrupt_handler(LinkedList<PCB>  *device_array, T &ready_queue, 
                        int device_num, const string &device_type, int array_size, float alpha);

template <class T>
void disk_system_call_handler(disk *disks, T &ready_queue,
                              int device_num, const string &device_type, int array_size, float alpha);

template <class T>
void disk_interrupt_handler(disk *disks, T &ready_queue,
                            int device_num, const string &device_type, int array_size, float alpha);

void verify_input(int &input);

void newLine();//gets rid of any unecessary characters from input

void verify_history_parameter(float &alpha);

void assign_cylinder_count(disk *disks, int disks_count);

template <class T>//return true if found, and killed. False if process is nowhere to be found.
bool kill_process(T &ready_queue, LinkedList<PCB> *printer_array, LinkedList<PCB> *cd_array, disk *disk_array, PCB &pid_number, PCB &result,
                  int printers, int cd_disks, int disks);

PCB process_resolution(string &action);

template <class T>
void verify_number(T& value);

void verify_burst(float& value);

bool find_in_map( map<char, string> the_map, char inp, string &output);//fail look up on this value

/*DISABLED FOR PROJECT 3
struct compare
{
  bool operator()( const PCB& l, const PCB& r) const
  {
    return l.return_tau_current() > r.return_tau_current();
  }
};
*/

/*
////////////
////////////
//////MEM FUNC///
//////////////////
*/
void populate_memory(vector<int> &sys_mem);//initialize all memory to unused (-1).

void populate_f_frames(vector<int> &sys_mem, queue<int> &free_frames); //using the current state of memory, populate the free frames

void alloc_n_frames(vector<int> &sys_mem, queue<int> &free_frames, PCB &process, int n);//Give the process n frames 

void dealloc_frames(vector<int> &sys_mem, queue<int> &free_frames, PCB &process); //Free up the frames used by the process

template <class T>
PCB pop_job_pool(vector<int> &sys_mem, queue<int> &free_frames, T &job_pool);//pops the next process from the job pool and allocs it mem.

template <class T>
void pop_if_free( priority_queue<PCB> &job_pool, queue<int> &free_frames, T &ready_queue, vector<int> &sys_mem);

void print_frame_list(vector<int> sys_mem);

void print_free_frame_list(queue<int> free_frames);

void print_job_list( priority_queue<PCB> job_pool);

bool determine_hex_output(string hex_value, PCB &process, string &hex_output);//determine hexvalue. return false if nonexistant

int convert_bin_to_dec(string decimal_string);

string convert_dec_to_bin(int n);

void print_both_tables(vector<int> sys_mem, queue<int> free_frames);

bool determine_power_of_two(int n);

int return_highest_GCD(int n );//return the highest number that divides n up.

void verify_larger_than_zero(int &input);

void print_list_two( vector<int> sys_mem, queue<int> free_frames);


/*
///////////////////////////
///////////////////////////
//////////INPUT VERIFICATION///
///////////////////////////
*/
void verify_burst(float& value)
{
  verify_number(value);
  while(value < 0)
  {
    cout <<  "Enter a non-negative value" << endl;
    verify_number(value); 
  }
}

template <class T>
void verify_number(T& input)
{
  cin >> input;
  while( ! cin )
  {
    cout << "Please enter a numeric number..." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> input;
  }
}

void verify_input(int &input)
{

  while(input <= 0 || input >= 100){
    verify_number( input );
    cout << endl;
    if(input <= 0 || input >= 100) cout << "Error: Invalid input. Enter a number from 1 to 99" << endl; 
  }

}

void verify_history_parameter(float &alpha)
{
  while(alpha <= 0 || alpha >= 1.00)
  {
    verify_number(alpha);
    cout << endl;
    if( alpha < 0 || alpha > 1.00 ) cout << "Error: Re-enter a history parameter must be within the range [0,1]" << endl;
  }
}

void verify_larger_than_zero(int &input)
{
  verify_number(input);
  while( input <= 0 )
  {
    cout << "Error: enter a number larger than 0" << endl;
    verify_number(input);
  }
}

/*
//////////////////////////////
//////////////////////////////
////////////////////////MAIN//
//////////////////////////////
*/
int main()
{
  BinaryHeap<PCB> ready_queue;

  priority_queue<PCB> job_pool;

  float average_total_cpu_time = 0.00;
  float end_time = 0;
  float global_cpu_time = 0.00;
  float processes_terminated = 0.00;
  PCB previously_terminated;

  queue<int> free_frames; //Memory init

  cout << "Hello! Welcome to SILL-3 OS" << endl;

  /*
   ********************MEMORY INI***********************
   */

  int total_memory = 0;
  int page_size = 0;
  int max_process_size = 0;
  do{
  cout << "Enter the system memory size (in words) ";
  verify_larger_than_zero(total_memory);


  cout << "Enter the page size ";
  verify_larger_than_zero(page_size);

  while( determine_power_of_two( page_size) )
  {
    cout << "Please enter a power of two" << endl;
    verify_larger_than_zero(page_size);
  }

  if((total_memory % page_size != 0)) cout << "Page size does not evenly divide total_memory; re-enter parameters" << endl;
  }while( (total_memory % page_size != 0) );

  cout << "Enter the max process size ";
  verify_larger_than_zero(max_process_size);
  while( max_process_size > total_memory){
      cout << "Max size larger than available memory" << endl;
      cout << "Enter a new value" << endl;
      verify_larger_than_zero(max_process_size);

  }

  int frames = ceil(total_memory / page_size);
  vector<int> sys_mem(frames, -1);

  populate_f_frames(sys_mem, free_frames);//initializes our free frames.

  /*
   ********************SYS GEN INI***********************
   */
  int printers = 0;
  cout << "Enter the number of printers: ";
  verify_input(printers);

  LinkedList<PCB> * printer_array;

  printer_array = new LinkedList<PCB> [printers];

  int disks = 0;
  cout << "Enter the number of disks: ";

  verify_input(disks);

  disk * disk_array;
  disk_array = new disk [disks];

  assign_cylinder_count(disk_array, disks);

  int cd_disks = 0;
  cout << "Enter the number of CD/R drives: ";
  verify_input(cd_disks);

  LinkedList<PCB> * cd_array;
  cd_array = new LinkedList<PCB> [cd_disks];


  /*
   ********************TIME INI***********************
   */

  float alpha = 0.00;
  cout << "Enter the history parameter: ";
  verify_history_parameter(alpha);
  
  float tau_prev = 0.00;
  cout << "Enter the initial tau_prev : ";
  verify_number( tau_prev );

  while( tau_prev < 0.00){

    cout << "Enter a non-negative value" << endl;
    verify_number( tau_prev );

  }

  char terminate = 'n';
  string action;

  int pid_tracker = 1, device_num = 0;
  char char_action;
  char observe_queue;

  PCB vessel, process_pid, result;

  while(terminate != 'x')
  {
    cout << "checking if processes may be allocated from job pool..." << endl;
    pop_if_free( job_pool, free_frames, ready_queue, sys_mem );//Alloc waiting processes if there is sufficient memory

    //Otherwise, we go on with our usual bidness
    device_num = 0;
    cout << "Please...enter an action to take" << endl;
    cout << "Or enter h for some help" << endl;
    cin >> action;
    device_resolution(action, char_action, device_num);

    if( char_action == 'K' ) process_pid = process_resolution( action );

    switch (char_action)
    {
      case 'A':
        generate_new_PCB(ready_queue, pid_tracker, tau_prev, job_pool, sys_mem, free_frames, total_memory, page_size, max_process_size);
        break;
      case 'S':
        //A function that prints the requested queue.
        cout << "You've entered S for Snapshot" << endl;
        cout << "But first, enter the queue_array you wish to observe..." << endl;
        cin >> observe_queue;
        newLine();
        switch(observe_queue)
        {
          case 'r':
              print_heap(ready_queue);
            break;
          case 'p':
              print_queue_array(printer_array, "printer", printers);
              break;
          case 'd':
              print_disk_array(disk_array, "disks", disks);
              break;
          case 'c':
              print_queue_array(cd_array, "CD\\R", cd_disks);
              break;
          case 'm':
              print_frame_list( sys_mem );
              cout << "press any key, then enter, to print the free frames" << endl;
              cin >> action;
              newLine();
              print_free_frame_list( free_frames );
              break;
          case 'j':
              print_job_list( job_pool );
              break;
          default:
              cout << "invalid inp" << endl;
              break;
        }
        break;
      case 't':
        cout << "terminating the first object in the ready_queue..." << endl;

        if(! ready_queue.isEmpty() ){

          previously_terminated = ready_queue.findMin();
          ready_queue.deleteMin();

          cout << "Process " << previously_terminated.get_pid() << " has terminated." << endl;
          cout << "Enter length of last CPU burst: ";

          verify_burst(end_time);

          cout << endl;
          previously_terminated.increment_time(end_time);

          previously_terminated.termination_update();

          average_total_cpu_time += previously_terminated.return_total_time();

          processes_terminated += 1.00;

          global_cpu_time = average_total_cpu_time / processes_terminated;

          dealloc_frames( sys_mem, free_frames, previously_terminated );


          cout << "Average system CPU time: " << global_cpu_time << endl;


        } 
        else cout << "Ready queue is empty!" << endl;

        break;
      case 'p':
        system_call_handler(printer_array, ready_queue, device_num, "printers", printers, alpha);
        break;
      case 'd':
        disk_system_call_handler(disk_array, ready_queue, device_num, "disks", disks, alpha);
        break;
      case 'c':
        system_call_handler(cd_array, ready_queue, device_num, "cd disks", cd_disks, alpha);
        break;
      case 'P':
        interrupt_handler(printer_array, ready_queue, device_num, "printer", printers, alpha);
        break;
      case 'D':
        disk_interrupt_handler(disk_array, ready_queue, device_num, "disk", disks, alpha);
        break;
      case 'C':
        interrupt_handler(cd_array, ready_queue, device_num, "cd disk", cd_disks, alpha);
        break;
      case 'h':
        cout << "The allowed commands, and what they do: " << endl;
        cout << " A for admit, t for terminate, pXX, cXX, dXX, PXX, CXX, DXX for device num XX" << endl;
        cout << " S for snapshot, and r, p, c, d in order to browse the respective queues." << endl;
        cout << " K# to kill a process, wherein you replace # with the process" << endl;
        cout << " q for quit." << endl;
        break;
      case 'K':
        if( kill_process(ready_queue, printer_array, cd_array, disk_array, process_pid, result, printers, cd_disks, disks) )
        {
          dealloc_frames( sys_mem, free_frames, result );

          average_total_cpu_time += result.return_total_time();

          result.termination_update();

          processes_terminated += 1.00;

          global_cpu_time = average_total_cpu_time / processes_terminated;

          cout << "Average system CPU time: " << global_cpu_time << endl;

        }
        else{
          cout << "Process does not exist" << endl;
        }
        break;
      case 'q':
         cout << "Quit?" << endl;
         cout << "y for yes, n for no" << endl;
         char query;
         cin >> query;
         newLine();
         if(query == 'y') terminate = 'x';
         break;
      default:
        cout << "Invalid input. Mayhaps consult h?" << endl;
        break;

    }

  }

  cout << "Shutting down..." << endl;
  delete[] printer_array;
  delete[] disk_array;
  delete[] cd_array;

  return 0;//"Keeping ANSI happy." - Eric Schweitzer, 2015
}

template <class T>
void generate_new_PCB(T &ready_queue, int &pid_tracker, float tau_prev, priority_queue<PCB> &job_pool, vector<int> &sys_mem, 
                      queue<int> &free_frames, int total_mem, int page_size, int max_size)
{

  float proc_mem = 0, frames_required = 0;

  float cpu_burst = 0;

  cout << "How much memory does the new process require" << endl;
  cin >> proc_mem;
  if( proc_mem > max_size )
  {
    cout << "Process exceeds maximum process size; rejected" << endl;
    return;
  }

  bool new_process_alloc = false;

  frames_required = ceil( proc_mem / float(page_size) );

  PCB new_item(pid_tracker, tau_prev, proc_mem, frames_required), vessel;

  if(proc_mem > total_mem)
  {
    cout << "Not enough memory to support the program" << endl;
    cout << "Process has been rejected" << endl;
    new_process_alloc = false;
  }
  else if( frames_required > free_frames.size() )
  {
    cout << "Not enough free frames. Placing process into job_pool" << endl;
    new_item.change_comparison("mem");
    job_pool.push( new_item );
    new_process_alloc = false;
    pid_tracker++; //only updated if a process has requested a suitable amount of memory

  }
  else
  {
    alloc_n_frames(sys_mem, free_frames, new_item, frames_required);
    new_process_alloc = true;
    pid_tracker++; //only updated if a process has requested a suitable amount of memory

  }

  if( ! ready_queue.isEmpty() )//update the process in the cpu atm
  {
    vessel = ready_queue.findMin();
    ready_queue.deleteMin();

    cout << "How much time did process " << vessel.get_pid() << " spend?" << endl;
    verify_burst(cpu_burst);

    vessel.increment_time(cpu_burst);

    ready_queue.insert(vessel);//the item is pushed back into the ready_queue after cpu time is acc. for.
  }

  if( new_process_alloc) ready_queue.insert(new_item);//If the process has been admitted into the queue

  return;
}

/*************************************************
print_heap: pops out the queue an element at a time, and prints each item
*************************************************/
template < class T >
void print_heap( T the_queue )
{
    PCB vessel;

    cout << endl;
    cout << "PID" << "    " << "TotalT." << "    " <<  "AvgT" 
                << "       " << "R.Time" 
                   << "     " << "CPUT" << endl;
    while( ! the_queue.isEmpty() ) //While the queue isn't empty:
    {
      vessel = the_queue.findMin();
      the_queue.deleteMin();
      vessel.print_time_stats();

    }
    cout << endl;
}

void print_queue(LinkedList<PCB> &the_queue)
{
  cout << "queue contents" << endl;
      cout << "PID" << "    " << "TotalT." << "    " <<  "AvgT" 
                << "       " << "R.Time" 
                   << "     " << "CPUT." << endl;
  the_queue.print_nodes();
}

void print_disk_array(disk *disks, string device, int num)
{
  for(int i = 0; i < num; i++){
    cout << "hello!" << endl;
  disks[i].print_queues();
  } 
}

void device_resolution(string &input, char &case_switch, int &device_num)
{

  if(input.length() == 1)
  {
    case_switch = input[0];
    newLine();
    return;
  }

  stringstream character_stream, number_stream;

  character_stream << input[0];

  character_stream >> case_switch;

  number_stream << input.substr(1);

  number_stream >> device_num;

}

PCB process_resolution( string &action )
{
  PCB process_pid;

  if(action.length() == 1)
  {
    process_pid.set_pid( -1 );
    return process_pid;
  }

  stringstream number_stream;

  number_stream << action.substr(1);

  int value = 0; number_stream >> value;

  process_pid.set_pid( value );

  return process_pid;
}

void admit_into_queue(LinkedList<PCB> &queue, PCB &new_admission)
{
  queue.enQueue(new_admission);

}

template <class T>
void system_call_handler(LinkedList<PCB> *device_array, T &ready_queue, int device_num,
                         string device_type, int array_size, float alpha)
{
        string file_name, location;
        int file_size;
        char file_op;
        PCB vessel;

        float cpu_burst_time = 0.00;

        file file_parameters;
        if(device_num > array_size)
        {
          cout << "error: " << device_type << " number exceeded." << endl;
          return;
        }
        else if (device_num == 0){
          cout << "error: " << device_type << " has no value " << device_num << " amount." << endl;
          return; 
        }
        else if( ready_queue.isEmpty() )
        {
          cout << "No processes in ready queue" << endl;
          return;
        }

        vessel = ready_queue.findMin();
        ready_queue.deleteMin();

        cout << "Sending process "  << vessel.get_pid() << " to a " << device_type << " queue" << endl;
        cout << "Enter the file name: ";
        while( file_name.length() == 0 || file_name.length() > 16)
        {
          cin >> file_name;
          if(file_name.length() > 16)
          {
            cout << "Please reenter your file; input too long." << endl;
          } 
          else if(file_name.length() == 0) cout << "No input detected. Try again." << endl;
        }
        cout << endl;

        cout << "Enter the logical address: ";

        cin >> location;

        string output;

        while( ! determine_hex_output(location, vessel, output) )
        {
          cout << "Please enter valid input" << endl;
          cin >> location;
        }

        cout << "Physical address: " << output << endl;

        cout << endl;

        cout << "Enter the file operation: ";
        if(device_type == "printers"){
          cout << "Printer op; setting operation to (w)rite." << endl;
          file_op = 'w';
        }
        else{
          cin >> file_op;
          newLine();
          cout << endl;
          while(file_op != 'r' && file_op != 'w')
          {
            cout << "Please enter r for read or w for write: " << endl;
            cin >> file_op;
            newLine();
          }
        }

        if( file_op == 'w' )
        {
          cout << "Enter the file size: ";
          cin >> file_size;
          cout << endl;
        }
        else file_size = 0;//Figure out how to generalize the no file write case.


        cout << "How much time did process " << vessel.get_pid() << " spend in the CPU?" << endl;
        verify_burst(cpu_burst_time);

        cout << "parameters are: " << file_name << " " << file_size << " " << output << " " << file_op
             << " cpu_burst_time: " << cpu_burst_time << endl;

        file_parameters.change_parameters(file_name, file_op, file_size, output);

        vessel.file_parameters(file_parameters);

        vessel.calc_next_burst(alpha, cpu_burst_time);

        cout << "going into device number: " << device_num << endl;

        admit_into_queue(device_array[device_num - 1], vessel);
}

void print_queue_array( LinkedList<PCB> *queue_array, const string &device_name, int array_size)
{
  char again;
  for(int i = 0; i < array_size; i++)
  {
    cout << "PID" << "    " << "FName" << "      " 
         <<  "FLoc"<< "       " << "FSiz" 
         << "       " << "r/w" << "        " << "NxBrst" <<  endl;
    queue_array[i].print_nodes();
    cout << endl;

    if(i < array_size - 1){


    cout << "would you like to see " << device_name << " " << i+2 << "? Hit y for yes, n for no" << endl;
    cin >> again;
    newLine();
    }
    if(again == 'n') continue;                
  }  
  cout << endl;
}

template <class T>
void interrupt_handler(LinkedList<PCB> *device_array, T &ready_queue, 
                       int device_num, const string &device_type, int array_size, float alpha)
{

  if(device_num > array_size || device_num == 0)
  {
    cout << "error: invalid input. Check your device number." << endl;
    return;
  }
  else if( ! device_array[device_num - 1].IsEmpty() )
  {
    PCB vessel, from_queue;

    vessel = device_array[device_num - 1].peek();
    device_array[device_num - 1].deQueue();


    float cpu_burst_time = 0.00;

    if(  ! ready_queue.isEmpty() )
    {
      from_queue = ready_queue.findMin();
      ready_queue.deleteMin();

      cout << "How much time did process: " << from_queue.get_pid() << " spend in the CPU?" << endl;
      verify_burst(cpu_burst_time);

      from_queue.increment_time(cpu_burst_time);

      ready_queue.insert(from_queue); //place the first process back into the ready queue

    }

    vessel.calc_next_burst(alpha, cpu_burst_time);

    ready_queue.insert( vessel );

  }
  else
  {
    cout << device_type << " " << device_num << " is empty." << endl;
  }

}

void newLine()
//newLine makes sure any unecessary characters are not read.
//EX: if the program asks for n, and the user enters "name name name"
//It will only read n and discard the rest.
{
    char symbol;

    do
    {
        cin.get(symbol);
    } while( symbol != '\n');
}

template <class T>
void disk_interrupt_handler(disk *disks, T &ready_queue, 
                       int device_num, const string &device_type, int array_size, float alpha)
{
  if(device_num > array_size || device_num == 0)
  {
    cout << "error: invalid input. Check your device number." << endl;
    return;
  }
    PCB vessel, from_queue;

    if( disks[device_num-1].return_next(vessel) ) //if there is something to process.
    {
      float cpu_burst_time = 0.00;
      if(  ! ready_queue.isEmpty() )
      {
        from_queue = ready_queue.findMin();
        ready_queue.deleteMin();

        cout << "How much time did process: " << from_queue.get_pid() << " spend in the CPU?" << endl;
        verify_burst(cpu_burst_time);

        from_queue.increment_time(cpu_burst_time);

        ready_queue.insert(from_queue); //place the first process back into the ready queue

      }

      vessel.change_comparison("time");

      ready_queue.insert( vessel );

    }
    else
    {
      cout << device_type << " " << device_num << " is empty." << endl;
    }
}

template <class T>
void disk_system_call_handler(disk *disks, T &ready_queue, int device_num,
                              const string &device_type, int array_size, float alpha)
{
        string file_name, location;
        int file_size, cylinder;
        char file_op;
        PCB vessel;

        float cpu_burst_time = 0.00;

        file file_parameters;
        if(device_num > array_size)
        {
          cout << "error: " << device_type << " number exceeded." << endl;
          return;
        }
        else if (device_num == 0){
          cout << "error: " << device_type << " has no value " << device_num << " amount." << endl;
          return; 
        }
        else if( ready_queue.isEmpty() )
        {
          cout << "No processes in ready queue" << endl;
          return;
        }

        vessel = ready_queue.findMin();
        ready_queue.deleteMin();

        cout << "Sending process " << vessel.get_pid() << " to a " << device_type << " queue" << endl;
        cout << "Enter the file name: ";
        while( file_name.length() == 0 || file_name.length() > 16)
        {
          cin >> file_name;
          if(file_name.length() > 16)
          {
            cout << "Please reenter your file; input too long." << endl;
          } 
          else if(file_name.length() == 0) cout << "No input detected. Try again." << endl;
        }
        cout << endl;

        cout << "Entering into disk " << device_num << endl;

        cout << endl;

        cout << "Enter the logical address: ";

        cin >> location;

        string output;

        while( !determine_hex_output(location, vessel, output) )
        {
          cout << "Please enter valid input" << endl;
          cin >> location;
        }

        cout << "Physical address: " << output << endl;


        cout << endl;


        cout << "Enter the cylinder location" << endl;
        verify_number(cylinder);

        while( cylinder > disks[device_num-1].return_cylinders() )
        {
          cout << "Invalid location; enter a number from 0 to " << (disks[device_num-1]).return_cylinders() << endl;
          verify_number( cylinder );
        }


        vessel.set_mem(cylinder);
        
        cout << "You have entered: " << vessel.return_mem() << endl;

        cout << "Enter r for read, w for write: ";
        cin >> file_op;
        newLine();
        cout << endl;
        while(file_op != 'r' && file_op != 'w')
        {
          cout << "Please enter r for read or w for write: " << endl;
          cin >> file_op;
          newLine();
        }
        
        if( file_op == 'w' )
        {
          cout << "Enter the file size: ";
          verify_number( file_size );
          cout << endl;
        }
        else file_size = 0;//Figure out how to generalize the no file write case.

        cout << "How much time did process " << vessel.get_pid() << " spend in the CPU?" << endl;
        verify_burst(cpu_burst_time);


        cout << "parameters are: " << file_name << " " << file_size << " " << output << " " << file_op
             << " cpu_burst_time: " << cpu_burst_time << endl;

        file_parameters.change_parameters(file_name, file_op, file_size, output);

        vessel.file_parameters(file_parameters);

        vessel.calc_next_burst(alpha, cpu_burst_time);

        cout << "going into device number: " << device_num << endl;

        vessel.change_comparison("disk");// Essential!

        disks[device_num-1].admit_PCB(vessel);
}

void assign_cylinder_count(disk *disks, int disks_count)
{
    int cylinders = 0;
    for(int i = 0; i < disks_count; i++)
    {
      cout << "enter the amount of cylinders for disk: " << i+1 << endl;
      verify_number(cylinders);

      while(cylinders <= 0)
      {
        cout << "Please enter a non-zero, non-negative cylinder count" << endl;
        verify_number(cylinders);

      }
      disks[i].set_cylinders(cylinders);
    }
}

/*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxKILL PROCESS FUNCTION
xxxxxxxxxxxxxxxxxxxxxxxxxxxxKILL PROCESS FUNCTION
xxxxxxxxxxxxxxxxxxxxxxxxxxxxKILL PROCESS FUNCTION
xxxxxxxxxxxxxxxxxxxxxxxxxxxxKILL PROCESS FUNCTION
xxxxxxxxxxxxxxxxxxxxxxxxxxxxKILL PROCESS FUNCTION
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*/
template <class T>
bool kill_process(T &ready_queue, LinkedList<PCB> *printer_array, LinkedList<PCB> *cd_array, disk *disk_array, PCB &pid_number, PCB &result,
                  int printers, int cd_disks, int disks)
{
  //Go through each queue; ask each queue if the process has been found.
  cout << "Looking for process " << pid_number.get_pid() << endl;

  if( !ready_queue.isEmpty() )
  {
    if( ready_queue.find( pid_number, result ) ) return true;
  }
  

    for(int i = 0; i < printers; i++)
    {
      if( ! printer_array[i].IsEmpty() )
        if( printer_array[i].find( pid_number, result ) ) return true;
    }

    for(int i = 0; i < cd_disks; i++)
    {
      if( !cd_array[i].IsEmpty() )
        if(cd_array[i].find( pid_number, result ) ) return true;
    }

    for(int i = 0; i < disks; i++)
    {
      if(!disk_array[i].empty() )
        if( disk_array[ i ].kill_process(pid_number, result) ) return true;
    }

  return false; //If we've gone through all the queues, and have not found the process, we have failed...
}

void populate_memory(vector<int> &sys_mem)
{
  int size = sys_mem.size();

  for(int i = 0; i < size; i++)
  {
    sys_mem[i] = -1;
  }

}

void populate_f_frames(vector<int> &sys_mem, queue<int> &free_frames)
{
  int size = sys_mem.size();

  for( int i = 0; i < size; i++)
  {
    if(sys_mem[i] == -1) free_frames.push(i);
  }
}

void alloc_n_frames(vector<int> &sys_mem, queue<int> &free_frames, PCB &process, int n)//Give the process n_frames frames (kek)
{
  LinkedList<int> frames;
  int curr_frame = 0, process_pid = process.get_pid();

  for(int i = 0; i < n; i ++)
  {
    curr_frame = free_frames.front();//A frame is no longer free

    sys_mem[curr_frame] = process_pid;//the sys mem is update to reflect this

    frames.enQueue( curr_frame );//used to store in the page table where the frame physically is.

    free_frames.pop();//free frames is updated.

  }

  process.alloc_frame_list( frames );//the page table entries in the process will be updated.
}

void dealloc_frames(vector<int> &sys_mem, queue<int> &free_frames, PCB &process) //Free up the frames used by the process
{
  vector<int> alloc_frames;

  alloc_frames = process.get_page_table(); //the frames used by the process are returned.

  int curr_frame = 0, size = alloc_frames.size();

  while( ! alloc_frames.empty() )
  {
    curr_frame = alloc_frames.back();

    alloc_frames.pop_back();

    sys_mem[ curr_frame ] = -1;//Signifies that the frame has been reclaimed.
    free_frames.push( curr_frame );//the reclaimed frame is now pushed back into the free frame list.
  }

}

template <class T>
PCB pop_job_pool(vector<int> &sys_mem, queue<int> &free_frames, T &job_pool)//pops the next process from the job pool and allocs it mem.
{
  PCB next = job_pool.top();
  job_pool.pop();

  alloc_n_frames( sys_mem, free_frames, next, next.get_mem_size() );

  return next;
}

template <class T>
void pop_if_free( priority_queue<PCB> &job_pool, queue<int> &free_frames, T &ready_queue, vector<int> &sys_mem)
//job_pool, free_frames, ready_queue, sys_mem
{
  PCB job_pool_front, vessel, next;

  float cpu_burst;

  if( ! job_pool.empty() ) job_pool_front = job_pool.top();//get the first item from the job pool, if not empty

  while( (! job_pool.empty()) && (job_pool_front.return_frames() < free_frames.size()) )//keep giving processes memory until we cannot anymore
    //conditionally dep. on whether there are jobs pop, or enough memory`
  {
    cout << "Enough space for a new process" << endl;

    next = pop_job_pool( sys_mem, free_frames, job_pool );//Allocs job_pool_front, since return_mem > free_frames.size

    //credit process at the top of the ready_queue for time.
    if( ! ready_queue.isEmpty() )//update the process in the cpu atm
    {
      vessel = ready_queue.findMin();
      ready_queue.deleteMin();

      cout << "How much time did process " << vessel.get_pid() << " spend?" << endl;
      verify_burst(cpu_burst);

      vessel.increment_time(cpu_burst);

      vessel.change_comparison("time");
      ready_queue.insert(vessel);//the item is pushed back into the ready_queue after cpu time is acc. for.
    }

    ready_queue.insert(next);

    if( ! job_pool.empty() )job_pool_front = job_pool.top();

  }

}


void print_both_tables(vector<int> sys_mem, queue<int> free_frames)
{
  cout << "Frame table" << "-------" << "Free frame table" << endl; 
  cout << "Frame" << " " << "PID" << "---------" << "Frame" << endl;
  for(int i = 0; i < sys_mem.size(); i++)
  {
    if( sys_mem[i] != -1) cout << left << setw(6) << i << left << setw(12) << sys_mem[i];
    else cout << "                  ";

    if(! free_frames.empty() ){
        cout << left << setw(13) << free_frames.front();
        cout << endl;
        free_frames.pop();
    }
  }

}

void print_job_list( priority_queue<PCB> job_pool)
{
  while( ! job_pool.empty() )
  {
    cout << job_pool.top() << endl;
    job_pool.pop();
  }
}

bool determine_hex_output(string hex_value, PCB &process, string &hex_output)//determine hexvalue. return false if nonexistant
{
  map<char, string> hex_to_binary = {{'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}
                                        ,{'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}
                                        ,{'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};

  string converted_to_bin, look_up;
  char val;
  for( int i = 0; i < hex_value.size(); i++)//Convert to Binary.
  {
    //Say FF14
    if( converted_to_bin.size() == 0 ) {
      val = hex_value[i];
      if( find_in_map(hex_to_binary, val, look_up) ){
        converted_to_bin = look_up;

      }
      else{
        cout << "Lookup failed; invalid input" << endl;
        return false;
      }
    }
    else{ 
    val = hex_value[i];
    if ( find_in_map(hex_to_binary, val, look_up) ){
      converted_to_bin = converted_to_bin + look_up;

    }
    else{
      cout << "Look up failed; invalid input" << endl;
      return false;
    }
    }
    //Output should be 1111111100010100
  }
  cout << "converted to: " << converted_to_bin << endl;

  string page_entry, frame_offset;
  int frame_begin = 0;//position where the offset begins. It is where the bottom loop ends

  string page_n;

  int page_table_size = process.return_frames();
  page_n = convert_dec_to_bin( page_table_size );

  cout << "we need this many bits: " << page_n << endl;
  page_table_size = page_n.size();
  if( converted_to_bin.size() < page_n.size() ){
    cout << "error: not enough bits to use; input too small" << endl;
    return false;
  }

  for(int i = 0; i < page_table_size; i++)
  {
    //for 1111111100010100 and pg size: 8
    if( page_entry.size() == 0 ){ 
      frame_begin++;
      page_entry = converted_to_bin[i];
    }
    else{
      frame_begin++;
      page_entry = page_entry + converted_to_bin[i];
    }

    // page_entry should be: 11111111, frame off should be: 00010100
  }
  frame_offset = converted_to_bin.substr(frame_begin);

  cout << "page entry: " << page_entry << " frame offset: " << frame_offset << endl;

  int dec_page_val = convert_bin_to_dec( page_entry ), dec_frame_off = convert_bin_to_dec( frame_offset);

  cout << "Looking up in page entry: " << dec_page_val << endl;
  if( dec_page_val > process.get_mem_size() ){
   cout << "Error: requested a logical address larger than the page table size" << endl;
   return false; //if the page value is larger than the amt of frames, fail.
  }
  int physical_page = process.fetch_physical_frame(dec_page_val); //return the corresponding value to the logical input.

  stringstream hex_phys;

  hex_phys << std::hex << physical_page;

  string physical_page_res( hex_phys.str() );

  stringstream hex_offset;

  hex_offset << std::hex << dec_frame_off;

  string frame_offset_hex( hex_offset.str() );

  string final_result = physical_page_res + frame_offset_hex;

  hex_output = final_result;

  cout << "finished!" << endl;
  return true;
}

int convert_bin_to_dec(string decimal_string)
{
  int decimal = 0;
  stringstream toint;

  toint << decimal_string;

  toint >> decimal;

  int pos = 0, right_most = 0, converted_value = 0;

  while(decimal != 0)//Loop will continue so long as there are values to be obtained.
  {
    right_most = decimal % 10;//obtain the rightmost bit

    decimal = decimal / 10;//remove the rightmost bit.

    converted_value += right_most * int( pow(2, pos) );//Mutiply by the respective power of two

    pos++; //Increment for the next power of two.
  }

  return converted_value;
}

bool find_in_map( map<char, string> the_map, char inp, string &output)
{
  map<char,string>::iterator the_it = the_map.find(inp);

  if( the_it != the_map.end() )
  {
    output = the_it->second;
    return true;
  }

  else return false;
}

bool determine_power_of_two(int n)
{
  if( n == 0 ) return false;
  else{
    while( (n % 2 == 0))
    {
      n = n / 2;

    }
    if( n == 1 ) return false;
    else return true;
  }
}

int return_highest_GCD(int n)
{
  int current = 1;
  for(int i = 1; i <= 32; i++)
  {
    if( (n % i == 0)  ) current = i;
  }
  return current;
}

void print_frame_list( vector<int> sys_mem )
{
  int size = sys_mem.size();
  int rows = return_highest_GCD( size );
  int columns = size / rows;
  int temp;
  if( columns > 32 )
  {
    temp = columns;
    columns = rows;
    rows = columns;
  }

  cout << "divided up " << size << " into " << rows << " rows, and " << columns << " columns" << endl;


  int external_counter = 0;
  for( int i = 0; i < columns; i++)
  {
    cout << "Fr" << "  " << "PID" << " |";
  }
  cout << endl;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      cout << left << setw(4) << external_counter << left << setw(4) << sys_mem[external_counter] << "|";
      external_counter++;
    }
    cout << endl;
  }

}

void print_free_frame_list( queue<int> free_frames )
{
  int size = free_frames.size();
  int rows = return_highest_GCD( size );
  int columns = size / rows;
  int temp;
  if( columns > 32 )
  {
    temp = columns;
    columns = rows;
    rows = columns;
  }

  for(int i = 0; i < columns; i++)
  {
    cout << "Fr" << "  |";
  }
  cout << endl;

  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      cout << left << setw(4) << free_frames.front() << "|";
      free_frames.pop();
    }
    cout << endl;
  }
}

string convert_dec_to_bin(int n)
{
  string reverse_s;
  int cur_pow = 2;
  while( n != 0 )
  {
    if((n % 2 == 0)){
      if(reverse_s.size() == 0) reverse_s = "0";
      else reverse_s = reverse_s + "0";
    }
    else{
      if(reverse_s.size() == 0) reverse_s = "1";
      else reverse_s = reverse_s + "1";
    }
    n /= 2;
  }

  stack<char> reverse;
  for(int i = 0; i < reverse_s.size(); i++)
  {
    reverse.push(reverse_s[i]);
  }
  string result;
  while( ! reverse.empty() )
  {
    if(result.size() == 0) result = reverse.top();
    else result = result + reverse.top();
    reverse.pop();
  }
  return result;
}

