cmake_minimum_required(VERSION 2.6)

# Declare the project
project(bulletlib)

# set output path:
set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(bullet/src)

#Create src file list
set(src_files
	bullet/src/LinearMath/btQuickprof.cpp
	bullet/src/LinearMath/btGeometryUtil.cpp
	bullet/src/LinearMath/btAlignedAllocator.cpp
	bullet/src/LinearMath/btSerializer.cpp
	bullet/src/LinearMath/btConvexHull.cpp
	bullet/src/LinearMath/btPolarDecomposition.cpp
	bullet/src/LinearMath/btVector3.cpp
	bullet/src/LinearMath/btConvexHullComputer.cpp
)

#add_definitions( -DDEBUG_LEVEL=3 )

#Create a static Lib:
add_library(linearmath STATIC ${src_files} )

