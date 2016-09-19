#pragma once

#include <stdint.h>
#include <stddef.h>

#ifndef nullptr
#define nullptr NULL
#endif

#ifndef override
#define override
#endif

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
		// Types

		typedef enum {
			Command			= 0,
			Condition		= 1,
		} Type;

		// ------------------------------------------------------
		// Fields

		Type		containerType;
		size_t		indexToNext;

		// For command
		ICommand *cmd;

		// For condition
		bool	(*conditionCb)(void);
		size_t	indexToNextIfFalse;
		size_t	trueBranchSize;
		size_t	falseBranchSize;

		// ------------------------------------------------------
		// API

		ChainContainer()
			: containerType(Command), indexToNext(0) {}

		virtual ~ChainContainer() {}

		virtual size_t getNext() { return this->indexToNext; };
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

		CommandChain&	start(ICommand& cmd);
		CommandChain&	then(ICommand& cmd);
		CommandChain&	if_(bool(*conditionCb)());
		CommandChain&	else_();
		CommandChain&	if_end();

		void			debugPrint();
		void			exec();
	private:
		ICommandChainBuffer &chainBuffer;

		size_t	currentAddPosition;
	};
}