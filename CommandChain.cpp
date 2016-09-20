#include "CommandChain.h"

#include <stdio.h>

namespace Utils {
	CommandChain::CommandChain(ICommandChainBuffer &buff) 
		: chainBuffer(buff), currentAddPosition(0), useCondition(false), 
		trueSection(false), trueBlockSize(0), falseBlockSize(0), conditionIndex(0) {

	}

	CommandChain& CommandChain::start(ICommand& cmd) {
		this->currentAddPosition = 0;
		return this->then(cmd);
	}

	CommandChain& CommandChain::then(ICommand& cmd) {
		if (this->currentAddPosition < this->chainBuffer.getSize()) {
			ChainContainer* buffer = this->chainBuffer.getBuffer();
			buffer[this->currentAddPosition].cmd = &cmd;
			buffer[this->currentAddPosition].indexToNext = this->currentAddPosition + 1;
			if (this->useCondition) {
				if (this->trueSection) {
					this->trueBlockSize++;
				} else {
					this->falseBlockSize++;
				}
			}
			this->currentAddPosition++;
		}
		return *this;
	}

	CommandChain& CommandChain::if_(bool(*conditionCb)()) {
		if (this->currentAddPosition < this->chainBuffer.getSize()) {
			ChainContainer* buffer = this->chainBuffer.getBuffer();
			buffer[this->currentAddPosition].conditionCb = conditionCb;
			this->conditionIndex = this->currentAddPosition;
			this->useCondition = true;
			this->trueSection = true;
			this->currentAddPosition++;
		}
		return *this;
	}

	CommandChain& CommandChain::else_() {
		if (this->currentAddPosition < this->chainBuffer.getSize()) {
			this->trueSection = false;
		}
		return *this;
	}

	CommandChain& CommandChain::if_end() {
		if (this->currentAddPosition <= this->chainBuffer.getSize()) {
			ChainContainer* buffer = this->chainBuffer.getBuffer();
			buffer[this->conditionIndex].indexToNext = this->conditionIndex + 1;
			buffer[this->conditionIndex].indexToNextIfFalse = this->conditionIndex + this->trueBlockSize + 1;
			buffer[this->conditionIndex + this->trueBlockSize].indexToNext = 
				this->conditionIndex + this->trueBlockSize + this->falseBlockSize + 1;
		}
		return *this;
	}

	void CommandChain::end() {
		
	}
	
	void CommandChain::exec() {
		ChainContainer* buffer = this->chainBuffer.getBuffer();
		size_t i = 0;
		while ( i < this->currentAddPosition) {
			if (buffer[i].cmd) {
				buffer[i].cmd->exec();
				i = buffer[i].indexToNext;
			} else if (buffer[i].conditionCb) {
				if (buffer[i].conditionCb()) {
					i = buffer[i].indexToNext;
				} else {
					i = buffer[i].indexToNextIfFalse;
				}
			}
		}
	}

	void CommandChain::debugPrint() {
		ChainContainer* buffer = this->chainBuffer.getBuffer();
		for (size_t i = 0; i < this->chainBuffer.getSize(); ++i) {
			printf("[%2d] Next: %2d NextFalse: %2d\n",i, buffer[i].indexToNext, buffer[i].indexToNextIfFalse);
		}
	}
}