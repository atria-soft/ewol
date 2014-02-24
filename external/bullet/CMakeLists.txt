cmake_minimum_required(VERSION 2.8)

# Declare the project
project(linearmath)

# set output path:
set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_DIR}/bullet/src )


SET(LinearMath_SRCS
	bullet/src/LinearMath/btAlignedAllocator.cpp
	bullet/src/LinearMath/btConvexHull.cpp
	bullet/src/LinearMath/btConvexHullComputer.cpp
	bullet/src/LinearMath/btGeometryUtil.cpp
	bullet/src/LinearMath/btPolarDecomposition.cpp
	bullet/src/LinearMath/btQuickprof.cpp
	bullet/src/LinearMath/btSerializer.cpp
	bullet/src/LinearMath/btVector3.cpp
)

SET(LinearMath_HDRS
	bullet/src/LinearMath/btAabbUtil2.h
	bullet/src/LinearMath/btAlignedAllocator.h
	bullet/src/LinearMath/btAlignedObjectArray.h
	bullet/src/LinearMath/btConvexHull.h
	bullet/src/LinearMath/btConvexHullComputer.h
	bullet/src/LinearMath/btDefaultMotionState.h
	bullet/src/LinearMath/btGeometryUtil.h
	bullet/src/LinearMath/btGrahamScan2dConvexHull.h
	bullet/src/LinearMath/btHashMap.h
	bullet/src/LinearMath/btIDebugDraw.h
	bullet/src/LinearMath/btList.h
	bullet/src/LinearMath/btMatrix3x3.h
	bullet/src/LinearMath/btMinMax.h
	bullet/src/LinearMath/btMotionState.h
	bullet/src/LinearMath/btPolarDecomposition.h
	bullet/src/LinearMath/btPoolAllocator.h
	bullet/src/LinearMath/btQuadWord.h
	bullet/src/LinearMath/btQuaternion.h
	bullet/src/LinearMath/btQuickprof.h
	bullet/src/LinearMath/btRandom.h
	bullet/src/LinearMath/btScalar.h
	bullet/src/LinearMath/btSerializer.h
	bullet/src/LinearMath/btStackAlloc.h
	bullet/src/LinearMath/btTransform.h
	bullet/src/LinearMath/btTransformUtil.h
	bullet/src/LinearMath/btVector3.h
)

#Create a static Lib:
#ADD_LIBRARY(LinearMath STATIC ${LinearMath_SRCS} ${LinearMath_HDRS})
#Create a static Lib:
add_library(linearmath STATIC ${LinearMath_SRCS} ${LinearMath_HDRS} )


#include_directories(${CMAKE_CURRENT_DIR}/bullet/src)

#set(linearmath_INCLUDE_DIR "bullet/src")
#include_directories(${CMAKE_CURRENT_DIR}/bullet/src)
#link_directories(${CMAKE_CURRENT_DIR}/bullet/src)

#message(STATUS "linearmath_INCLUDE_DIR=${linearmath_INCLUDE_DIR}")