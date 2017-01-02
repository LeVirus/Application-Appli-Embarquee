DEBUG=yes
ifeq ($(DEBUG),yes)
	CXXFLAGS=-std=c++14 -Wall -Wextra -pedantic -g
else
	CXXFLAGS=-std=c++14 -march=native -O2
endif

LDFLAGS= -lopencv_imgproc -lopencv_imgcodecs -lopencv_objdetect -lopencv_core -lopencv_highgui -lboost_system -lboost_thread -lpthread
CXX= g++ -fPIC
OBJ_DIR=objets/
SRC_DIR=src/
INCLUDE_DIR=include/
LIB_DIR=lib/
BIN_DIR=

SRC=$(wildcard $(SRC_DIR)*.cpp)
SRC_BIS=$(subst $(SRC_DIR),$(OBJ_DIR),$(SRC))
OBJ=$(SRC_BIS:.cpp=.o)

EXE=exe

all: $(EXE)

$(EXE):main.cpp $(OBJ)
	$(CXX)  $(CXXFLAGS) -I $(INCLUDE_DIR) -L $(LIB_DIR) `pkg-config --libs opencv` $^ $(LDFLAGS) -o $@



$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(INCLUDE_DIR)%.h
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

.PHONY: clean mrproper

clean:
	rm -f $(OBJ_DIR)*.o

mrproper: clean
	rm -f $(EXE) 


