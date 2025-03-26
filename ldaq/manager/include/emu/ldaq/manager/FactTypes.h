#ifndef __emu_ldaq_manager_FactTypes_h__
#define __emu_ldaq_manager_FactTypes_h__

#include <string>

using namespace std;

namespace emu { namespace ldaq { namespace manager {

  class LocalDAQStatusFact {
  public:
    enum ParameterName_t { runType, state, daqState, isSupervised, isBuildingEvents, nParameters };
    static const char*  getTypeName() { return typeName_; }
    static const string getParameterName( const ParameterName_t p ) { return parameterNames_[p]; }
  protected:
    static const char* const typeName_; 
    static const char* const parameterNames_[nParameters];
  };

}}} // namespace emu::ldaq::manager

#endif
