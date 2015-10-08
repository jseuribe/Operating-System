/********************************************************************************************************************************************************
 Title: Linked_List.h
 Author: Jose Antonio Uribe
 Created: May 9, 2014
 Purpose:A stack/queue hybrid combination, this class may act as a stack or a queue when needed. It may also act as a standard linked list, given a position, it may either tell you what is present at that location, and also may even remove the object at that location.
 Build with: g++ -o programname Your_Main_Function_Holding_File_Here".cpp

!NOTE!: Remember to #include the header Linked_List_Project3.h into your main file! This is required for compilation!
!NOTE! Include namespace UribeLinkedList whenever you need to use these definitions!
********************************************************************************************************************************************************/
#ifndef URIBELINKEDLIST_H
#define URIBELINKEDLIST_H

namespace UribeLinkedList
{
template<class T>
    class node//Our class node. It is the basis of our LinkedList class
    {
    public:
        node();//imp
        void destroyLastNode();
        void deleteThisNode(int loc);
        void insert_new_node(const T new_data);
        void appendNewNode(const T new_data, int loc);
        void setData(const T inp);
        void setNext(node<T> *new_next);
        void setLast(node<T> *new_last);
        T getData();
        node<T>* getNext();
        node<T>* getLast();
        bool IsEmpty();//imp
        void print_All_Nodes();
        void operator=(node<T> *rhs);
    private:
        T content;
        node<T>* next;
        node<T>* last;

    };


    template<class T>
    class LinkedList//The definition of our LinkedList class
    {
    public:
        LinkedList();//.default constructor. Sets front and back to NULL.
        LinkedList(const LinkedList<T>& rhs);//.copy constructor. Creates a deep copy of our rhs into our new object.
        ~LinkedList();//.destructor. completely deallocates dynamically allocated memory.
        bool IsEmpty();//.checks to see if our linked list is empty.
        void push(const T new_item);//pushes a new item, such that it's the top.
        T pop();//.pops the topmost item out from the top
        const T& peek();//.peeks at the item in the front, as if it where a linked list.
        const T& peekB();//.peeks at the item in the back, as if it where a linked list.
        void enQueue(const T new_item);//.queues an item into the back of the list.
        T deQueue();//.removes the first item of our list.
        T elementAt(int num);//.checks the element at num. NOTE: will return garbage if checking an item bigger than the amount of items.
        T popElementAt(int num);//.pops out the item at num.
        void orderly_insert(const T new_data);
        void insertElementAt(const T& new_item, int num);//.inserts an item at num.
        LinkedList<T>& operator=(LinkedList<T>& rhs);//.assignment operator. allows for chain assignments.
        int length();

    private:
        node<T> *frontEnd;
        node<T> *backEnd;
        int queue_length;
    };

}//UribeLinkedList

#endif //URIBELINKEDLIST_H


