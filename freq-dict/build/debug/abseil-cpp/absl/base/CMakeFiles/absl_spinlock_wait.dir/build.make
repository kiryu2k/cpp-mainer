# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kirrryu/myprojects/freq-dict

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kirrryu/myprojects/freq-dict/build/debug

# Include any dependencies generated for this target.
include abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/compiler_depend.make

# Include the progress variables for this target.
include abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/progress.make

# Include the compile flags for this target's objects.
include abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/flags.make

abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/flags.make
abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o: ../../abseil-cpp/absl/base/internal/spinlock_wait.cc
abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kirrryu/myprojects/freq-dict/build/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o"
	cd /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o -MF CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o.d -o CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o -c /home/kirrryu/myprojects/freq-dict/abseil-cpp/absl/base/internal/spinlock_wait.cc

abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.i"
	cd /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kirrryu/myprojects/freq-dict/abseil-cpp/absl/base/internal/spinlock_wait.cc > CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.i

abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.s"
	cd /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kirrryu/myprojects/freq-dict/abseil-cpp/absl/base/internal/spinlock_wait.cc -o CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.s

# Object files for target absl_spinlock_wait
absl_spinlock_wait_OBJECTS = \
"CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o"

# External object files for target absl_spinlock_wait
absl_spinlock_wait_EXTERNAL_OBJECTS =

abseil-cpp/absl/base/libabsl_spinlock_wait.a: abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/internal/spinlock_wait.cc.o
abseil-cpp/absl/base/libabsl_spinlock_wait.a: abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/build.make
abseil-cpp/absl/base/libabsl_spinlock_wait.a: abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kirrryu/myprojects/freq-dict/build/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabsl_spinlock_wait.a"
	cd /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base && $(CMAKE_COMMAND) -P CMakeFiles/absl_spinlock_wait.dir/cmake_clean_target.cmake
	cd /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_spinlock_wait.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/build: abseil-cpp/absl/base/libabsl_spinlock_wait.a
.PHONY : abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/build

abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/clean:
	cd /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base && $(CMAKE_COMMAND) -P CMakeFiles/absl_spinlock_wait.dir/cmake_clean.cmake
.PHONY : abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/clean

abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/depend:
	cd /home/kirrryu/myprojects/freq-dict/build/debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kirrryu/myprojects/freq-dict /home/kirrryu/myprojects/freq-dict/abseil-cpp/absl/base /home/kirrryu/myprojects/freq-dict/build/debug /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base /home/kirrryu/myprojects/freq-dict/build/debug/abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : abseil-cpp/absl/base/CMakeFiles/absl_spinlock_wait.dir/depend
