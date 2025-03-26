#ifndef __emu_utils_RingBuffer_h__
#define __emu_utils_RingBuffer_h__

#include <cstddef> // for size_t
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

template<typename Element>
class RingBuffer{

private:
  RingBuffer(); /// Force capacity to be set ==> no default ctor

public:
  RingBuffer( RingBuffer& other, bool isToBeTimeOrdered=false )
    : capacity_( other.getCapacity() )
    , elements_( new Element*[ capacity_ ] )
    , currentIndex_( other.getCapacity() - 1 )
    , nElements_( 0 )
  {
    for ( size_t i=0; i<capacity_; i++ ) *(elements_+i) = NULL;

    if ( isToBeTimeOrdered ){
      for ( size_t i=0; i<other.getNElements(); i++ ){
	addElement( *other.getTimeOrderedElementAt( i ) );
      }
    }
    else{
      for ( size_t i=0; i<other.getNElements(); i++ ){
	addElement( *other.getElementAt( i ) );
      }
    }

  }

  RingBuffer( size_t capacity )
    : capacity_( capacity > 0 ? capacity : 1 )
    , elements_( new Element*[ capacity ] )
    , currentIndex_( capacity_ - 1 )
    , nElements_( 0 )
  {
    if ( capacity == 0 ) throw out_of_range( "RingBuffer capacity must not be set to 0." );
    for ( size_t i=0; i<capacity_; i++ ) *(elements_+i) = NULL;
  }
  
  ~RingBuffer(){
    for ( size_t i=0; i<capacity_; i++ ) delete *(elements_+i);
    delete [] elements_;
  }

  void copyTo( RingBuffer &target, bool isToBeTimeOrdered=false ) const {
    if ( capacity_ > target.getCapacity() ){
      ostringstream oss;
      oss << "Copy target RingBuffer object has smaller capacity (" << target.getCapacity() << ") than the source (" << capacity_ << ")";
      throw out_of_range( oss.str() );
    }

    if ( isToBeTimeOrdered ){
      for ( size_t i=0; i<nElements_; i++ ){
	target.addElement( *getTimeOrderedElementAt( i ) );
      }
    }
    else{
      for ( size_t i=0; i<nElements_; i++ ){
	target.addElement( *getElementAt( i ) );
      }
    }
  }

  RingBuffer& addElement( const Element &e ){
    if ( currentIndex_+1 == capacity_ ) currentIndex_ = 0;
    else                                currentIndex_++;

    if ( *(elements_+currentIndex_) == NULL )  *(elements_+currentIndex_) = new Element( e );
    else                                      **(elements_+currentIndex_) = e;

    if ( nElements_ < capacity_ ) nElements_++;

    return *this;
  }

  RingBuffer& updateElement( const Element &e ){
    // Replace the newest element. (It does not advance the write index or increment the number of elements.)
    if ( *(elements_+currentIndex_) == NULL )  *(elements_+currentIndex_) = new Element( e );
    else                                      **(elements_+currentIndex_) = e;
    return *this;
  }

  size_t getCapacity () const { return capacity_ ; }
  size_t getNElements() const { return nElements_; }

  Element* getElementAt( size_t index ) const {
    if ( index >= capacity_ ){
      ostringstream oss;
      oss << "Element index " << index << " is out of range for RangeBuffer of capacity " << capacity_;
      throw out_of_range( oss.str() );
    }
    return *(elements_+index);
  }

  Element* getTimeOrderedElementAt( size_t index ) const {
    if ( index >= capacity_ ){
      ostringstream oss;
      oss << "Element index " << index << " is out of range for RangeBuffer of capacity " << capacity_;
      throw out_of_range( oss.str() );
    }
    return *(elements_+( ( getOldestIndex()+index )%capacity_ ));
  }

  size_t getNewestIndex() const {
    return currentIndex_;    
  }

  size_t getOldestIndex() const {
    if   ( nElements_ < capacity_ || currentIndex_+1 == capacity_ ) return 0;
    return currentIndex_+1;
  }
  
  Element* getNewest() const {
    return getElementAt( getNewestIndex() );
  }

  Element* getOldest() const {
    return getElementAt( getOldestIndex() );
  }

  Element* getFirst() const { return *elements_              ; }
  Element* getLast () const { return *(elements_+capacity_-1); }


private:
  size_t  capacity_; ///< the max number of elements
  Element **elements_; ///< array of pointers to elements
  size_t  currentIndex_; ///< write index
  size_t  nElements_; ///< number of elements added

};

template<typename T>
ostream& operator<<( ostream& os, const RingBuffer<T>& rb ){
  os << "capacity=" << rb.getCapacity()
     << " N_elements=" << rb.getNElements()
     << " [ ";
  for ( int i=0; i<rb.getCapacity(); i++ ){
    T* e = rb.getElementAt(i);
    if ( e==NULL ) os << "- ";
    else           os << *rb.getElementAt(i) << " ";
  }
  os << "]";
  return os;
}

#endif
