﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("theSOM")
find_package( OpenGL REQUIRED )
# Include sub-projects.
add_subdirectory ("theSOM")
add_subdirectory ("3rdParty")
