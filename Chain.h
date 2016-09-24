#pragma once

#include <stdint.h>
#include <stddef.h>

#include <functional>

namespace utils {
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

	class IChainBuffer {
	public:
		IChainBuffer() {}
		virtual ~IChainBuffer() {}

		virtual ChainContainer* getBuffer() = 0;
		virtual size_t getSize() const = 0;
	};

	template<size_t size>
	class ChainBuffer : public IChainBuffer {
	public:
		ChainBuffer() : IChainBuffer() {}
		virtual ChainContainer*	getBuffer()			{ return this->buffer; }
		virtual size_t			getSize() const		{ return this->bufferSize; }
	private:
		ChainContainer	buffer[size];
		const size_t	bufferSize = size;
	};

	// ===========================================================================
	// Commands chain

	class Chain {
	public:
		Chain(IChainBuffer &buff);

		Chain&	start	(std::function<void()> fcn);
		Chain&	then	(std::function<void()> fcn);
		Chain&	if_ 	(std::function<bool()> conditionCb);
		Chain&	else_ 	();
		Chain&	if_end	();
		void	end();

		void	exec();
	private:
		IChainBuffer &chainBuffer;
		size_t		currentAddPosition;

		bool		useCondition;
		bool		trueSection;
		size_t		trueBlockSize;
		size_t		falseBlockSize;
		size_t		conditionIndex;
	};
}