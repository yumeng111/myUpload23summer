#ifndef __emu_daq_rui_FEROLDataStream_h__
#define __emu_daq_rui_FEROLDataStream_h__

#include "emu/ldaq/server/Base.h"

#include "toolbox/mem/Pool.h"
#include "toolbox/mem/Reference.h"

using namespace std;

namespace emu { namespace ldaq { namespace rui {
      
      class FEROLDataStream{

      public:
	FEROLDataStream( uint16_t fedId );
	~FEROLDataStream();
	size_t insertData( char *const data, const size_t dataLength, const emu::ldaq::server::PositionInEvent_t position, const uint32_t l1a );
	toolbox::mem::Reference* extractBlock();
	bool hasBlocksToExtract() const;
	void printData(std::ostream& os, char *const data, const size_t dataLength);
      private:
	void createMemoryPool();
	void forcePreviousBlockToEndEvent();
	uint16_t fedId_;
	uint16_t fedIdFromData_;
	const uint32_t maxPayloadSize_;
	bool isFirstBlock_;
	uint16_t blockNumber_;
	uint16_t l1a_;
	toolbox::mem::Pool* pool_;
	toolbox::net::URN*  poolURN_;
	deque<toolbox::mem::Reference*> blocks_;
      };

}}} // namespace emu::ldaq::rui

#endif
