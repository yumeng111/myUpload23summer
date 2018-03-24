#include "emu/ldaq/rui/CDF_FED_Header.h"

emu::ldaq::rui::CDF_FED_Header::CDF_FED_Header( const uint64_t *const data )
  : data_( data )
{
  // See http://cmsdoc.cern.ch/cms/TRIDAS/horizontal/RUWG/DAQ_IF_guide/DAQ_IF_guide.html#CDF
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "SLk" ).addRange(  0,  1 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "Re1" ).addRange(  2,  2 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "LtH" ).addRange(  3,  3 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "FOV" ).addRange(  4,  7 ).setCanonicalValue( 0x0 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "SId" ).addRange(  8, 19 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "BxI" ).addRange( 20, 31 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "Lv1" ).addRange( 32, 55 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "EvT" ).addRange( 56, 59 ) );
  fields_.push_back( emu::utils::BitField<uint64_t>().setName( "BoE" ).addRange( 60, 63 ).setCanonicalValue( 0x5 ) );
}

ostream& emu::ldaq::rui::operator<<( ostream& os, const emu::ldaq::rui::CDF_FED_Header& fed_header ){
  os << "FED header\n";
  vector < emu::utils::BitField<uint64_t> > fields = fed_header.getFields();
  for ( vector < emu::utils::BitField<uint64_t> >::const_iterator f = fields.begin(); f != fields.end(); ++f ){
    os << " "   << f->getName()
       << " 0x" << hex << f->getValue( fed_header.getData() )
       << " (" << dec << f->getValue( fed_header.getData() ) << ") "
       << ( f->isCanonicalValue( fed_header.getData() ) ? "" : " not canonical!" )
       << "\n";
  }
  return os;
}
