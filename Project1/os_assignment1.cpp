#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "queue_hybrid.cpp"
#include "PCB.h"

using namespace std;

void generate_new_PCB(LinkedList<PCB> &ready_queue, int& pid_tracker);

void print_queue(const LinkedList<PCB> &the_queue);

void print_queue_array(const LinkedList<PCB> *queue_array, const string &device_name, int array_size);

void admit_into_queue(LinkedList<PCB> &queue, PCB &new_admission);

void device_resolution(string &input, char &case_switch, int &device_num);

void system_call_handler(LinkedList<PCB> *device_array, LinkedList<PCB> &ready_queue, int device_num, string device_type, int array_size);

void interrupt_handler(LinkedList<PCB>  *device_array, LinkedList<PCB> &ready_queue, int device_num, const string &device_type, int array_size);

void verify_input(int &input);

void newLine();//gets rid of any unecessary characters from input


void verify_input(int &input)
{

  while(input <= 0 || input >= 100){
    cin >> input;
    cout << endl;
    if(input <= 0 || input >= 100) cout << "Error: Invalid input. Enter a number from 1 to 99" << endl; 
  }
}

int main()
{
  LinkedList<PCB> ready_queue;

  int printers = 0;
  cout << "Hello! Welcome to SILL-E OS" << endl;
  cout << "Enter the number of printers: ";
  verify_input(printers);

  LinkedList<PCB> * printer_array;

  printer_array = new LinkedList<PCB> [printers];

  int disks = 0;
  cout << "Enter the number of disks: ";

  verify_input(disks);

  LinkedList<PCB> * disk_array;
  disk_array = new LinkedList<PCB> [disks];

  int cd_disks = 0;
  cout << "Enter the number of CD/R drives: ";
  verify_input(cd_disks);

  LinkedList<PCB> * cd_array;
  cd_array = new LinkedList<PCB> [cd_disks];

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
        generate_new_PCB(ready_queue, pid_tracker);
        break;
      case 'S':
        //A function that prints the requested queue.
        cout << "Hey hey hey, snapshot timeeeeeee" << endl;
        cout << "But first, enter the queue_array you wish to observe..." << endl;
        cin >> observe_queue;
        switch(observe_queue)
        {
          case 'r':
              print_queue(ready_queue);
            break;
          case 'p':
              print_queue_array(printer_array, "printer", printers);
              break;
          case 'd':
              print_queue_array(disk_array, "disks", disks);
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

        if(! ready_queue.IsEmpty()) ready_queue.deQueue();
        else cout << "Ready queue is empty!" << endl;

        break;
      case 'p':
        system_call_handler(printer_array, ready_queue, device_num, "printers", printers);
        break;
      case 'd':
        system_call_handler(disk_array, ready_queue, device_num, "disks", disks);
        break;
      case 'c':
        system_call_handler(cd_array, ready_queue, device_num, "cd disks", cd_disks);
        break;
      case 'P':
        interrupt_handler(printer_array, ready_queue, device_num, "printer", printers);
        break;
      case 'D':
        interrupt_handler(disk_array, ready_queue, device_num, "disk", disks);
        break;
      case 'C':
        interrupt_handler(cd_array, ready_queue, device_num, "cd disk", cd_disks);
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

void generate_new_PCB(LinkedList<PCB> &ready_queue, int &pid_tracker)
{

  PCB new_item(pid_tracker, false);

  ready_queue.enQueue(new_item);

  pid_tracker++;
}

void print_queue(const LinkedList<PCB> &the_queue)
{
  cout << "Ready Queue contents" << endl;
  cout << "~PID" << "            " << "F. Name" << "         " <<  "F. Loc"<< "           " << "F. Size" 
                 << "          " << "r/w" << endl;
  cout << the_queue;
}

void device_resolution(string &input, char &case_switch, int &device_num)
{
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

void system_call_handler(LinkedList<PCB> *device_array, LinkedList<PCB> &ready_queue, int device_num,
                         string device_type, int array_size)
{
        string file_name;
        int file_size, location;
        char file_op;
        PCB vessel;

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
        else if(ready_queue.IsEmpty())
        {
          cout << "No processes in ready queue" << endl;
          return;
        }

        vessel = ready_queue.peek();
        ready_queue.deQueue();
        cout << "Sending the first object to a printer queue" << endl;
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

        cout << "Enter the file size: ";
        cin >> file_size;
        cout << endl;

        cout << "Enter the file location: ";
        cin >> location;
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

        cout << "parameters are: " << file_name << " " << file_size << " " << location << " " << file_op << endl;

        file_parameters.change_parameters(file_name, file_op, file_size, location);

        vessel.file_parameters(file_parameters);

        cout << "going into device number: " << device_num << endl;
        admit_into_queue(device_array[device_num - 1], vessel);
}

void print_queue_array(const LinkedList<PCB> *queue_array, const string &device_name, int array_size)
{
  char again;
  for(int i = 0; i < array_size; i++)
  {
    cout << device_name << i+1 << endl;
    cout << "~PID" << "            " << "F. Name" << "         "
                 <<  "F. Loc" <<  "         " << "F. Size" 
                 << "         " << "r/w"  << endl;
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

void interrupt_handler(LinkedList<PCB> *device_array, LinkedList<PCB> &ready_queue, 
                       int device_num, const string &device_type, int array_size)
{
  if(device_num > array_size || device_num == 0)
  {
    cout << "error: invalid input. Check your device number." << endl;
    return;
  }
  else if( ! device_array[device_num - 1].IsEmpty() )
  {
    PCB vessel;

    vessel = device_array[device_num - 1].peek();
    device_array[device_num - 1].deQueue();

    ready_queue.enQueue(vessel);

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
