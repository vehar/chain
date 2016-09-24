#include "Chain.h"

namespace utils {
	Chain::Chain(IChainBuffer &buff) 
		: chainBuffer(buff), currentAddPosition(0), useCondition(false), 
		trueSection(false), trueBlockSize(0), falseBlockSize(0), conditionIndex(0) {

	}

	Chain& Chain::start(std::function<void()> fcn) {
		this->currentAddPosition = 0;
		return this->then(fcn);
	}

	Chain& Chain::then(std::function<void()> fcn) {
		if (this->currentAddPosition < this->chainBuffer.getSize()) {
			ChainContainer* buffer = this->chainBuffer.getBuffer();
			buffer[this->currentAddPosition].fcn = fcn;
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

	Chain& Chain::if_(std::function<bool()> conditionCb) {
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

	Chain& Chain::else_() {
		if (this->currentAddPosition < this->chainBuffer.getSize()) {
			this->trueSection = false;
		}
		return *this;
	}

	Chain& Chain::if_end() {
		if (this->currentAddPosition <= this->chainBuffer.getSize()) {
			ChainContainer* buffer = this->chainBuffer.getBuffer();
			buffer[this->conditionIndex].indexToNext = this->conditionIndex + 1;
			buffer[this->conditionIndex].indexToNextIfFalse = this->conditionIndex + this->trueBlockSize + 1;
			buffer[this->conditionIndex + this->trueBlockSize].indexToNext = 
				this->conditionIndex + this->trueBlockSize + this->falseBlockSize + 1;
		}
		this->useCondition = 0;
		this->trueSection = 0;
		this->trueBlockSize = 0;
		this->falseBlockSize = 0;
		this->conditionIndex = 0;
		return *this;
	}

	void Chain::end() {
		
	}
	
	void Chain::exec() {
		ChainContainer* buffer = this->chainBuffer.getBuffer();
		size_t i = 0;
		while ( i < this->currentAddPosition) {
			if (buffer[i].fcn != nullptr) {
				buffer[i].fcn();
				i = buffer[i].indexToNext;
			} else if (buffer[i].conditionCb != nullptr) {
				if (buffer[i].conditionCb()) {
					i = buffer[i].indexToNext;
				} else {
					i = buffer[i].indexToNextIfFalse;
				}
			}
		}
	}
}