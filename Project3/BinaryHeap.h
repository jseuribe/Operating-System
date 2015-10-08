#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "dsexceptions.h"
#include <vector>
using namespace std;

// BinaryHeap class
//
// CONSTRUCTION: with an optional capacity (that defaults to 100)
// This queue class is used by the <TEXTBOOK FROM 335 NAME > Here
// Two modifications were made: the addition of the find() and decreaseKey() functions.
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( minItem )   --> Remove (and optionally return) smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// NEW FUNCTIONS
// bool find(Comparable inp, Comparable &result) ==> Find inp in the heap, and delete it.
// bool decreaseKey(Comparable inp, int delta)   ==> Find inp in the heap, and augment it's priority.
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinaryHeap
{
  public:
    explicit BinaryHeap( int capacity = 100 )
      : array( capacity + 1 ), currentSize{ 0 }
    {
    }

    explicit BinaryHeap( const vector<Comparable> & items )
      : array( items.size( ) + 10 ), currentSize{ items.size( ) }
    {
        for( int i = 0; i < items.size( ); ++i )
            array[ i + 1 ] = items[ i ];
        buildHeap( );
    }

    bool isEmpty( ) const
      { return currentSize == 0; }

    /**
     * Find the smallest item in the priority queue.
     * Return the smallest item, or throw Underflow if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return array[ 1 ];
    }
    
    /**
     * Insert item x, allowing duplicates.
     */
    void insert( const Comparable & x )
    {
        if( currentSize == array.size( ) - 1 )
            array.resize( array.size( ) * 2 );

            // Percolate up
        int hole = ++currentSize;
        Comparable copy = x;
        
        array[ 0 ] = std::move( copy );
        for( ; x < array[ hole / 2 ]; hole /= 2 )
            array[ hole ] = std::move( array[ hole / 2 ] );
        array[ hole ] = std::move( array[ 0 ] );
    }
    

    /**
     * Insert item x, allowing duplicates.
     */
    void insert( Comparable && x )
    {
        if( currentSize == array.size( ) - 1 )
            array.resize( array.size( ) * 2 );

            // Percolate up
        int hole = ++currentSize;
        for( ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 )
            array[ hole ] = std::move( array[ hole / 2 ] );
        array[ hole ] = std::move( x );
    }
    
    /**
     * Remove the minimum item.
     * Throws UnderflowException if empty.
     */
    void deleteMin( )
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        array[ 1 ] = std::move( array[ currentSize-- ] );
        percolateDown( 1 );
    }

    /**
     * Remove the minimum item and place it in minItem.
     * Throws Underflow if empty.
     */
    void deleteMin( Comparable & minItem )
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        minItem = std::move( array[ 1 ] );
        array[ 1 ] = std::move( array[ currentSize-- ] );
        percolateDown( 1 );
    }

    void makeEmpty( )
      { currentSize = 0; }

    bool find(Comparable &inp, Comparable &result)
    {
      if( isEmpty() ) throw UnderflowException{ };


      cout << "Looking for process: " << inp.get_pid() << endl;
      if( decreaseKey(inp, -1000000 ) )//Decrease by some sort of obscenely large val. Enough to make it the top.
      {
        deleteMin( result );
        cout << "Result is: " << result.get_pid() << endl;
        result.key_normalize();//Return the tau to normal.
        return true;
      }
      else return false;
    }

  private:
    int                currentSize;  // Number of elements in heap
    vector<Comparable> array;        // The heap array

    /**
     * Establish heap order property from an arbitrary
     * arrangement of items. Runs in linear time.
     */
    void buildHeap( )
    {
        for( int i = currentSize / 2; i > 0; --i )
            percolateDown( i );
    }

    /**
     * Internal method to percolate down in the heap.
     * hole is the index at which the percolate begins.
     */
    void percolateDown( int hole )
    {
        int child;
        Comparable tmp = std::move( array[ hole ] );

        for( ; hole * 2 <= currentSize; hole = child )
        {
            child = hole * 2;
            if( child != currentSize && array[ child + 1 ] < array[ child ] )
                ++child;
            if( array[ child ] < tmp )
                array[ hole ] = std::move( array[ child ] );
            else
                break;
        }
        array[ hole ] = std::move( tmp );
    }

    bool decreaseKey(Comparable &data, float delta)
    {

        if( isEmpty( ) ) throw UnderflowException{ };

        Comparable temp; int p = 0; bool found = false;

        for( int i = 1; i <= currentSize; i++)//find the object we seek.
        {
          temp = array[i];

          cout << "Comparing to: " << temp.get_pid() << endl;
          if( temp == data )
          {
            p = i;
            found = true; //found the item
            cout << "Process " << temp.get_pid() << " is in position " << p << endl;
            break; //end the loop; we've found our match.
          }
        }

        if( found == false )
        {
          return false;
        }

        int hole = p;
        
        temp.key_decrease(delta);

        array[ 0 ] = std::move( temp );
        cout << "comparing " << temp.get_pid() << " to " << array[hole / 2].get_pid() << endl;
        cout << "comparison state: " << temp.return_comp_state() << endl;

        cout << temp.return_tau_current() << endl;
        cout << array[ hole / 2].return_tau_current() << endl;
        if( temp < array[ hole / 2 ] )
        {
          cout << "comparison passed" << endl;
        }
        else if( 0 == hole / 2 ) cout << "hole size " << endl;
        else cout << "fresh outta hecks" << endl;

        for( ; temp < array[ hole / 2 ]; hole /= 2 ){
            cout << "process " << array[hole/2].get_pid() << " is in position " << hole << endl;
            cout << "But will be moved to " << hole <<  " where process " << array[hole] << " is" << endl;
            array[hole] = std::move( array[hole/2] );//In order to get the value of the iD, we save our stored item.
        }
        cout << "Final position of " << temp.get_pid() << " is: " << hole << endl;
        array[ hole ] = std::move( array[ 0 ] );


        return true;//object has been found if we've progressed to this point. The item has been modified by delta.
    }
};

#endif
