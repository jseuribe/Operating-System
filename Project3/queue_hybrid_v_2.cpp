/********************************************************************************************************************************************************
 Title: Linked_List.cpp
 Author: Jose Antonio Uribe
 Created: May 9, 2014
 Purpose:A stack/queue hybrid combination, this class may act as a stack or a queue when needed. It may also act as a standard linked list, given a position, it may either tell you what is present at that location, and also may even remove the object at that location.
 Build with: g++ -o programname Your_Main_Function_Holding_File_Here.cpp

!NOTE!: Remember to #include the header Linked_List_Project3.h into your main file! This is required for compilation!
!NOTE! Include namespace UribeLinkedList whenever you need to use these definitions!
********************************************************************************************************************************************************/
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstddef>
#include <iomanip>
using namespace std;

#ifndef URIBELINKEDLIST_H
#define URIBELINKEDLIST_H
#include "queue_hybrid.h"

////////////////////////////////////////////
//////////CLASS NODE IMPLEMENTATIONS ARE CONTAINED HERE/////
/////////////CLASS STACK IS BELOW/////////.

/********************************************************************************************************************************************************
Title: Linked_List.h
Author: Jose Antonio Uribe
Created: May 9, 2014
Purpose:A stack/queue hybrid combination, this class may act as a stack or a queue when needed. 
It may also act as a standard linked list, given a position, it may either tell you what is present at that location, 
and also may even remove the object at that location.

Build with: g++ -o programname Your_Main_Function_Holding_File_Here".cpp

!NOTE!: Remember to #include the header Linked_List_Project3.h into your main file! This is required for compilation!
!NOTE! Include namespace UribeLinkedList whenever you need to use these definitions!
********************************************************************************************************************************************************/



        template<class T>
        class node//Our class node. It is the basis of our LinkedList class
        {
        public:
            node();


            //imp
            void destroyLastNode();

            void deleteThisNode(int loc);

            void insert_new_node(const T new_data);

            void appendNewNode(const T new_data, int loc);

            void setData(const T inp);

            void setNext(node<T> *new_next);

            T& getData();

            node<T> *getNext();

            void setLast(node<T> *new_last);

            node<T> *getLast();

            bool IsEmpty();

            //imp
            void print_All_Nodes();

            void operator=(node<T> *rhs);

            bool operator<(T rhs);

        private:
            T content;
            node<T> *next;
            node<T> *last;

        };

    template<class T>
    class LinkedList;

    template<class T>
    ostream& operator<<(ostream& ost, const LinkedList<T> &rhs);

        template<class T>
        class LinkedList//The definition of our LinkedList class
        {
        public:
            LinkedList();
            //.default constructor. Sets front and back to NULL.

            LinkedList(const LinkedList<T> &rhs);
            //.copy constructor. Creates a deep copy of our rhs into our new object.

            ~LinkedList();
            //.destructor. completely deallocates dynamically allocated memory.

            bool IsEmpty(); //doubly linked
            //.checks to see if our linked list is empty.

            void push(const T new_item);//doubly linked

            //pushes a new item, such that it's the top.
            T pop();//doubly linked
            //.pops the topmost item out from the top

            T peek();//doubly linked
            //.peeks at the item in the front, as if it where a linked list.

            const T &peekB();//doubly linked
            //.peeks at the item in the back, as if it where a linked list.

            void enQueue(const T new_item);//doubly linked
            //.queues an item into the back of the list.

            T deQueue();//doubly linked
            //.removes the first item of our list.

            T elementAt(int num);
            //.checks the element at num. NOTE: will return garbage if checking an item bigger than the amount of items.
            
            T popElementAt(int num);
            //.pops out the item at num.

            bool find(T inp, T &data);
            //Finds and pops the element if matched

            void insertElementAt(const T &new_item, int num);
            //inserts in an orderly fashion. WILL NOT WORK IF ORDER IS DISTURBED.

            void orderly_insert( const T new_data);
            //.inserts an item at num.

            LinkedList<T> &operator=(LinkedList<T> &rhs);
            //.equality operator. true only if the lists are the same size, and all elements same.

            int length();

            void print_nodes();

            friend ostream& operator<< <> (ostream& ost, const LinkedList<T> &rhs);


        private:
            node<T> *frontEnd;
            node<T> *backEnd;
            int queue_length;
        };


        template<class T>
        node<T>::node() {
            //cout << "constructor time" << endl;
            next = NULL;
            last = NULL;

            
        }

        template<class T>
        bool node<T>::operator<(T rhs)
        {
            return content < rhs;
        }

        template<class T>
        void node<T>::destroyLastNode()//This will serve as our destructor. Due to the nature of the linked list, destruction is procedural
        {
            node<T> *currentNode, *before_node;
            currentNode = this;
            before_node = NULL;
            if (this->next == NULL) {
                //cout << "deleting our only node" << endl;
                delete this;

            }
            else {
                while (currentNode->next != NULL) {
                    before_node = currentNode;
                    currentNode = currentNode->next;
                }

                currentNode->last = NULL;//Sever any connection the last node had to our list.

                before_node->next = NULL;//Make sure our list now points to nothing.

                delete currentNode;//and our to-be-destroyed node is now our destroyed-node

            }


        }

        template<class T>
        void node<T>::deleteThisNode(int loc) {
            node<T> *temp_before, *temp_next;
            temp_before = NULL;//Before our first node, is nothing
            temp_next = this;
            int counter = 1;
            while (counter != loc) {
                temp_before = temp_next;//Our before node is now the the node we were currently pointing to
                temp_next = temp_next->next;//Our next next becomes our next node.
                counter++;
            }

            node<T> *new_next;
            new_next = temp_next->next;// n_n = t_n->next

            temp_before->next = new_next;// t_b -> t_n->next
            new_next->last = temp_before;// t_b <-> t_n->next
            delete temp_next;
        }

        template<class T>
        void node<T>::insert_new_node(const T new_data) {
            node<T> *temp, *before;
            temp = this;
            before = NULL;

            while (temp->next != NULL){

                before = temp;
                temp = temp->next;
            } 

            node<T> *n_node = new node<T>;//A body for the content is created

            n_node->next = NULL;//The content will be our new last node, so it's next points to NULL.
            n_node->content = new_data;//The body now holds

            temp->next = n_node;
            temp->last = before;
        }

        template<class T>
        void node<T>::appendNewNode(const T new_data, int loc) {
            node<T> *temp_before, *temp_next;
            temp_next = this;
            int counter = 1;

            while (counter != loc) {
                temp_before = temp_next;
                temp_next = temp_next->next;
                counter++;
            }

            node<T> *new_node;
            new_node = new node<T>;
            new_node->content = new_data;

            new_node->next = temp_next;// n_n -> t_n

            temp_next->last = new_node;//  n_n <-> t_n

            temp_before->next = new_node; // t_b -> n_n <-> t_n

            new_node->last = temp_before; // t_b <-> n_n <-> t_n
        }

        template<class T>
        bool node<T>::IsEmpty() {
            if (next == NULL) return 1;//if we have a top that's simply a pointer to the first element, this should work
            else return 0;
        }


        template<class T>
        void node<T>::setData(T inp) {
            content = inp;
        }

        template<class T>
        T& node<T>::getData() {
            return content;
        }

        template<class T>
        void node<T>::setNext(node<T> *new_next) {
            next = new_next;
        }

        template<class T>
        node<T> *node<T>::getNext() {
            return next;
        }

        template<class T>
        void node<T>::setLast(node<T> *new_last)
        {
            last = new_last;
        }

        template<class T>
        node<T> *node<T>::getLast()
        {
            return last;
        }

        template<class T>
        void node<T>::print_All_Nodes() {
            node<T> *temp;
            temp = this;

            while (temp != NULL) {
                cout << temp->content;
                temp = temp->next;
            }
            cout << endl;
        }

        template<class T>
        void node<T>::operator=(node<T> *rhs) {
            content = rhs->content;
            next = rhs->next;//Creates a deep copy. Note, that no new memory space is allocated. The same address has been passed.
            last = rhs->last;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////CLASS LinkedList BEGINS HERE////////////////////////////////////////////////////////////////////////////////////////////

        template<class T>
        LinkedList<T>::LinkedList()//Sets front and back of a newly created LinkedList to NULL; IE, Empty.
        {
            frontEnd = NULL;
            backEnd = NULL;
            queue_length = 0;
            queue_length = 0;
        }

        template<class T>
        LinkedList<T>::LinkedList(const LinkedList<T> &rhs) {
            frontEnd = NULL;
            backEnd = NULL;
            queue_length = rhs.queue_length;
            node<T> *p, *b, *new_prev;
            b = rhs.frontEnd;//An immediate accessor to our top node.
            p = NULL;

            new_prev = NULL;

            while (b != NULL)//We will progress through the stack as a linked list.
            {
                node<T> *new_node, *new_prev;
                new_node = new node<T>;
                new_node->setLast(NULL);

                new_node->setData(b->getData());

                if (p != NULL){// p -> new_node
                    new_node->setLast(p);
                    p->setNext(new_node);
                }
                else{

                    frontEnd = new_node;
                    new_node->setLast(NULL);
                }
                
                p = new_node;//P contains our current node. IE, this is what we're modifying
                b = b->getNext();
                p->setNext(NULL);
                if( b == NULL) backEnd = p;
            }

        }

        template<class T>
        LinkedList<T>::~LinkedList() {
            while (!(IsEmpty()))// A simple check to see if we should continue popping
            {
                pop();
            }
        }

        template<class T>
        bool LinkedList<T>::IsEmpty() {
            if (frontEnd == NULL) {    //cout << "empty as hecke" << endl;
                return true;
            }
            else return false;
        }

        template<class T>
        void LinkedList<T>::push(const T new_item) {
            node<T> *new_node;
            new_node = new node<T>;
            new_node->setData(new_item);

            if (IsEmpty()) {
                new_node->setLast(NULL);
                new_node->setNext(NULL);
                frontEnd = new_node;
                backEnd = new_node;
                queue_length++;
                return;
            }
            else {
                node<T> *temp;
                temp = frontEnd; // f -> ...

                temp->setLast(new_node); // n <- f <-> ...

                new_node->setNext(temp);// n <-> f <-> ...

                frontEnd = new_node;//  f-> n <-> temp <-> ...
            }
            queue_length++;

        }

        template<class T>
        T LinkedList<T>::pop() {
            //cout << "pop" << endl;
            T result;
            if (IsEmpty()) {
                cout << "Popping an empty stack" << endl;
                exit(1);
            }
            else if(queue_length == 1)
            {
                //cout << "popping the last element" << endl;
                node<T> *temp;
                result = backEnd->getData();
                delete backEnd;
                frontEnd = NULL;
                backEnd = NULL;
                queue_length--;
                return result;
            }
            else{
                node<T> *temp;

                result = backEnd->getData();
                //cout << "popping " << result << endl;
                temp = backEnd->getLast();// 2(temp)
                //cout << "obtained the back" << endl;

                T second_result;
                second_result = temp->getData();
                //cout << "The node behind the current is  " << second_result << endl;
                temp->setNext(NULL);
                //cout << "what was the back is now NULL" << endl;
                // 1 -> 2(temp) -|
                delete backEnd;

                backEnd = temp;

                queue_length--;//decrement by one
                //cout << result << endl;
                return result;

            }

        }

        template<class T>
        T LinkedList<T>::peek() {
            if (frontEnd == NULL) {
                cout << "peeking into an empty item" << endl;
                exit(1);
            }
            node<T> *temp;
            temp = frontEnd;
            T item;
            item = temp->getData();
            return item;
        }

        template<class T>
        const T &LinkedList<T>::peekB() {
            if (backEnd == NULL) {
                cout << "peeking into an empty item from the back" << endl;
                exit(1);
            }
            node<T> *temp;
            temp = backEnd;
            return temp->getData();
        }

        template<class T>
        void LinkedList<T>::enQueue(const T new_item)//Should add a new item to the END of the list.
        {
            node<T> *new_node;
            new_node = new node<T>;
            new_node->setData(new_item);
            new_node->setNext(NULL);
            queue_length++;

            if (IsEmpty())//If our new_node is just the first node to be added, it is treated as such:
            {
                frontEnd = new_node;
                backEnd = new_node;
                return;
            }
            else {
                node<T> *p; // ... <-> b
                p = backEnd; // ... <-> b=p

                p->setNext(new_node);// ... <-> b=p -> new_node
                //cout << "the new_node is " << new_item << endl;
                new_node->setLast(p);// ... <-> b=p <-> new_node
                //cout << "the previous node is " << p->getData() << endl;
                backEnd = new_node;//Our new node is set via the backEnd pointer
                // <-> p <-> new_node <- b
            }

        }

        template<class T>
        T LinkedList<T>::deQueue() {
            T result;
            if (IsEmpty()) {
                cout << "ATTEMPTING TO DEQUEUE AN EMPTY OBJECT" << endl;
                exit(1);
            }
            else {
                node<T> *discard;
                discard = frontEnd; // d -> front
                result = discard->getData();
                //cout << "discarding" << result << endl;

                //cout << "setting up the next up" << endl;

                node<T> *next_up;


                next_up = discard->getNext();// next_up -> d->next
                //cout << "next up is equal to discard->next" << endl;

                frontEnd = discard->getNext();//Our frontEnd is discarded and next is the new front
                //cout << "front is now discard->next" << endl;

                // f -> d->next

                if (frontEnd == NULL) backEnd = NULL;//If we dequeued our only node, we set the back to NULL as well
                discard->setLast(NULL);//Set the last pointer to NULL, just in case.
                delete discard;
                queue_length--;
                return result;
            }
        }

        template<class T>
        T LinkedList<T>::elementAt(int num) {
            node<T> *p;
            p = frontEnd;
            int counter = 0;

            while (counter != num && p->getNext() != NULL) {
                p = p->getNext();
                counter++;
            }
            if (counter == num) return p->getData();
            else exit(1);
        }

        template<class T>
        T LinkedList<T>::popElementAt(int num) {
            node<T> *p, *b;
            b = frontEnd;
            p = frontEnd;
            int counter = 0;

            if (num > 0 && frontEnd == NULL) {
                cout << "Attempting to pop an item in a position other than zero in an empty list" << endl;
                exit(1);
            }

            while (counter != num && p->getNext() != NULL) {
                b = p;//Will remember the previous position's location/address.
                p = p->getNext();
                counter++;
            }

            if (counter != num && p->getNext() == NULL)//Handles the case where we attempt to destroy a non-existant node
            {
                cout << "ERROR: POPPING AN ITEM FROM A LOCATION FAR BEYOND OUR BACKPOINT" << endl;
                exit(1);
            }

            T target;

            if (counter == num) {//Handles any case where the list has not gone out of bounds

                if (counter == 0) {//if counter == 0.
                    return deQueue();
                }

                else if (p->getNext() == NULL)//Case in which we are popping out the last node
                {
                    return pop();
                }

                else{

                node<T> *p_node;

                p_node = p->getNext();//retrieve p, the node after our to-be-deleted node.
                // ... <-> b <-> p <-> (p_next) <-> ...

                p_node->setLast(b);// ... <-> b <- p_next <->...

                b->setNext(p->getNext());//B, our before node, now points to the node AFTER p, our target
                // ... <-> b <-> p_next <->...

                target = p->getData();//Our data gets retrieved
                p->setLast(NULL);

                delete p;//and destroyed

                queue_length--;
                return target;

                }
            }
        }

        template<class T>
        bool LinkedList<T>::find(T inp, T& data)
        {
            node<T> *p, *b, *c;
            p = frontEnd;
            c = backEnd;

            T curr, result, curr_b;


            if( ! IsEmpty() )
            {

                curr = p->getData();
                curr_b = c->getData();

                if( curr == inp )// the front is checked
                {
                    result = deQueue(); //The first item is what we're looking for. A dequeue is sufficient.
                    data = result;
                    return true;
                }
                else if( curr_b == inp)//the back is checked.
                {
                    result = pop(); //The item is at the end; pop out from the back.
                    data = result;
                    return true;
                }
                else{

                    b = p;
                    p = p->getNext();
                    curr = p->getData();


                }

                while(p->getNext() != NULL)
                {
                    if( curr == inp )// If our item is matched.
                    {
                        data = p->getData();//The data is set here!

                        node<T> *p_next;
                        p_next = p->getNext();


                        b->setNext( p_next );// b <-> g <-> p, <-> b -> p && g <-> p
                        p_next->setLast( b );

                        delete p;
                        queue_length--;//Highly important!
                        return true; //The sought item has been found.

                    }
                    //Otherwise, we continue onwards...
                    b = p;
                    p = p->getNext();
                    curr = p->getData();
                }

                return false; //End the function call; we've failed to find the item if we got to the end.
            }
            else return false;//Prevents any nonsense when trying to go through a list that's empty
        }

        template<class T>
        void LinkedList<T>::orderly_insert(const T new_data)
        {
            int counter = 0;

            if( IsEmpty() ){
                enQueue( new_data );
            }
            else{
                node<T> *p, *b;
                p = frontEnd;

                T current = p->getData();
                while( p->getNext() != NULL ) //Either we find its spot, or its at the end
                {
                    //cout << "compared " << new_data << " against " << current << endl;
                    b = p;
                    //cout << "Getting the next datum"<<endl;
                    p = p->getNext();
                    if (current < new_data) counter++;

                    if( p != NULL ) current = p->getData();

                }
                if( current < new_data && p->getNext() == NULL)//If it's bigger, and nothing is next, inc by 1.
                {
                    counter+=1;
                }
                //cout << new_data << " is less than " << current << endl;
                
                //cout << "sup. inserting at " << counter << endl;
                insertElementAt( new_data, counter);
                

            
            }
            return;
                
        }

        template<class T>
        void LinkedList<T>::insertElementAt(const T &new_item, int num) {
            node<T> *p, *b;
            b = frontEnd;
            p = frontEnd;
            int counter = 0;

            if (num > 0 && frontEnd == NULL) {
                cout << "entering an item to a position other than zero for a non-empty list" << endl;
                exit(1);
            }
            else {
                while (counter != num && p->getNext() != NULL) {
                    b = p;//Will remember the previous position's location/address.
                    p = p->getNext();
                    counter++;
                }

                if (frontEnd == NULL)//We're setting our new item to the next node if the list is empty
                {
                    enQueue( new_item );//We're really just enqueueing/pushing. Same diff

                }
                else if( num == 0)
                {
                    push( new_item );
                }
                else if (p->getNext() == NULL && num == counter+1)//We're setting our new item to the back
                {
                    enQueue( new_item );
                }
                else {
                    node<T> *new_node;
                    new_node = new node<T>;
                    new_node->setData(new_item);
                    T target;

                    node<T> *temp;
                    temp = b->getNext();//we use our before node to insert our new item
                    b->setNext(new_node);

                    new_node->setLast(b);

                    new_node->setNext(temp);
                    temp->setLast( new_node );

                    queue_length++;
                }
        }
    }

        template<class T>
        LinkedList<T> &LinkedList<T>::operator=(LinkedList<T> &rhs) {
            if (this != &rhs)//Equating the same thing to itself is redundant, and completely useless.
            {
                while (!(IsEmpty())) pop();//Evacuates our calling object.
                if (rhs.IsEmpty()) {
                    frontEnd = NULL;
                    backEnd = NULL;
                    queue_length = 0;
                    return *this;
                }
                node<T> *p, *b, *new_prev;
                queue_length = rhs.queue_length;
                b = rhs.frontEnd;//An immediate accessor to our top node.
                p = NULL;

                while (b != NULL)//We will progress through the stack as a linked list.
                {
                    node<T> *new_node;
                    new_node = new node<T>;
                    new_node->setData(b->getData());

                    if (p != NULL){// p -> new_node
                    new_node->setLast(p);
                    p->setNext(new_node);
                    }
                    else frontEnd = new_node;

                    p = new_node;//P contains our current node. IE, this is what we're modifying
                    b = b->getNext();
                    p->setNext(NULL);
                    if(b == NULL) backEnd = p;
                }
                return *this;
            }
            else return *this;

        }

        template<class T>
        int LinkedList<T>::length()
        {
            return queue_length;
        }

        
        template<class T>
        ostream& operator<<(ostream& ost, const LinkedList<T> &rhs) {
            node<T> *p;
            char loop;
            p = rhs.frontEnd;

            while( p != NULL){

                //if(visited_first == false) ost << "*";
                ost << p->getData();
                p = p->getNext();
            }

            return ost;
                
            
        }

        template<class T>
        void LinkedList<T>::print_nodes() {
            node<T> *p;
            T the_data;
            p = frontEnd;

            while( p != NULL){

                //if(visited_first == false) ost << "*";
                the_data = p->getData();
                the_data.print_out();
                p = p->getNext();
            }                
            
        }


#endif
