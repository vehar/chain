# CommandChain

## Сборка примера
Для запуска примера выполнить `make all`. Если Windows, приложить к exe `libgcc_s_dw2-1.dll` и `libstdc++-6.dll`.

## Пример использования
Все, что необходимо - приложить к проекту два файла - CommandChain.h и CommandChain.cpp.
В качестве команд цепочки должен использоваться объект унаследованного от `ICommand` класса, например:
```C++
class TestCommand : public Utils::ICommand {
public:
	TestCommand() : ICommand() {}

	virtual void exec() override { printf("Test command\n"); }
};
```
Для использования CommandChain предварительно нужно создать буфер на необходимое число команд, при этом if входит в число команд, а if_end и end нет, а так же иметь тот набор команд, которые необходимо исполнять. Команды передаются по ссылке. 
```C++
TestCommand testCmd;
TrueCommand trueCmd;
FalseCommand falseCmd;
CommandChainBuffer<20> chainBuffer;
```
Затем можно создлавать сам объект цепочки и через чейнинг задавать последовательность неких команд:
```C++
CommandChain chain(chainBuffer);

chain
	.start(testCmd)
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
Цепочка так же может начинаться с if:
```C++
chain
	.if_([]() -> bool { return true; })
		.then(trueCmd)
		.then(trueCmd)
	.if_end()
	.then(testCmd)
.end();
```
Либо не содержать ветвлений вовсе:
```C++
chain
	.start(testCmd)
	.then(testCmd)
	.then(testCmd)
	.then(testCmd)
.end();
```
