# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\alamo\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\alamo\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = X:\1Third_Year\CourseProject_StealMyKeyboard

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = X:\1Third_Year\CourseProject_StealMyKeyboard\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SP_CW.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SP_CW.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SP_CW.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SP_CW.dir/flags.make

CMakeFiles/SP_CW.dir/main.cpp.obj: CMakeFiles/SP_CW.dir/flags.make
CMakeFiles/SP_CW.dir/main.cpp.obj: X:/1Third_Year/CourseProject_StealMyKeyboard/main.cpp
CMakeFiles/SP_CW.dir/main.cpp.obj: CMakeFiles/SP_CW.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=X:\1Third_Year\CourseProject_StealMyKeyboard\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SP_CW.dir/main.cpp.obj"
	C:\Users\alamo\AppData\Local\Programs\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SP_CW.dir/main.cpp.obj -MF CMakeFiles\SP_CW.dir\main.cpp.obj.d -o CMakeFiles\SP_CW.dir\main.cpp.obj -c X:\1Third_Year\CourseProject_StealMyKeyboard\main.cpp

CMakeFiles/SP_CW.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SP_CW.dir/main.cpp.i"
	C:\Users\alamo\AppData\Local\Programs\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E X:\1Third_Year\CourseProject_StealMyKeyboard\main.cpp > CMakeFiles\SP_CW.dir\main.cpp.i

CMakeFiles/SP_CW.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SP_CW.dir/main.cpp.s"
	C:\Users\alamo\AppData\Local\Programs\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S X:\1Third_Year\CourseProject_StealMyKeyboard\main.cpp -o CMakeFiles\SP_CW.dir\main.cpp.s

# Object files for target SP_CW
SP_CW_OBJECTS = \
"CMakeFiles/SP_CW.dir/main.cpp.obj"

# External object files for target SP_CW
SP_CW_EXTERNAL_OBJECTS =

SP_CW.exe: CMakeFiles/SP_CW.dir/main.cpp.obj
SP_CW.exe: CMakeFiles/SP_CW.dir/build.make
SP_CW.exe: CMakeFiles/SP_CW.dir/linkLibs.rsp
SP_CW.exe: CMakeFiles/SP_CW.dir/objects1.rsp
SP_CW.exe: CMakeFiles/SP_CW.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=X:\1Third_Year\CourseProject_StealMyKeyboard\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SP_CW.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\SP_CW.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SP_CW.dir/build: SP_CW.exe
.PHONY : CMakeFiles/SP_CW.dir/build

CMakeFiles/SP_CW.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SP_CW.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SP_CW.dir/clean

CMakeFiles/SP_CW.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" X:\1Third_Year\CourseProject_StealMyKeyboard X:\1Third_Year\CourseProject_StealMyKeyboard X:\1Third_Year\CourseProject_StealMyKeyboard\cmake-build-debug X:\1Third_Year\CourseProject_StealMyKeyboard\cmake-build-debug X:\1Third_Year\CourseProject_StealMyKeyboard\cmake-build-debug\CMakeFiles\SP_CW.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/SP_CW.dir/depend

