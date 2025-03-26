#ifndef __emu_daq_rui_EventBufferRing_h__
#define __emu_daq_rui_EventBufferRing_h__

#include "emu/ldaq/rui/Ring.h"
#include "emu/ldaq/rui/EventBuffer.h"

using namespace std;

namespace emu { namespace ldaq { namespace rui {

      class EventBufferRing{
      public:

	friend ostream& operator<<( ostream& os, emu::ldaq::rui::EventBufferRing& ebr );

	EventBufferRing();
	EventBufferRing( const size_t size );
	~EventBufferRing();
	size_t setSize( size_t size );
	bool addData( const size_t dataSize, const char* data, const bool startsEvent );
	void emptyEventBuffers();
	list<const emu::ldaq::rui::EventBuffer*> getEventBuffers() const;

      private:
	Ring<emu::ldaq::rui::EventBuffer*> ring_; ///< Circular list of event buffers. 
	Ring<emu::ldaq::rui::EventBuffer*>::iterator current_; ///< Points to the event buffer that's currently being filled
      };

      ostream& operator<<( ostream& os, emu::ldaq::rui::EventBufferRing& ebr );

}}} // namespace emu::ldaq::rui

#endif
