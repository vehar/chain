#pragma once

#include <stdint.h>
#include <stddef.h>

#include <functional>

namespace Utils {
	// ===========================================================================
	// Command interface

	class ICommand {
	public:
		virtual ~ICommand() {}
		virtual void exec() = 0;
	};

	// ===========================================================================
	// Support types
	class ChainContainer {
	public:
		// ------------------------------------------------------
		// Fields
		size_t		indexToNext;

		// Chain function
		std::function<void()> fcn;

		// For condition
		std::function<bool()> conditionCb;
		size_t	indexToNextIfFalse;

		// ------------------------------------------------------
		// API

		ChainContainer()
			: indexToNext(0), fcn(nullptr), 
			conditionCb(nullptr), indexToNextIfFalse(0) {}

		virtual ~ChainContainer() {}
	};

	class ICommandChainBuffer {
	public:
		ICommandChainBuffer() {}
		virtual ~ICommandChainBuffer() {}

		virtual ChainContainer* getBuffer() = 0;
		virtual size_t getSize() const = 0;
	};

	template<size_t size>
	class CommandChainBuffer : public ICommandChainBuffer {
	public:
		CommandChainBuffer() : ICommandChainBuffer() {}
		virtual ChainContainer*	getBuffer()			{ return this->buffer; }
		virtual size_t			getSize() const		{ return this->bufferSize; }
	private:
		ChainContainer	buffer[size];
		const size_t	bufferSize = size;
	};

	// ===========================================================================
	// Commands chain

	class CommandChain {
	public:
		CommandChain(ICommandChainBuffer &buff);

		CommandChain&	start(std::function<void()> fcn);
		CommandChain&	then(std::function<void()> fcn);
		CommandChain&	if_(std::function<bool()> conditionCb);
		CommandChain&	else_();
		CommandChain&	if_end();
		void			end();

		void			exec();
	private:
		ICommandChainBuffer &chainBuffer;
		size_t			currentAddPosition;

		bool			useCondition;
		bool			trueSection;
		size_t			trueBlockSize;
		size_t			falseBlockSize;
		size_t			conditionIndex;
	};
}