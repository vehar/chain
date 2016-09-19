#include "CommandChain.h"

#include <stdio.h>

namespace Utils {
	CommandChain::CommandChain(ICommandChainBuffer &buff)
		: chainBuffer(buff), currentAddPosition(0) {

	}

	CommandChain& CommandChain::start(ICommand& cmd) {
		this->currentAddPosition = 0;
		return this->then(cmd);
	}

	CommandChain& CommandChain::then(ICommand& cmd) {

		return *this;
	}

	CommandChain& CommandChain::if_(bool(*conditionCb)()) {

		return *this;
	}

	CommandChain& CommandChain::else_() {
		return *this;
	}

	CommandChain& CommandChain::if_end() {
		return *this;
	}

	void CommandChain::exec() {

	}

	void CommandChain::debugPrint() {
		ChainContainer* buffer = this->chainBuffer.getBuffer();
		for (size_t i = 0; i < this->chainBuffer.getSize(); ++i) {
			printf("[%2d] T: %d, Next: %2d\n",i, buffer[i].containerType, buffer[i].getNext());
		}
	}
}
