# CommandChain

## Сборка
Выполнить `make all`. Если Windows, приложить к exe `libgcc_s_dw2-1.dll` и `libstdc++-6.dll`.

## Пример использования
```C++
using namespace Utils;

TestCommand testCmd;
TrueCommand trueCmd;
FalseCommand falseCmd;

CommandChainBuffer<7> chainBuffer;
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
	
chain.exec();
```
