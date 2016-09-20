CC				= g++
COMPILE_OPTS	= -std=c++1z -Wall
LIBS			= 
OPTIMIZATION	= -O2
OBJ_DIR			= obj
BIN_DIR			= bin

EXEC			= test.exe
SOURCES			= CommandChain.cpp main.cpp
OBJECTS			= $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

all: dir $(EXEC)

dir:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(COMPILE_OPTS) $(OPTIMIZATION) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(LINK_OPTS) $(OBJECTS) -o $(BIN_DIR)/$(EXEC) $(LIBS)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)