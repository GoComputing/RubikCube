# ****************************** Makefile setup ****************************** #
.PHONY: all run clean mrproper
.SECONDARY:


# *********************************** Paths ********************************** #
INC=include
SRC=src
OBJ=obj
BIN=bin


# ************************** Compile/link parameters ************************* #
CXX = g++
CXXFLAGS = -I$(INC) -c
CL = g++
CLFLAGS =


# **************************** General parameters **************************** #
EXEC = $(BIN)/main
EXEC_PARAMS = 


# ******************************* General rules ****************************** #
all: $(EXEC)


run: $(EXEC)
	./$(EXEC) $(EXEC_PARAMS)


# ******************************* Dependencies ******************************* #
$(BIN)/main: $(OBJ)/main.o
	@echo "Linking executable..."
	$(CL) $(CLFLAGS) -o $@ $^


$(OBJ)/main.o: $(SRC)/main.cpp
	@echo "Compiling main program..."
	$(CL) $(CXXFLAGS) -o $@ $^


# *********************************** Utils ********************************** #
clean:
	-rm $(OBJ)/*


mrproper: clean
	-rm $(EXEC)
