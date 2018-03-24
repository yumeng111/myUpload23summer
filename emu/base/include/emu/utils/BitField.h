#ifndef __emu_utils_BitField_h__
#define __emu_utils_BitField_h__

#include <vector>
#include <string>
#include <stdexcept>
#include "stdint.h"

using namespace std;

namespace emu { namespace utils {

    // class BitField;
    
    
    template<class T>
    class BitField{
    public:
      struct Range_t{
	int lsb; /// Position of the least significant bit for calculating its value.
	int msb; /// Position of the  most significant bit for calculating its value.
	Range_t( int l, int m ){
	  lsb = l;
	  msb = m;
	}
      };
      BitField();
      BitField<T>& addRange( const int lsb, const int msb );
      BitField<T>& setName( const string& name ){ name_ = name; return *this; }
      BitField<T>& setCanonicalValue( const T value ){ canonicalValue_ = value; hasCanonicalValue_ = true; return *this; }
      string getName() const { return name_; }
      T getCanonicalValue() const { return canonicalValue_; }
      T getValue( const T *const data ) const;
      bool isCanonicalValue( const T *const data ) const { return ( !hasCanonicalValue_ ) || ( canonicalValue_ == getValue( data ) ); }
    private:
      string name_;
      vector< BitField<T>::Range_t > ranges_;
      bool hasCanonicalValue_;
      T canonicalValue_;
    };
  }
}

template<class T>
emu::utils::BitField<T>::BitField()
  : hasCanonicalValue_( false )
{}

template<class T>
emu::utils::BitField<T>&
emu::utils::BitField<T>::addRange( const int lsb, const int msb ){
  if ( lsb < 0 || sizeof( T ) * 8 <= (size_t) lsb ) throw runtime_error( "The least significant bit given is out of range." );
  if ( msb < 0 || sizeof( T ) * 8 <= (size_t) msb ) throw runtime_error( "The most significant bit given is out of range." );
  ranges_.push_back( emu::utils::BitField<T>::Range_t( lsb, msb ) );
  return *this;
}

template<class T>
T emu::utils::BitField<T>::getValue( const T *const data ) const {
  T value( 0 );
  const T one( 1 );
  int pos( 0 ); // The position of the bit in the value word.
  for ( typename vector< emu::utils::BitField<T>::Range_t >::const_iterator r = ranges_.begin(); r != ranges_.end(); ++r ){
    int step( r->lsb <= r->msb ? 1 : -1 );
    for ( int i = r->lsb; i != r->msb + step; i += step ){
      if ( i >= pos ) value |= ( *data & ( one << i ) ) >> ( i - pos );
      else            value |= ( *data & ( one << i ) ) << ( pos - i );
      ++pos;
    }
  }
  return value;
}

#endif
