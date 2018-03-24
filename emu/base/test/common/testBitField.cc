#include "emu/utils/BitField.h"
#include "emu/utils/String.h"
#include <iostream>
#include <string>

using namespace emu::utils;
using namespace std;


int main(int argc, char **argv)
{
  BitField<uint64_t> bf;
  bf.setName( "test" );
  bf.addRange( 31, 31 ).addRange( 3, 7 ).setCanonicalValue( 0x27 );
  uint64_t data( 0xfedcba98 );
  cout << sizeof( char ) << endl;
  cout << sizeof( uint64_t ) << endl;
  cout << binaryFrom<uint64_t>( data ) << endl;
  cout << binaryFrom<uint64_t>( bf.getValue( &data ) ) << endl;
  if ( bf.isCanonicalValue( &data ) ) cout << "has canonical value " << binaryFrom<uint64_t>( bf.getCanonicalValue() )<< endl;

  BitField<uint32_t> bff;
  bff.addRange( 30, 32 ); // This will fail.
  return 0;
}
