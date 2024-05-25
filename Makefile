# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_SOURCE_DIR = /home/dom/alice/qt/ollama_gui/OllamaGui/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dom/alice/qt/ollama_gui

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dom/alice/qt/ollama_gui/CMakeFiles /home/dom/alice/qt/ollama_gui//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dom/alice/qt/ollama_gui/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named ollamagui

# Build rule for target.
ollamagui: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 ollamagui
.PHONY : ollamagui

# fast build rule for target.
ollamagui/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/build
.PHONY : ollamagui/fast

#=============================================================================
# Target rules for targets named ollamagui_autogen_timestamp_deps

# Build rule for target.
ollamagui_autogen_timestamp_deps: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 ollamagui_autogen_timestamp_deps
.PHONY : ollamagui_autogen_timestamp_deps

# fast build rule for target.
ollamagui_autogen_timestamp_deps/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui_autogen_timestamp_deps.dir/build.make CMakeFiles/ollamagui_autogen_timestamp_deps.dir/build
.PHONY : ollamagui_autogen_timestamp_deps/fast

#=============================================================================
# Target rules for targets named ollamagui_autogen

# Build rule for target.
ollamagui_autogen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 ollamagui_autogen
.PHONY : ollamagui_autogen

# fast build rule for target.
ollamagui_autogen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui_autogen.dir/build.make CMakeFiles/ollamagui_autogen.dir/build
.PHONY : ollamagui_autogen/fast

Widget.o: Widget.cpp.o
.PHONY : Widget.o

# target to build an object file
Widget.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/Widget.cpp.o
.PHONY : Widget.cpp.o

Widget.i: Widget.cpp.i
.PHONY : Widget.i

# target to preprocess a source file
Widget.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/Widget.cpp.i
.PHONY : Widget.cpp.i

Widget.s: Widget.cpp.s
.PHONY : Widget.s

# target to generate assembly for a file
Widget.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/Widget.cpp.s
.PHONY : Widget.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/main.cpp.s
.PHONY : main.cpp.s

ollamagui_autogen/mocs_compilation.o: ollamagui_autogen/mocs_compilation.cpp.o
.PHONY : ollamagui_autogen/mocs_compilation.o

# target to build an object file
ollamagui_autogen/mocs_compilation.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/ollamagui_autogen/mocs_compilation.cpp.o
.PHONY : ollamagui_autogen/mocs_compilation.cpp.o

ollamagui_autogen/mocs_compilation.i: ollamagui_autogen/mocs_compilation.cpp.i
.PHONY : ollamagui_autogen/mocs_compilation.i

# target to preprocess a source file
ollamagui_autogen/mocs_compilation.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/ollamagui_autogen/mocs_compilation.cpp.i
.PHONY : ollamagui_autogen/mocs_compilation.cpp.i

ollamagui_autogen/mocs_compilation.s: ollamagui_autogen/mocs_compilation.cpp.s
.PHONY : ollamagui_autogen/mocs_compilation.s

# target to generate assembly for a file
ollamagui_autogen/mocs_compilation.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ollamagui.dir/build.make CMakeFiles/ollamagui.dir/ollamagui_autogen/mocs_compilation.cpp.s
.PHONY : ollamagui_autogen/mocs_compilation.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... ollamagui_autogen"
	@echo "... ollamagui_autogen_timestamp_deps"
	@echo "... ollamagui"
	@echo "... Widget.o"
	@echo "... Widget.i"
	@echo "... Widget.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... ollamagui_autogen/mocs_compilation.o"
	@echo "... ollamagui_autogen/mocs_compilation.i"
	@echo "... ollamagui_autogen/mocs_compilation.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
