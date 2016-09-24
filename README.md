# CommandChain

## Сборка примера
Для запуска примера выполнить `make all`. Если Windows, приложить к exe `libgcc_s_dw2-1.dll` и `libstdc++-6.dll`.

## Пример использования
Все, что необходимо - приложить к проекту два файла - CommandChain.h и CommandChain.cpp. Для использования CommandChain предварительно нужно создать буфер на необходимое число команд, при этом if входит в число команд, а if_end и end нет. 
```C++
Utils::CommandChainBuffer<20> chainBuffer;
```
Затем можно создлавать сам объект цепочки и через чейнинг задавать последовательность неких команд:
```C++
Utils::CommandChain chain(chainBuffer);

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
```
Цепочка так же может начинаться с if:
```C++
chain
    .if_([&counter]() -> bool { return true; })
        .then([&counter]() { printf("If command, true block [%d]\n", counter); counter++; })
        .then([&counter]() { printf("If command, true block [%d]\n", counter); counter++; })
    .if_end()
.end();
```
Либо не содержать ветвлений вовсе:
```C++
chain
    .start([&counter]() { printf("Start command [%d]\n", counter); counter++; })
    .then([&counter]() { printf("Then command [%d]\n", counter); counter++; })
    .then([&counter]() { printf("Then command [%d]\n", counter); counter++; })
    .then([&counter]() { printf("Then command [%d]\n", counter); counter++; })
.end();
```
Для исполнения можно передавать все, что может поместить в себе std::function<> с сигнатурой void(). Для условий необходимо bool().
