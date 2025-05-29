# Output file
TARGET = build/Spotify.exe
TEST_TARGET = build/TestSpotify.exe

# Compiler and Flags
CC = g++
CFLAGS = -Wall -g -std=c++20 -fprofile-arcs -ftest-coverage

# Source folder, header folder
SRCDIR = src
INCDIR = include
OBJDIR = build/obj
TESTDIR = test
TEST_OBJDIR = build/obj/test

# Automatically find all subdirectories in Include and Src
INCLUDES = $(shell find $(INCDIR) -type d)
CFLAGS += $(foreach dir, $(INCLUDES), -I$(dir))

# Find all .cpp files in Src, excluding main.cpp for test
SRCS = $(filter-out $(SRCDIR)/main.cpp, $(shell find $(SRCDIR) -name "*.cpp"))
MAIN_SRC = $(SRCDIR)/main.cpp
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
MAIN_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(MAIN_SRC))

# Find all .cpp files in Test
TEST_SRCS = $(shell find $(TESTDIR) -name "*.cpp")
TEST_OBJS = $(patsubst $(TESTDIR)/%.cpp, $(TEST_OBJDIR)/%.o, $(TEST_SRCS))

# FFmpeg settings
FFMPEG_FLAGS = $(shell pkg-config --cflags --libs libavformat libavcodec libswresample libavutil libswscale)
CFLAGS += $(FFMPEG_FLAGS)
LDLIBS += $(shell pkg-config --libs libavformat libavcodec libswscale libswresample libavutil)

# SDL2 settings
SDL2_FLAGS = $(shell pkg-config --cflags --libs sdl2 SDL2_mixer)
CFLAGS += $(SDL2_FLAGS)
LDLIBS += $(shell pkg-config --libs sdl2 SDL2_mixer)

# FTXUI settings
FTXUI_FLAGS = -I./FTXUI/include
FTXUI_LIBS = ./FTXUI/build/libftxui-component.a ./FTXUI/build/libftxui-dom.a ./FTXUI/build/libftxui-screen.a
CFLAGS += $(FTXUI_FLAGS)
LDLIBS += $(FTXUI_LIBS) -pthread -lncurses

# TagLib settings
CFLAGS += -I/usr/include/taglib
LDFLAGS += -L/usr/lib/x86_64-linux-gnu
LDLIBS += -ltag

# ICU settings
ICU_FLAGS = -licuuc -licudata -licui18n
LDLIBS += $(ICU_FLAGS)

# libudev settings
LIBUDEV_FLAGS = -ludev
LDLIBS += $(LIBUDEV_FLAGS)

# Boost settings
BOOST_FLAGS = -lboost_system -lboost_filesystem -lboost_thread
CFLAGS += $(BOOST_FLAGS)
LDLIBS += $(BOOST_FLAGS)

# Google Test & Google Mock settings
GTEST_FLAGS = -lgtest -lgmock -pthread
GTEST_MAIN = -lgtest_main

# Main rule
all: $(TARGET)

# Create executable file
$(TARGET): $(OBJS) $(MAIN_OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Create test executable file
$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^ $(GTEST_FLAGS) $(GTEST_MAIN) $(LDFLAGS) $(LDLIBS)

# Compile .cpp files to .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule (clean object files, executable, and test files)
clean:
	rm -rf $(OBJDIR) $(TEST_OBJDIR) $(TARGET) $(TEST_TARGET) coverage.info coverage_filtered.info out

# Run executable
run: all
	./$(TARGET)

# Run unit tests
runtest: $(TEST_TARGET)
	@echo "Cleaning up previous GCOV data..."
	find . -name '*.gcda' -exec rm -f {} +
	@echo "Running tests..."
	./$(TEST_TARGET)

# Generate coverage report
coverage: $(TEST_TARGET)
	@echo "Generating code coverage..."
	@lcov --capture --directory build/obj/controller --output-file coverage.info
	@lcov --remove coverage.info '/usr/*' 'tests/*' 'external_libs/*' -o coverage_filtered.info
	@genhtml coverage_filtered.info --output-directory out
	@echo "Coverage report generated in 'out/' directory."

# Reset coverage data
reset:
	@echo "Resetting coverage data..."
	find . -name '*.gcda' -exec rm -f {} +
	find . -name '*.gcno' -exec rm -f {} +
	rm -rf coverage.info coverage_filtered.info out
