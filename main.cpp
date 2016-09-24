#include <stdint.h>
#include <stddef.h>

#include <stdio.h>

#include "Chain.h"

int main() {
	utils::ChainBuffer<20> chainBuffer;
	utils::Chain chain(chainBuffer);

	int32_t counter = 0;

	chain
		.start([&counter]() { printf("Start command [%d]\n", counter); counter++; })
		.if_([&counter]() -> bool { return true; })
			.then([&counter]() { printf("If command, true block [%d]\n", counter); counter++; })
			.then([&counter]() { printf("If command, true block [%d]\n", counter); counter++; })
		.if_end()
		.then([&counter]() { printf("Then command [%d]\n", counter); counter++; })
		.if_([&counter]() -> bool { return true; })
			.then([&counter]() { printf("If command, true block [%d]\n", counter); counter++; })
			.then([&counter]() { printf("If command, true block [%d]\n", counter); counter++; })
		.if_end()
	.end();

	chain.exec();

	return 0;
}