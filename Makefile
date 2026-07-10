CXX      ?= clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -O2
LDFLAGS  :=

SRCS_LIB := src/Grid.cpp src/Bresenham.cpp src/RobotController.cpp src/Command.cpp src/CommandParser.cpp
OBJS_LIB := $(SRCS_LIB:.cpp=.o)

MAIN_SRC := src/main.cpp
MAIN_OBJ := $(MAIN_SRC:.cpp=.o)

TEST_SRC := tests/test_main.cpp
TEST_OBJ := $(TEST_SRC:.cpp=.o)

TARGET      := robot_movement
TEST_TARGET := robot_movement_tests

.PHONY: all clean test run

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJS_LIB) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(OBJS_LIB) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

run: $(TARGET)
	./$(TARGET) examples/example1.txt

clean:
	rm -f $(OBJS_LIB) $(MAIN_OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET)
