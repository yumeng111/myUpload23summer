#include "emu/ldaq/rui/CDF_FED_Trailer.h"

emu::ldaq::rui::CDF_FED_Trailer::CDF_FED_Trailer( const uint64_t *const data )
  : data_( data )
{
  // See http://cmsdoc.cern.ch/cms/TRIDAS/horizontal/RUWG/DAQ_IF_guide/DAQ_IF_guide.html#CDF
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "SLk" ).addRange(  0,  1 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "CMd" ).addRange(  2,  2 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "LtT" ).addRange(  3,  3 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "TTS" ).addRange(  4,  7 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "ESt" ).addRange(  8, 11 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "Re1" ).addRange( 12, 13 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "IdE" ).addRange( 14, 14 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "SEr" ).addRange( 15, 15 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "CRC" ).addRange( 16, 31 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "WCt" ).addRange( 32, 55 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "Re2" ).addRange( 56, 59 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "EoE" ).addRange( 60, 63 ).setCanonicalValue( 0xa ) );
}

ostream& emu::ldaq::rui::operator<<( ostream& os, const emu::ldaq::rui::CDF_FED_Trailer& fed_trailer ){
  os << "FED trailer\n";
  vector < emu::utils::BitField<uint64_t> > fields = fed_trailer.getFields();
  for ( vector < emu::utils::BitField<uint64_t> >::const_iterator f = fields.begin(); f != fields.end(); ++f ){
    os << " "   << f->getName()
       << " 0x" << hex << f->getValue( fed_trailer.getData() )
       << " (" << dec << f->getValue( fed_trailer.getData() ) << ") "
       << ( f->isCanonicalValue( fed_trailer.getData() ) ? "" : " not canonical!" )
       << "\n";
  }
  return os;
}
