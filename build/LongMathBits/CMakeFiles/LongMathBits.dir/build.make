# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/augrossi/Documents/LongMathProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/augrossi/Documents/LongMathProject/build

# Include any dependencies generated for this target.
include LongMathBits/CMakeFiles/LongMathBits.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include LongMathBits/CMakeFiles/LongMathBits.dir/compiler_depend.make

# Include the progress variables for this target.
include LongMathBits/CMakeFiles/LongMathBits.dir/progress.make

# Include the compile flags for this target's objects.
include LongMathBits/CMakeFiles/LongMathBits.dir/flags.make

LongMathBits/CMakeFiles/LongMathBits.dir/codegen:
.PHONY : LongMathBits/CMakeFiles/LongMathBits.dir/codegen

LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o: LongMathBits/CMakeFiles/LongMathBits.dir/flags.make
LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o: /Users/augrossi/Documents/LongMathProject/LongMathBits/LongMathBits.cpp
LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o: LongMathBits/CMakeFiles/LongMathBits.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/augrossi/Documents/LongMathProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o"
	cd /Users/augrossi/Documents/LongMathProject/build/LongMathBits && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o -MF CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o.d -o CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o -c /Users/augrossi/Documents/LongMathProject/LongMathBits/LongMathBits.cpp

LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/LongMathBits.dir/LongMathBits.cpp.i"
	cd /Users/augrossi/Documents/LongMathProject/build/LongMathBits && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/augrossi/Documents/LongMathProject/LongMathBits/LongMathBits.cpp > CMakeFiles/LongMathBits.dir/LongMathBits.cpp.i

LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/LongMathBits.dir/LongMathBits.cpp.s"
	cd /Users/augrossi/Documents/LongMathProject/build/LongMathBits && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/augrossi/Documents/LongMathProject/LongMathBits/LongMathBits.cpp -o CMakeFiles/LongMathBits.dir/LongMathBits.cpp.s

# Object files for target LongMathBits
LongMathBits_OBJECTS = \
"CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o"

# External object files for target LongMathBits
LongMathBits_EXTERNAL_OBJECTS =

LongMathBits/libLongMathBits.a: LongMathBits/CMakeFiles/LongMathBits.dir/LongMathBits.cpp.o
LongMathBits/libLongMathBits.a: LongMathBits/CMakeFiles/LongMathBits.dir/build.make
LongMathBits/libLongMathBits.a: LongMathBits/CMakeFiles/LongMathBits.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/augrossi/Documents/LongMathProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libLongMathBits.a"
	cd /Users/augrossi/Documents/LongMathProject/build/LongMathBits && $(CMAKE_COMMAND) -P CMakeFiles/LongMathBits.dir/cmake_clean_target.cmake
	cd /Users/augrossi/Documents/LongMathProject/build/LongMathBits && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LongMathBits.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
LongMathBits/CMakeFiles/LongMathBits.dir/build: LongMathBits/libLongMathBits.a
.PHONY : LongMathBits/CMakeFiles/LongMathBits.dir/build

LongMathBits/CMakeFiles/LongMathBits.dir/clean:
	cd /Users/augrossi/Documents/LongMathProject/build/LongMathBits && $(CMAKE_COMMAND) -P CMakeFiles/LongMathBits.dir/cmake_clean.cmake
.PHONY : LongMathBits/CMakeFiles/LongMathBits.dir/clean

LongMathBits/CMakeFiles/LongMathBits.dir/depend:
	cd /Users/augrossi/Documents/LongMathProject/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/augrossi/Documents/LongMathProject /Users/augrossi/Documents/LongMathProject/LongMathBits /Users/augrossi/Documents/LongMathProject/build /Users/augrossi/Documents/LongMathProject/build/LongMathBits /Users/augrossi/Documents/LongMathProject/build/LongMathBits/CMakeFiles/LongMathBits.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : LongMathBits/CMakeFiles/LongMathBits.dir/depend

