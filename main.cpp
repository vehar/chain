#include <stdint.h>
#include <stddef.h>

#include <stdio.h>

#include "CommandChain.h"

class TestCommand : public Utils::ICommand {
public:
	TestCommand() : ICommand() {}

	virtual void exec() override { printf("Test command\n"); }
};

class TrueCommand : public Utils::ICommand {
public:
	TrueCommand() : ICommand() {}

	virtual void exec() override { printf("TRUE command\n"); }
};

class FalseCommand : public Utils::ICommand {
public:
	FalseCommand() : ICommand() {}

	virtual void exec() override { printf("FALSE command\n"); }
};

int main() {
	using namespace Utils;

	TestCommand testCmd;
	TrueCommand trueCmd;
	FalseCommand falseCmd;

	CommandChainBuffer<20> chainBuffer;
	CommandChain chain(chainBuffer);

	chain
		.start(testCmd)
		.if_([]() -> bool { return true; })
			.then(trueCmd)
			.then(trueCmd)
		.if_end()
		.then(testCmd)
	.end();

	chain.exec();

	return 0;
}