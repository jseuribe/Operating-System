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
/////////////CLASS STACK IS BELOW/////////
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

            bool IsEmpty();

            //imp
            void print_All_Nodes();

            void operator=(node<T> *rhs);

        private:
            T content;
            node<T> *next;

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
            bool IsEmpty();

            //.checks to see if our linked list is empty.
            void push(const T new_item);

            //pushes a new item, such that it's the top.
            T pop();

            //.pops the topmost item out from the top
            const T &peek();

            //.peeks at the item in the front, as if it where a linked list.
            const T &peekB();

            //.peeks at the item in the back, as if it where a linked list.
            void enQueue(const T new_item);

            //.queues an item into the back of the list.
            T deQueue();

            //.removes the first item of our list.
            T elementAt(int num);

            //.checks the element at num. NOTE: will return garbage if checking an item bigger than the amount of items.
            T popElementAt(int num);

            //.pops out the item at num.
            void insertElementAt(const T &new_item, int num);

            //.inserts an item at num.
            LinkedList<T> &operator=(LinkedList<T> &rhs);

            //.assignment operator. allows for chain assignments.
            bool operator==(LinkedList<T> &rhs);

            //.equality operator. true only if the lists are the same size, and all elements same.

            int length();

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
        }

        template<class T>
        void node<T>::destroyLastNode()//This will serve as our destructor. Due to the nature of the linked list, destruction is procedural
        {
            node<T> *currentNode, *before_node;
            currentNode = this;
            before_node;
            if (this->next == NULL) {
                cout << "deleting our only node" << endl;
                delete this;

            }
            else {
                while (currentNode->next != NULL) {
                    before_node = currentNode;
                    currentNode = currentNode->next;
                }

                delete currentNode;//and our to-be-destroyed node is now our destroyed-node
                before_node->next = NULL;
            }


        }

        template<class T>
        void node<T>::deleteThisNode(int loc) {
            node<T> *temp_before, *temp_next;
            temp_before;//Before our first node, is nothing
            temp_next = this;
            int counter = 1;
            while (counter != loc) {
                temp_before = temp_next;//Our before node is now the the node we were currently pointing to
                temp_next = temp_next->next;//Our next next becomes our next node.
                counter++;
            }

            temp_before->next = temp_next->next;
            delete temp_next;
        }

        template<class T>
        void node<T>::insert_new_node(const T new_data) {
            node<T> *temp;
            temp = this;

            while (temp->next != NULL) temp = temp->next;

            node<T> *n_node = new node<T>;//A body for the content is created
            n_node->next = NULL;//The content will be our new last node, so it's next points to NULL.
            n_node->content = new_data;//The body now holds
            temp->next = n_node;
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
            new_node->next = temp_next;
            temp_before->next = new_node;
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
            node<T> *p, *b;
            b = rhs.frontEnd;//An immediate accessor to our top node.
            p = NULL;

            while (b != NULL)//We will progress through the stack as a linked list.
            {
                node<T> *new_node;
                new_node = new node<T>;
                new_node->setData(b->getData());
                if (p != NULL) p->setNext(new_node);
                else frontEnd = new_node;
                p = new_node;//P contains our current node. IE, this is what we're modifying
                b = b->getNext();
                p->setNext(NULL);
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
                new_node->setNext(NULL);
                frontEnd = new_node;
                backEnd = new_node;
                return;
            }
            else {
                node<T> *temp;
                temp = frontEnd;
                new_node->setNext(temp);
                frontEnd = new_node;
            }
            queue_length++;

        }

        template<class T>
        T LinkedList<T>::pop() {
            T result;
            if (IsEmpty()) {
                cout << "Popping an empty stack" << endl;
                exit(1);
            }
            else {
                queue_length--;
                node<T> *temp;
                temp = frontEnd;
                result = temp->getData();//Grabs the first node, its data, sets front to the next node, and deletes the old node
                frontEnd = temp->getNext();
                delete temp;
                if (frontEnd == NULL) backEnd = NULL;

                return result;


            }

        }

        template<class T>
        const T &LinkedList<T>::peek() {
            if (frontEnd == NULL) {
                cout << "peeking into an empty item" << endl;
                exit(1);
            }
            node<T> *temp;
            temp = frontEnd;
            return temp->getData();
        }

        template<class T>
        const T &LinkedList<T>::peekB() {
            if (backEnd == NULL) {
                cout << "peeking into an empty item" << endl;
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
                node<T> *p;
                p = backEnd;
                p->setNext(new_node);
                backEnd = new_node;//Our new node is set via the backEnd pointer
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
                discard = frontEnd;
                result = discard->getData();
                node<T> *next_up;
                next_up = discard->getNext();
                frontEnd = discard->getNext();//Our frontEnd is discarded and next is the new front
                if (frontEnd == NULL) backEnd = NULL;//If we dequeued our only node, we set the back to NULL as well
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

            if (counter == num) {

                if (counter == 0) {
                    if (frontEnd == NULL)//If we have only one node, this handles the situation
                    {
                        return pop();

                    }
                    else//If this is just the first node, we do this. We're simply deleting the last node.
                    {
                        node<T> *temp;
                        temp = new node<T>;
                        temp = frontEnd;
                        target = temp->getData();
                        frontEnd = temp->getNext();
                        delete temp;
                        queue_length--;
                        return target;
                    }


                }

                else if (p->getNext() == NULL)//Case in which we are popping out the last node
                {
                    b->setNext(NULL);
                    target = p->getData();
                    delete p;
                    backEnd = b;
                    queue_length--;
                    return target;
                }

                else b->setNext(p->getNext());//B, our before node, now points to the node AFTER p, our target
                target = p->getData();//Our data gets retrieved
                delete p;//and destroyed
                queue_length--;
                return target;
            }
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
                if (counter > num + 1 && p->getNext() == NULL)//If we're adding one spot beyond our current size, it is acceptable. Not otherwise.
                {
                    cout << "ERROR: INSERTING AN ITEM TO A LOCATION FAR BEYOND OUR BACKPOINT" << endl;
                    exit(1);
                }

                node<T> *new_node;
                new_node = new node<T>;
                new_node->setData(new_item);
                T target;
                if (frontEnd == NULL)//We're setting our new item to the next node if the list is empty
                {
                    new_node->setNext(b);
                    frontEnd = new_node;
                    backEnd = new_node;
                    queue_length++;

                }
                else if (p->getNext() == NULL)//We're setting our new item to the back
                {
                    p->setNext(new_node);
                    backEnd = new_node;
                    new_node->setNext(NULL);
                    queue_length++;
                }
                else {
                    node<T> *temp;
                    temp = b->getNext();//we use our before node to insert our new item
                    b->setNext(new_node);
                    new_node->setNext(temp);
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
                node<T> *p, *b;
                queue_length = rhs.queue_length;
                b = rhs.frontEnd;//An immediate accessor to our top node.
                p = NULL;

                while (b != NULL)//We will progress through the stack as a linked list.
                {
                    node<T> *new_node;
                    new_node = new node<T>;
                    new_node->setData(b->getData());
                    if (p != NULL) p->setNext(new_node);
                    else frontEnd = new_node;
                    p = new_node;//P contains our current node. IE, this is what we're modifying
                    b = b->getNext();
                    p->setNext(NULL);
                }
                return *this;
            }
            else return *this;

        }

        template<class T>
        bool LinkedList<T>::operator==(LinkedList<T> &rhs) {

            if (this == &rhs) return true;
            else if (frontEnd == NULL && rhs.frontEnd == NULL) return true;//An empty list is always equal to an empty list
            else {
                node<T> *left, *right;
                left = frontEnd;
                right = rhs.frontEnd;
                int lc = 0, rc = 0;//our size counters.
                do {
                    if (left->getNext() != NULL) lc++;
                    if (right->getNext() != NULL) rc++;
                    if (lc == rc)//this check will continue only if lc and rc maintain the same value
                    {
                        if (left->getData() != right->getData()) {
                            return false;

                        }
                    }

                    left = left->getNext();
                    right = right->getNext();
                } while (left != NULL && right != NULL);

            }
            return true;
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
            int external_iterator_counter = 0;
            bool visited_first = false;

            while( p != NULL){
                if( rhs.queue_length > 10){//Prohibit input larger than 10 ONLY IF our queue length is bigger than 24!
                  for(int i = 0; i < 10; i++){
                    if( external_iterator_counter == rhs.queue_length) return ost;
                    //if(visited_first == false) ost << "*";
                    ost << p->getData();
                    p = p->getNext();
                    external_iterator_counter++;
                    visited_first = true;
                  }  
                  cout << "Would you like to see the next 10 entries?" << endl;
                  cout << "Enter y or n" << endl;
                  cin >> loop;
                  if(loop == 'n') return ost;
                  else{
                    cout << "~PID" << "            " << "F. Name" << "         " <<  "F. Loc"<< "           " << "F. Size" 
                 << "          " << "r/w" << endl;
                  }
                }
                else{
                    //if(visited_first == false) ost << "*";
                    ost << p->getData();
                    p = p->getNext();
                    visited_first = true;
                }
            }

            return ost;
                
            
        }
#endif
