#ifndef __emu_daq_rui_CDF_FED_trailer_h__
#define __emu_daq_rui_CDF_FED_trailer_h__

#include "emu/utils/BitField.h"

#include <ostream>

using namespace std;

namespace emu { namespace ldaq { namespace rui {

      class CDF_FED_Trailer{
      public:
	friend ostream& operator<<( ostream& os, const CDF_FED_Trailer& fed_trailer );
	friend ostream& operator<<( ostream& os,       CDF_FED_Trailer& fed_trailer );
	CDF_FED_Trailer( const uint64_t *const data );
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
