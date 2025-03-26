#ifndef _emu_ldaq_manager_exception_Exception_h_
#define _emu_ldaq_manager_exception_Exception_h_

#include "xcept/Exception.h"

/**
 * Exceptions thrown by the  package in the
 * namespace emu::ldaq::manager::exception
 */

namespace emu { namespace ldaq {
    
  /**
   * Generic exception raised by the emu::ldaq::manager package.
   */
  XCEPT_DEFINE_EXCEPTION(manager, Exception)

}}

#endif
