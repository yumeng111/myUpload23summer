#ifndef _emu_daq_rui_exception_Exception_h_
#define _emu_daq_rui_exception_Exception_h_

#include "xcept/Exception.h"

/**
 * Exceptions thrown by the  package in the
 * namespace emu::ldaq::rui::exception
 */

namespace emu { namespace ldaq {
    
  /**
   * Generic exception raised by the emu::ldaq::rui package.
   */
  XCEPT_DEFINE_EXCEPTION(rui, Exception)

  /**
   * I2O exception raised by the emu::ldaq::rui package.
   */
  XCEPT_DEFINE_EXCEPTION(rui, I2O)

}}

#endif
