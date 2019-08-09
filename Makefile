# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/safronov/workspace/graphite/carbontestcpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/safronov/workspace/graphite/carbontestcpp

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target test
test:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running tests..."
	/usr/bin/ctest --force-new-ctest-process $(ARGS)
.PHONY : test

# Special rule for the target test
test/fast: test

.PHONY : test/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/safronov/workspace/graphite/carbontestcpp/CMakeFiles /home/safronov/workspace/graphite/carbontestcpp/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/safronov/workspace/graphite/carbontestcpp/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named carbontestcpp

# Build rule for target.
carbontestcpp: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 carbontestcpp
.PHONY : carbontestcpp

# fast build rule for target.
carbontestcpp/fast:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/build
.PHONY : carbontestcpp/fast

#=============================================================================
# Target rules for targets named benchmarks

# Build rule for target.
benchmarks: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 benchmarks
.PHONY : benchmarks

# fast build rule for target.
benchmarks/fast:
	$(MAKE) -f CMakeFiles/benchmarks.dir/build.make CMakeFiles/benchmarks.dir/build
.PHONY : benchmarks/fast

#=============================================================================
# Target rules for targets named clang-tidy

# Build rule for target.
clang-tidy: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 clang-tidy
.PHONY : clang-tidy

# fast build rule for target.
clang-tidy/fast:
	$(MAKE) -f CMakeFiles/clang-tidy.dir/build.make CMakeFiles/clang-tidy.dir/build
.PHONY : clang-tidy/fast

#=============================================================================
# Target rules for targets named cppcheck

# Build rule for target.
cppcheck: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 cppcheck
.PHONY : cppcheck

# fast build rule for target.
cppcheck/fast:
	$(MAKE) -f CMakeFiles/cppcheck.dir/build.make CMakeFiles/cppcheck.dir/build
.PHONY : cppcheck/fast

src/client.o: src/client.cpp.o

.PHONY : src/client.o

# target to build an object file
src/client.cpp.o:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/client.cpp.o
.PHONY : src/client.cpp.o

src/client.i: src/client.cpp.i

.PHONY : src/client.i

# target to preprocess a source file
src/client.cpp.i:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/client.cpp.i
.PHONY : src/client.cpp.i

src/client.s: src/client.cpp.s

.PHONY : src/client.s

# target to generate assembly for a file
src/client.cpp.s:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/client.cpp.s
.PHONY : src/client.cpp.s

src/config.o: src/config.cpp.o

.PHONY : src/config.o

# target to build an object file
src/config.cpp.o:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/config.cpp.o
.PHONY : src/config.cpp.o

src/config.i: src/config.cpp.i

.PHONY : src/config.i

# target to preprocess a source file
src/config.cpp.i:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/config.cpp.i
.PHONY : src/config.cpp.i

src/config.s: src/config.cpp.s

.PHONY : src/config.s

# target to generate assembly for a file
src/config.cpp.s:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/config.cpp.s
.PHONY : src/config.cpp.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/netstat.o: src/netstat.cpp.o

.PHONY : src/netstat.o

# target to build an object file
src/netstat.cpp.o:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/netstat.cpp.o
.PHONY : src/netstat.cpp.o

src/netstat.i: src/netstat.cpp.i

.PHONY : src/netstat.i

# target to preprocess a source file
src/netstat.cpp.i:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/netstat.cpp.i
.PHONY : src/netstat.cpp.i

src/netstat.s: src/netstat.cpp.s

.PHONY : src/netstat.s

# target to generate assembly for a file
src/netstat.cpp.s:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/netstat.cpp.s
.PHONY : src/netstat.cpp.s

src/runner.o: src/runner.cpp.o

.PHONY : src/runner.o

# target to build an object file
src/runner.cpp.o:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/runner.cpp.o
.PHONY : src/runner.cpp.o

src/runner.i: src/runner.cpp.i

.PHONY : src/runner.i

# target to preprocess a source file
src/runner.cpp.i:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/runner.cpp.i
.PHONY : src/runner.cpp.i

src/runner.s: src/runner.cpp.s

.PHONY : src/runner.s

# target to generate assembly for a file
src/runner.cpp.s:
	$(MAKE) -f CMakeFiles/carbontestcpp.dir/build.make CMakeFiles/carbontestcpp.dir/src/runner.cpp.s
.PHONY : src/runner.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... install"
	@echo "... install/strip"
	@echo "... carbontestcpp"
	@echo "... benchmarks"
	@echo "... rebuild_cache"
	@echo "... clang-tidy"
	@echo "... list_install_components"
	@echo "... cppcheck"
	@echo "... install/local"
	@echo "... test"
	@echo "... edit_cache"
	@echo "... src/client.o"
	@echo "... src/client.i"
	@echo "... src/client.s"
	@echo "... src/config.o"
	@echo "... src/config.i"
	@echo "... src/config.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/netstat.o"
	@echo "... src/netstat.i"
	@echo "... src/netstat.s"
	@echo "... src/runner.o"
	@echo "... src/runner.i"
	@echo "... src/runner.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

