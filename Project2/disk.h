#ifndef DISK_H
#define DISK_H
#include "queue_hybrid.h"
#include "PCB.h"
#include <iostream>

using namespace std;


//ALL SORTS ARE MADE BASED ON DISK DISTANCE COMPARISONS.
//MAKE SURE THE CORRECT FLAG IS ON WHEN SENDING A PCB INTO THE DISK.

class disk
{
	private:

		bool current_direction;
		int cylinders; //Amount of cylinders the disk has.
		/* 
		  Each disk has two queues: one for servicing, the other for accepting requests.
		  The disk will service a queue until it is empty. At which the queue will switch between them, servicing the other
		*/
		LinkedList<PCB> queues[2]; //A queue of PCB's. Each disk has its own list of queues waiting to be serviced.
		int current_head_position; //The head will usually be pointing to this position on the disks
		int request_queue; //Queue 0  if 0, 1 if 1.
		void change_queue(); //Switches queues betweeen being the processed and request queue.
		void direction(); //0 means going up (left to right), 1 means going down (right to left)

		void print_queue_left(LinkedList<PCB> frozen, LinkedList<PCB> request);

		void print_queue_right(LinkedList<PCB> frozen, LinkedList<PCB> request);
	public:
		disk();

		disk(disk &inp);

		//Returns the next PCB from the frozen queue. if Both queues are empty, failure is represented by returning 0.
		bool return_next( PCB &vessel);
		//Will grant admission into the current request
		void admit_PCB(PCB& input);
		//inp number of cylinders
		void set_cylinders(int inp);
		//
		int return_cylinders();

		void print_queues();


};

disk::disk()
{
	current_direction = false;
	cylinders = 0;
	current_head_position = 0;
	request_queue = 0;
}

disk::disk( disk &inp )
{
	current_direction = inp.current_direction;
	cylinders = inp.cylinders;
	current_head_position = inp.current_head_position;
	request_queue = inp.request_queue;

	queues[0] = inp.queues[0];
	queues[1] = inp.queues[1];
}

void disk::change_queue()
{
	if( request_queue == 1) request_queue = 0;
	else request_queue = 1;
}

void disk::direction()
{
	current_direction = ! current_direction;
}

bool disk::return_next(PCB &vessel)
{
	if( queues[0].IsEmpty() && queues[1].IsEmpty() )///Nothing to process. Moving along.
	{
		return false;
	}

	int frozen_queue = 0;

	if(request_queue == 1) frozen_queue = 0;
	else frozen_queue = 1;

	if(current_direction == false)//left to right
	{
		//cout << "dequeuing from queue " << request_queue << endl;
		//cout << "going left to right" << endl;
		PCB new_PCB;
		//cout << "Attempting to dequeue from the left" << endl;
		new_PCB = queues[frozen_queue].peek();
		//cout << "successfully dequeued." << endl;
		vessel = new_PCB;

		queues[frozen_queue].deQueue();

		//current_head_position is assigned
		if( queues[frozen_queue].IsEmpty() ){
			change_queue();
			direction();
		} 

		return true;
	}
	else if( current_direction == true )//right to left.
	{
		//cout << "going right to left" << endl;
		PCB new_PCB;
		//cout << "Attempting to peek from the back" << endl;
		new_PCB = queues[frozen_queue].peekB();

		vessel = queues[frozen_queue].pop();
		//current head position is assigned
		if( queues[frozen_queue].IsEmpty() ) {
			change_queue();
			direction();
		}
		return true;
	}
}

void disk::admit_PCB( PCB& input )
{
	//cout << "inserting into queue " << request_queue << endl;
	//cout << input << endl;
	if( queues[0].IsEmpty() && queues[1].IsEmpty() )
	{
		cout << "No processes in both queues. Picking a queue to be the request queue" << endl;


		queues[request_queue].orderly_insert( input );//Places in the req queue currently empty
		change_queue();//The other queue will be used solely to handle incoming requests.
		//The queue with the single process has now become the frozen queue.

	}
	else
	{
		cout << "Placing process into the request queue" << endl;

		queues[request_queue].orderly_insert( input );//insert into current queue

		if(request_queue == 0 && queues[1].IsEmpty()){

			cout << "Frozen queue is empty. Current request queue is now frozen" << endl;
			change_queue();
			direction();
		}
		else if (request_queue == 1 && queues[0].IsEmpty() ) {

			cout << "Frozen queue is empty. Current request queue is now frozen" << endl;
			change_queue();
			direction();
		}
	}
}

void disk::set_cylinders( int input )
{
	cylinders = input;
}

int disk::return_cylinders()
{
	return cylinders;
}

void disk::print_queues()
{
	if( queues[0].IsEmpty() && queues[1].IsEmpty() )
	{
		cout << "No processes waiting" << endl;
		return;
	}
	else
	{
		if(request_queue == 0)
		{
			if(current_direction == 0)//deQueue frozen, pop request
			{
				   print_queue_left(queues[request_queue], queues[1]);
			}
			else//pop frozen, dequeue request
			{
				print_queue_right(queues[request_queue], queues[1]);
			}
		}
		else if(request_queue == 1)
		{

			if(current_direction == 0)
			{
				print_queue_left(queues[request_queue], queues[0]);

			}
			else
			{
				print_queue_right(queues[request_queue], queues[0]);
			}

		}	
	}
}


void disk::print_queue_left(LinkedList<PCB> request, LinkedList<PCB> frozen)
{
	//cout << "Frozen is least to most" << endl;
	//cout << "Requests will be most to least" << endl;
	PCB vessel; char see_next = 'x';

	//cout << "queues have been assigned to secondary print queues" << endl;

	cout << "frozen queue" << endl;
	cout << "PID" << "        " << "FName" << "      " <<  "FLoc"<< "       " << "FSiz" 
         << "       " << "r/w" << "        " << "NxBrst" <<  "     " << "Mem" << endl;

    while( ! frozen.IsEmpty() ){


        vessel = frozen.peek();

        frozen.deQueue();

        vessel.print_disk_stats();


    }

        cout << "request queue" << endl;

	cout << "PID" << "        " << "FName" << "      " <<  "FLoc"<< "       " << "FSiz" 
         << "       " << "r/w" << "        " << "NxBrst" <<  "     " << "Mem" << endl;


        while( ! request.IsEmpty() )
        {
            vessel = request.peekB();
            request.pop();

			vessel.print_disk_stats();


		}
}

void disk::print_queue_right(LinkedList<PCB> request, LinkedList<PCB> frozen)
{

	//cout << "Frozen is most to least" << endl;
	//cout << "Requests will be least to most" << endl;

	PCB vessel; char see_next = 'x'; 

	cout << "frozen queue" << endl;
	cout << "PID" << "        " << "FName" << "      " <<  "FLoc"<< "       " << "FSiz" 
         << "       " << "r/w" << "        " << "NxBrst" <<  "     " << "Mem" << endl;


    while( ! frozen.IsEmpty() ){
        //cout << "Looks like frozen isn't empty" << endl;
        vessel = frozen.peekB();
        frozen.pop();

        vessel.print_disk_stats();

    }

    cout << "request queue" << endl;

	cout << "PID" << "        " << "FName" << "      " <<  "FLoc"<< "       " << "FSiz" 
         << "       " << "r/w" << "        " << "NxBrst" <<  "     " << "Mem" << endl;

    while( ! request.IsEmpty() )
    {
        //cout << "Looks like request isn't empty" << endl;
        vessel = request.peek();
        request.deQueue();
        vessel.print_disk_stats();

	}
}

#endif
