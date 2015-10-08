#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <queue>
#include <limits>
#include "queue_hybrid_v_2.cpp"
#include "PCB.h"
#include "disk.h"

using namespace std;

template <class T>
void generate_new_PCB(T &ready_queue, int& pid_tracker, float tau_prev);

template <class T>
void print_heap( T the_queue );

void print_queue(const LinkedList<PCB> &the_queue);

void print_queue_array(const LinkedList<PCB> *queue_array, const string &device_name, int array_size);

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

template <class T>
void verify_number(T& value);

void verify_burst(float& value);

struct compare
{
  bool operator()( const PCB& l, const PCB& r) const
  {
    return l.return_tau_current() > r.return_tau_current();
  }
};

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

int main()
{
  priority_queue< PCB, vector<PCB>, compare > ready_queue;

  float average_total_cpu_time = 0.00;
  float end_time = 0;
  float global_cpu_time = 0.00;
  float processes_terminated = 0.00;
  PCB previously_terminated;

  int printers = 0;
  cout << "Hello! Welcome to SILL-E OS" << endl;
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

  PCB vessel;

  while(terminate != 'x')
  {
    device_num = 0;
    cout << "Please...enter an action to take" << endl;
    cout << "Or enter h for some help" << endl;
    cin >> action;
    device_resolution(action, char_action, device_num);
    switch (char_action)
    {
      case 'A':
        generate_new_PCB(ready_queue, pid_tracker, tau_prev);
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
          default:
              cout << "invalid inp" << endl;
              break;
        }
        break;
      case 't':
        cout << "terminating the first object in the ready_queue..." << endl;

        if(! ready_queue.empty() ){

          previously_terminated = ready_queue.top();
          ready_queue.pop();

          cout << "Process " << previously_terminated.get_pid() << " has terminated." << endl;
          cout << "Enter length of last CPU burst: ";

          verify_burst(end_time);

          cout << endl;
          previously_terminated.increment_time(end_time);

          previously_terminated.termination_update();

          average_total_cpu_time += previously_terminated.return_total_time();

          processes_terminated += 1.00;

          global_cpu_time = average_total_cpu_time / processes_terminated;

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
        cout << " q for quit." << endl;
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
void generate_new_PCB(T &ready_queue, int &pid_tracker, float tau_prev)
{

  PCB new_item(pid_tracker, tau_prev);

  float cpu_burst;
  PCB vessel;

  if( ! ready_queue.empty() )//update the process in the cpu atm
  {
    vessel = ready_queue.top();
    ready_queue.pop();

    cout << "How much time did process " << vessel.get_pid() << " spend?" << endl;
    verify_burst(cpu_burst);

    vessel.increment_time(cpu_burst);

    ready_queue.push(vessel);//the item is pushed back into the ready_queue after cpu time is acc. for.
  }

  ready_queue.push(new_item);

  pid_tracker++;
}

/*************************************************
print_heap: pops out the queue an element at a time, and prints each item
*************************************************/
template < class T >
void print_heap( T the_queue )
{
    PCB vessel;

    cout << endl;
      cout << "PID" << "        " << "TotalT." << "    " <<  "AvgT" 
                << "       " << "R.Time" 
                   << "     " << "CPUT." << endl;
    while( ! the_queue.empty() ) //While the queue isn't empty:
    {
      vessel = the_queue.top();
      the_queue.pop();
      vessel.print_time_stats();

    }
    cout << endl;
}

void print_queue(const LinkedList<PCB> &the_queue)
{
  cout << "queue contents" << endl;
      cout << "PID" << "        " << "TotalT." << "    " <<  "AvgT" 
                << "       " << "R.Time" 
                   << "     " << "CPUT." << endl;
  cout << the_queue;
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

void admit_into_queue(LinkedList<PCB> &queue, PCB &new_admission)
{
  queue.enQueue(new_admission);

}

template <class T>
void system_call_handler(LinkedList<PCB> *device_array, T &ready_queue, int device_num,
                         string device_type, int array_size, float alpha)
{
        string file_name;
        int file_size, location;
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
        else if( ready_queue.empty() )
        {
          cout << "No processes in ready queue" << endl;
          return;
        }

        vessel = ready_queue.top();
        ready_queue.pop();

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

        cout << "Enter the file location: ";

        verify_number(location);

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

        cout << "parameters are: " << file_name << " " << file_size << " " << location << " " << file_op
             << " cpu_burst_time: " << cpu_burst_time << endl;

        file_parameters.change_parameters(file_name, file_op, file_size, location);

        vessel.file_parameters(file_parameters);

        vessel.calc_next_burst(alpha, cpu_burst_time);

        cout << "going into device number: " << device_num << endl;

        admit_into_queue(device_array[device_num - 1], vessel);
}

void print_queue_array(const LinkedList<PCB> *queue_array, const string &device_name, int array_size)
{
  char again;
  for(int i = 0; i < array_size; i++)
  {
    cout << "PID" << "        " << "FName" << "      " <<  "FLoc"<< "       " << "FSiz" 
         << "       " << "r/w" << "        " << "NxBrst" <<  endl;
    cout << queue_array[i];
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

    if(  ! ready_queue.empty() )
    {
      from_queue = ready_queue.top();
      ready_queue.pop();

      cout << "How much time did process: " << from_queue.get_pid() << " spend in the CPU?" << endl;
      verify_burst(cpu_burst_time);

      from_queue.increment_time(cpu_burst_time);

      ready_queue.push(from_queue); //place the first process back into the ready queue

    }

    vessel.calc_next_burst(alpha, cpu_burst_time);

    ready_queue.push( vessel );

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
      if(  ! ready_queue.empty() )
      {
        from_queue = ready_queue.top();
        ready_queue.pop();

        cout << "How much time did process: " << from_queue.get_pid() << " spend in the CPU?" << endl;
        verify_burst(cpu_burst_time);

        from_queue.increment_time(cpu_burst_time);

        ready_queue.push(from_queue); //place the first process back into the ready queue

      }

      vessel.change_comparison();

      ready_queue.push( vessel );

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
        string file_name;
        int file_size, location, cylinder;
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
        else if( ready_queue.empty() )
        {
          cout << "No processes in ready queue" << endl;
          return;
        }

        vessel = ready_queue.top();
        ready_queue.pop();

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
        cout << "Enter the file location: ";
        verify_number(location);

        cout << endl;


        cout << "Enter the cylinder location" << endl;
        verify_number(cylinder);

        while( cylinder > disks[device_num-1].return_cylinders() || location <= 0)
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


        cout << "parameters are: " << file_name << " " << file_size << " " << location << " " << file_op
             << " cpu_burst_time: " << cpu_burst_time << endl;

        file_parameters.change_parameters(file_name, file_op, file_size, location);

        vessel.file_parameters(file_parameters);

        vessel.calc_next_burst(alpha, cpu_burst_time);

        cout << "going into device number: " << device_num << endl;

        vessel.change_comparison();// Essential!

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
