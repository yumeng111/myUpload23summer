#ifndef __emu_daq_rui_CDF_FED_header_h__
#define __emu_daq_rui_CDF_FED_header_h__

#include "emu/utils/BitField.h"

#include <ostream>

using namespace std;

namespace emu { namespace ldaq { namespace rui {

      class CDF_FED_Header{
      public:
	friend ostream& operator<<( ostream& os, const CDF_FED_Header& fed_header );
	friend ostream& operator<<( ostream& os,       CDF_FED_Header& fed_header );
	CDF_FED_Header( const uint64_t *const data );
	vector < emu::utils::BitField<uint64_t> > getFields() const { return fields_; }
	const uint64_t * getData() const { return data_; }
      private:
	vector < emu::utils::BitField<uint64_t> > fields_;
	const uint64_t *const data_;
      };

    }
  }
}



#endif
