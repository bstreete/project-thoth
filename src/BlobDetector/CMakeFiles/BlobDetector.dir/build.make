# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/carinaj/project-thoth/src/BlobDetector

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/carinaj/project-thoth/src/BlobDetector

# Include any dependencies generated for this target.
include CMakeFiles/BlobDetector.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/BlobDetector.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BlobDetector.dir/flags.make

CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o: CMakeFiles/BlobDetector.dir/flags.make
CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o: BlobDetector.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/carinaj/project-thoth/src/BlobDetector/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o -c /Users/carinaj/project-thoth/src/BlobDetector/BlobDetector.cpp

CMakeFiles/BlobDetector.dir/BlobDetector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BlobDetector.dir/BlobDetector.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/carinaj/project-thoth/src/BlobDetector/BlobDetector.cpp > CMakeFiles/BlobDetector.dir/BlobDetector.cpp.i

CMakeFiles/BlobDetector.dir/BlobDetector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BlobDetector.dir/BlobDetector.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/carinaj/project-thoth/src/BlobDetector/BlobDetector.cpp -o CMakeFiles/BlobDetector.dir/BlobDetector.cpp.s

CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.requires:
.PHONY : CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.requires

CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.provides: CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.requires
	$(MAKE) -f CMakeFiles/BlobDetector.dir/build.make CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.provides.build
.PHONY : CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.provides

CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.provides.build: CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o

# Object files for target BlobDetector
BlobDetector_OBJECTS = \
"CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o"

# External object files for target BlobDetector
BlobDetector_EXTERNAL_OBJECTS =

BlobDetector: CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o
BlobDetector: CMakeFiles/BlobDetector.dir/build.make
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_videostab.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_ts.a
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_superres.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_stitching.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_contrib.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_nonfree.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_gpu.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_photo.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_objdetect.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_legacy.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_video.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_ml.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_calib3d.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_features2d.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_highgui.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_imgproc.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_flann.2.4.10.dylib
BlobDetector: /usr/local/opencv-2.4.10/lib/libopencv_core.2.4.10.dylib
BlobDetector: CMakeFiles/BlobDetector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable BlobDetector"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BlobDetector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BlobDetector.dir/build: BlobDetector
.PHONY : CMakeFiles/BlobDetector.dir/build

CMakeFiles/BlobDetector.dir/requires: CMakeFiles/BlobDetector.dir/BlobDetector.cpp.o.requires
.PHONY : CMakeFiles/BlobDetector.dir/requires

CMakeFiles/BlobDetector.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BlobDetector.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BlobDetector.dir/clean

CMakeFiles/BlobDetector.dir/depend:
	cd /Users/carinaj/project-thoth/src/BlobDetector && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/carinaj/project-thoth/src/BlobDetector /Users/carinaj/project-thoth/src/BlobDetector /Users/carinaj/project-thoth/src/BlobDetector /Users/carinaj/project-thoth/src/BlobDetector /Users/carinaj/project-thoth/src/BlobDetector/CMakeFiles/BlobDetector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BlobDetector.dir/depend

