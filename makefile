# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Libraries
LDFLAGS = -lssl -lcrypto -lz

# Source Files (Updated)
SRC = src/main.cpp src/commands/init.cpp src/commands/hash_object.cpp src/utils.cpp
OBJ = $(SRC:.cpp=.o)

# Output Binary Name
TARGET = mygit.exe

# Default Rule
all: $(TARGET)

# Link Object Files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Compile Source Files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
ifeq ($(OS),Windows_NT)
	del /Q /F src\*.o src\commands\*.o src\utils.o mygit.exe
	rmdir /S /Q .mygit 2>NUL || echo ""
else
	rm -f $(OBJ) $(TARGET)
	rm -rf .mygit
endif