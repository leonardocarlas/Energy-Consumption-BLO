# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /home/leonardo/clion-2021.2.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/leonardo/clion-2021.2.2/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/leonardo/Scrivania/BLO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/leonardo/Scrivania/BLO/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/blo.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/blo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/blo.dir/flags.make

CMakeFiles/blo.dir/src/main.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/blo.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/main.c.o -c /home/leonardo/Scrivania/BLO/src/main.c

CMakeFiles/blo.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/main.c > CMakeFiles/blo.dir/src/main.c.i

CMakeFiles/blo.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/main.c -o CMakeFiles/blo.dir/src/main.c.s

CMakeFiles/blo.dir/src/ll.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/ll.c.o: ../src/ll.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/blo.dir/src/ll.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/ll.c.o -c /home/leonardo/Scrivania/BLO/src/ll.c

CMakeFiles/blo.dir/src/ll.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/ll.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/ll.c > CMakeFiles/blo.dir/src/ll.c.i

CMakeFiles/blo.dir/src/ll.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/ll.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/ll.c -o CMakeFiles/blo.dir/src/ll.c.s

CMakeFiles/blo.dir/src/plot.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/plot.c.o: ../src/plot.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/blo.dir/src/plot.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/plot.c.o -c /home/leonardo/Scrivania/BLO/src/plot.c

CMakeFiles/blo.dir/src/plot.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/plot.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/plot.c > CMakeFiles/blo.dir/src/plot.c.i

CMakeFiles/blo.dir/src/plot.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/plot.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/plot.c -o CMakeFiles/blo.dir/src/plot.c.s

CMakeFiles/blo.dir/src/read_input.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/read_input.c.o: ../src/read_input.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/blo.dir/src/read_input.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/read_input.c.o -c /home/leonardo/Scrivania/BLO/src/read_input.c

CMakeFiles/blo.dir/src/read_input.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/read_input.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/read_input.c > CMakeFiles/blo.dir/src/read_input.c.i

CMakeFiles/blo.dir/src/read_input.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/read_input.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/read_input.c -o CMakeFiles/blo.dir/src/read_input.c.s

CMakeFiles/blo.dir/src/utils.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/utils.c.o: ../src/utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/blo.dir/src/utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/utils.c.o -c /home/leonardo/Scrivania/BLO/src/utils.c

CMakeFiles/blo.dir/src/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/utils.c > CMakeFiles/blo.dir/src/utils.c.i

CMakeFiles/blo.dir/src/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/utils.c -o CMakeFiles/blo.dir/src/utils.c.s

CMakeFiles/blo.dir/src/models/m1a.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/models/m1a.c.o: ../src/models/m1a.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/blo.dir/src/models/m1a.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/models/m1a.c.o -c /home/leonardo/Scrivania/BLO/src/models/m1a.c

CMakeFiles/blo.dir/src/models/m1a.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/models/m1a.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/models/m1a.c > CMakeFiles/blo.dir/src/models/m1a.c.i

CMakeFiles/blo.dir/src/models/m1a.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/models/m1a.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/models/m1a.c -o CMakeFiles/blo.dir/src/models/m1a.c.s

CMakeFiles/blo.dir/src/models/m1.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/models/m1.c.o: ../src/models/m1.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/blo.dir/src/models/m1.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/models/m1.c.o -c /home/leonardo/Scrivania/BLO/src/models/m1.c

CMakeFiles/blo.dir/src/models/m1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/models/m1.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/models/m1.c > CMakeFiles/blo.dir/src/models/m1.c.i

CMakeFiles/blo.dir/src/models/m1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/models/m1.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/models/m1.c -o CMakeFiles/blo.dir/src/models/m1.c.s

CMakeFiles/blo.dir/src/models/m2ewh.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/models/m2ewh.c.o: ../src/models/m2ewh.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/blo.dir/src/models/m2ewh.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/models/m2ewh.c.o -c /home/leonardo/Scrivania/BLO/src/models/m2ewh.c

CMakeFiles/blo.dir/src/models/m2ewh.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/models/m2ewh.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/models/m2ewh.c > CMakeFiles/blo.dir/src/models/m2ewh.c.i

CMakeFiles/blo.dir/src/models/m2ewh.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/models/m2ewh.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/models/m2ewh.c -o CMakeFiles/blo.dir/src/models/m2ewh.c.s

CMakeFiles/blo.dir/src/models/m2ev.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/models/m2ev.c.o: ../src/models/m2ev.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/blo.dir/src/models/m2ev.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/models/m2ev.c.o -c /home/leonardo/Scrivania/BLO/src/models/m2ev.c

CMakeFiles/blo.dir/src/models/m2ev.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/models/m2ev.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/models/m2ev.c > CMakeFiles/blo.dir/src/models/m2ev.c.i

CMakeFiles/blo.dir/src/models/m2ev.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/models/m2ev.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/models/m2ev.c -o CMakeFiles/blo.dir/src/models/m2ev.c.s

CMakeFiles/blo.dir/src/models/mo.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/models/mo.c.o: ../src/models/mo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/blo.dir/src/models/mo.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/models/mo.c.o -c /home/leonardo/Scrivania/BLO/src/models/mo.c

CMakeFiles/blo.dir/src/models/mo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/models/mo.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/models/mo.c > CMakeFiles/blo.dir/src/models/mo.c.i

CMakeFiles/blo.dir/src/models/mo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/models/mo.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/models/mo.c -o CMakeFiles/blo.dir/src/models/mo.c.s

CMakeFiles/blo.dir/src/models/m2sb.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/models/m2sb.c.o: ../src/models/m2sb.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/blo.dir/src/models/m2sb.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/models/m2sb.c.o -c /home/leonardo/Scrivania/BLO/src/models/m2sb.c

CMakeFiles/blo.dir/src/models/m2sb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/models/m2sb.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/models/m2sb.c > CMakeFiles/blo.dir/src/models/m2sb.c.i

CMakeFiles/blo.dir/src/models/m2sb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/models/m2sb.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/models/m2sb.c -o CMakeFiles/blo.dir/src/models/m2sb.c.s

CMakeFiles/blo.dir/src/models/m3.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/models/m3.c.o: ../src/models/m3.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/blo.dir/src/models/m3.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/models/m3.c.o -c /home/leonardo/Scrivania/BLO/src/models/m3.c

CMakeFiles/blo.dir/src/models/m3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/models/m3.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/models/m3.c > CMakeFiles/blo.dir/src/models/m3.c.i

CMakeFiles/blo.dir/src/models/m3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/models/m3.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/models/m3.c -o CMakeFiles/blo.dir/src/models/m3.c.s

CMakeFiles/blo.dir/src/ul.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/ul.c.o: ../src/ul.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/blo.dir/src/ul.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/ul.c.o -c /home/leonardo/Scrivania/BLO/src/ul.c

CMakeFiles/blo.dir/src/ul.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/ul.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/ul.c > CMakeFiles/blo.dir/src/ul.c.i

CMakeFiles/blo.dir/src/ul.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/ul.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/ul.c -o CMakeFiles/blo.dir/src/ul.c.s

CMakeFiles/blo.dir/src/pso.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/pso.c.o: ../src/pso.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/blo.dir/src/pso.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/pso.c.o -c /home/leonardo/Scrivania/BLO/src/pso.c

CMakeFiles/blo.dir/src/pso.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/pso.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/pso.c > CMakeFiles/blo.dir/src/pso.c.i

CMakeFiles/blo.dir/src/pso.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/pso.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/pso.c -o CMakeFiles/blo.dir/src/pso.c.s

CMakeFiles/blo.dir/src/sa.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/sa.c.o: ../src/sa.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object CMakeFiles/blo.dir/src/sa.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/sa.c.o -c /home/leonardo/Scrivania/BLO/src/sa.c

CMakeFiles/blo.dir/src/sa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/sa.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/sa.c > CMakeFiles/blo.dir/src/sa.c.i

CMakeFiles/blo.dir/src/sa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/sa.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/sa.c -o CMakeFiles/blo.dir/src/sa.c.s

CMakeFiles/blo.dir/src/ga.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/ga.c.o: ../src/ga.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building C object CMakeFiles/blo.dir/src/ga.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/ga.c.o -c /home/leonardo/Scrivania/BLO/src/ga.c

CMakeFiles/blo.dir/src/ga.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/ga.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/ga.c > CMakeFiles/blo.dir/src/ga.c.i

CMakeFiles/blo.dir/src/ga.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/ga.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/ga.c -o CMakeFiles/blo.dir/src/ga.c.s

CMakeFiles/blo.dir/src/lbs.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/lbs.c.o: ../src/lbs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building C object CMakeFiles/blo.dir/src/lbs.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/lbs.c.o -c /home/leonardo/Scrivania/BLO/src/lbs.c

CMakeFiles/blo.dir/src/lbs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/lbs.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/lbs.c > CMakeFiles/blo.dir/src/lbs.c.i

CMakeFiles/blo.dir/src/lbs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/lbs.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/lbs.c -o CMakeFiles/blo.dir/src/lbs.c.s

CMakeFiles/blo.dir/src/csa.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/csa.c.o: ../src/csa.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building C object CMakeFiles/blo.dir/src/csa.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/csa.c.o -c /home/leonardo/Scrivania/BLO/src/csa.c

CMakeFiles/blo.dir/src/csa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/csa.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/csa.c > CMakeFiles/blo.dir/src/csa.c.i

CMakeFiles/blo.dir/src/csa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/csa.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/csa.c -o CMakeFiles/blo.dir/src/csa.c.s

CMakeFiles/blo.dir/src/de.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/de.c.o: ../src/de.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building C object CMakeFiles/blo.dir/src/de.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/de.c.o -c /home/leonardo/Scrivania/BLO/src/de.c

CMakeFiles/blo.dir/src/de.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/de.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/de.c > CMakeFiles/blo.dir/src/de.c.i

CMakeFiles/blo.dir/src/de.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/de.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/de.c -o CMakeFiles/blo.dir/src/de.c.s

CMakeFiles/blo.dir/src/grasp.c.o: CMakeFiles/blo.dir/flags.make
CMakeFiles/blo.dir/src/grasp.c.o: ../src/grasp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Building C object CMakeFiles/blo.dir/src/grasp.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/blo.dir/src/grasp.c.o -c /home/leonardo/Scrivania/BLO/src/grasp.c

CMakeFiles/blo.dir/src/grasp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blo.dir/src/grasp.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/leonardo/Scrivania/BLO/src/grasp.c > CMakeFiles/blo.dir/src/grasp.c.i

CMakeFiles/blo.dir/src/grasp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blo.dir/src/grasp.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/leonardo/Scrivania/BLO/src/grasp.c -o CMakeFiles/blo.dir/src/grasp.c.s

# Object files for target blo
blo_OBJECTS = \
"CMakeFiles/blo.dir/src/main.c.o" \
"CMakeFiles/blo.dir/src/ll.c.o" \
"CMakeFiles/blo.dir/src/plot.c.o" \
"CMakeFiles/blo.dir/src/read_input.c.o" \
"CMakeFiles/blo.dir/src/utils.c.o" \
"CMakeFiles/blo.dir/src/models/m1a.c.o" \
"CMakeFiles/blo.dir/src/models/m1.c.o" \
"CMakeFiles/blo.dir/src/models/m2ewh.c.o" \
"CMakeFiles/blo.dir/src/models/m2ev.c.o" \
"CMakeFiles/blo.dir/src/models/mo.c.o" \
"CMakeFiles/blo.dir/src/models/m2sb.c.o" \
"CMakeFiles/blo.dir/src/models/m3.c.o" \
"CMakeFiles/blo.dir/src/ul.c.o" \
"CMakeFiles/blo.dir/src/pso.c.o" \
"CMakeFiles/blo.dir/src/sa.c.o" \
"CMakeFiles/blo.dir/src/ga.c.o" \
"CMakeFiles/blo.dir/src/lbs.c.o" \
"CMakeFiles/blo.dir/src/csa.c.o" \
"CMakeFiles/blo.dir/src/de.c.o" \
"CMakeFiles/blo.dir/src/grasp.c.o"

# External object files for target blo
blo_EXTERNAL_OBJECTS =

blo: CMakeFiles/blo.dir/src/main.c.o
blo: CMakeFiles/blo.dir/src/ll.c.o
blo: CMakeFiles/blo.dir/src/plot.c.o
blo: CMakeFiles/blo.dir/src/read_input.c.o
blo: CMakeFiles/blo.dir/src/utils.c.o
blo: CMakeFiles/blo.dir/src/models/m1a.c.o
blo: CMakeFiles/blo.dir/src/models/m1.c.o
blo: CMakeFiles/blo.dir/src/models/m2ewh.c.o
blo: CMakeFiles/blo.dir/src/models/m2ev.c.o
blo: CMakeFiles/blo.dir/src/models/mo.c.o
blo: CMakeFiles/blo.dir/src/models/m2sb.c.o
blo: CMakeFiles/blo.dir/src/models/m3.c.o
blo: CMakeFiles/blo.dir/src/ul.c.o
blo: CMakeFiles/blo.dir/src/pso.c.o
blo: CMakeFiles/blo.dir/src/sa.c.o
blo: CMakeFiles/blo.dir/src/ga.c.o
blo: CMakeFiles/blo.dir/src/lbs.c.o
blo: CMakeFiles/blo.dir/src/csa.c.o
blo: CMakeFiles/blo.dir/src/de.c.o
blo: CMakeFiles/blo.dir/src/grasp.c.o
blo: CMakeFiles/blo.dir/build.make
blo: CMakeFiles/blo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Linking C executable blo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/blo.dir/build: blo
.PHONY : CMakeFiles/blo.dir/build

CMakeFiles/blo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/blo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/blo.dir/clean

CMakeFiles/blo.dir/depend:
	cd /home/leonardo/Scrivania/BLO/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/leonardo/Scrivania/BLO /home/leonardo/Scrivania/BLO /home/leonardo/Scrivania/BLO/cmake-build-debug /home/leonardo/Scrivania/BLO/cmake-build-debug /home/leonardo/Scrivania/BLO/cmake-build-debug/CMakeFiles/blo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/blo.dir/depend

