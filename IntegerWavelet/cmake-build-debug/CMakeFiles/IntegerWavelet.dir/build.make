# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/IntegerWavelet.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/IntegerWavelet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/IntegerWavelet.dir/flags.make

CMakeFiles/IntegerWavelet.dir/main.cpp.o: CMakeFiles/IntegerWavelet.dir/flags.make
CMakeFiles/IntegerWavelet.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/IntegerWavelet.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/IntegerWavelet.dir/main.cpp.o -c /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/main.cpp

CMakeFiles/IntegerWavelet.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IntegerWavelet.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/main.cpp > CMakeFiles/IntegerWavelet.dir/main.cpp.i

CMakeFiles/IntegerWavelet.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IntegerWavelet.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/main.cpp -o CMakeFiles/IntegerWavelet.dir/main.cpp.s

CMakeFiles/IntegerWavelet.dir/inthaar2.cc.o: CMakeFiles/IntegerWavelet.dir/flags.make
CMakeFiles/IntegerWavelet.dir/inthaar2.cc.o: ../inthaar2.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/IntegerWavelet.dir/inthaar2.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/IntegerWavelet.dir/inthaar2.cc.o -c /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/inthaar2.cc

CMakeFiles/IntegerWavelet.dir/inthaar2.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IntegerWavelet.dir/inthaar2.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/inthaar2.cc > CMakeFiles/IntegerWavelet.dir/inthaar2.cc.i

CMakeFiles/IntegerWavelet.dir/inthaar2.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IntegerWavelet.dir/inthaar2.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/inthaar2.cc -o CMakeFiles/IntegerWavelet.dir/inthaar2.cc.s

# Object files for target IntegerWavelet
IntegerWavelet_OBJECTS = \
"CMakeFiles/IntegerWavelet.dir/main.cpp.o" \
"CMakeFiles/IntegerWavelet.dir/inthaar2.cc.o"

# External object files for target IntegerWavelet
IntegerWavelet_EXTERNAL_OBJECTS =

IntegerWavelet: CMakeFiles/IntegerWavelet.dir/main.cpp.o
IntegerWavelet: CMakeFiles/IntegerWavelet.dir/inthaar2.cc.o
IntegerWavelet: CMakeFiles/IntegerWavelet.dir/build.make
IntegerWavelet: /opt/local/lib/libopencv_dnn.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_highgui.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_ml.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_objdetect.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_shape.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_stitching.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_superres.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_videostab.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_calib3d.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_features2d.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_flann.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_photo.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_video.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_videoio.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_imgcodecs.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_imgproc.3.4.8.dylib
IntegerWavelet: /opt/local/lib/libopencv_core.3.4.8.dylib
IntegerWavelet: CMakeFiles/IntegerWavelet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable IntegerWavelet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/IntegerWavelet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/IntegerWavelet.dir/build: IntegerWavelet

.PHONY : CMakeFiles/IntegerWavelet.dir/build

CMakeFiles/IntegerWavelet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/IntegerWavelet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/IntegerWavelet.dir/clean

CMakeFiles/IntegerWavelet.dir/depend:
	cd /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug /Users/Chasel/2020Spring/High_Performance_Computing/Project/HPC_PROJECT_WENTAO/SourceCodes/IntegerWavelet/cmake-build-debug/CMakeFiles/IntegerWavelet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/IntegerWavelet.dir/depend
