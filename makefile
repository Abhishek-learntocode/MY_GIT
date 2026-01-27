# Compiler
CXX = g++

# 1. TELL COMPILER WHERE TO FIND HEADERS (openssl/sha.h)
# (Standard MSYS2 location)
INCLUDES = -Iinclude -IC:/msys64/mingw64/include

# 2. TELL LINKER WHERE TO FIND LIBRARY FILES (.dll / .a)
LIB_DIRS = -LC:/msys64/mingw64/lib

# Compiler Flags
CXXFLAGS = -std=c++17 -Wall $(INCLUDES)

# Libraries to Link
LDFLAGS = $(LIB_DIRS) -lssl -lcrypto -lz

# Source Files
SRC = src/main.cpp src/commands/init.cpp src/commands/hash_object.cpp src/utils.cpp
OBJ = $(SRC:.cpp=.o)

# Output Binary
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