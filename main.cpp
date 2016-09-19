#include <stdint.h>
#include <stddef.h>

#include <stdio.h>

#include "Command.h"

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

	CommandChainBuffer<6> chainBuffer;
	CommandChain chain(chainBuffer);

	chain.start(testCmd)
		.then(testCmd)
		.then(testCmd)
		.if_( []() -> bool { return true; } )
			.then(trueCmd)
			.then(trueCmd)
		.else_()
			.then(falseCmd)
			.then(falseCmd)
		.if_end();

	chain.debugPrint();

	fflush(stdin);
	getchar();

	return 0;
}