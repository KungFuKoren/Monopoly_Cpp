#kkorenn1@gmail.com
# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# SFML library flags
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Output executable names
TARGET_MAIN = monopoly_main
TARGET_TEST = monopoly_test

# Common source files (without main)
COMMON_SRCS = Block.cpp Board.cpp Gameplay.cpp Player.cpp

# Main source files
MAIN_SRCS = main.cpp
TEST_SRCS = test.cpp

# Object files
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Default target to build both versions
all: $(TARGET_MAIN) $(TARGET_TEST)

# Rule to link the object files and create the executables
$(TARGET_MAIN): $(COMMON_OBJS) $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_MAIN) $(COMMON_OBJS) $(MAIN_OBJS) $(SFML_LIBS)

$(TARGET_TEST): $(COMMON_OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_TEST) $(COMMON_OBJS) $(TEST_OBJS) $(SFML_LIBS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executables
clean:
	rm -f $(COMMON_OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(TARGET_MAIN) $(TARGET_TEST)

# Run the main program
run_main: $(TARGET_MAIN)
	./$(TARGET_MAIN)

# Run the test program
run_test: $(TARGET_TEST)
	./$(TARGET_TEST)

.PHONY: all clean run_main run_test
