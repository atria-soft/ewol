cmake_minimum_required(VERSION 2.8)

# Declare the project
project(date)

##
## Include C++ X11 dependency ... (check correct flags)
##
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
endif()


# set output path:
set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(.)

#Create src file list
set(src_files
	date/date.cpp
)

add_definitions( -DDEBUG_LEVEL=3 )
add_definitions( -DDEBUG=1 )

if (WIN32)
	execute_process(COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE RESULT)
	#string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
	message(STATUS "Current date : ${RESULT}")
	add_definitions( -DBUILD_DAY=0 )
	add_definitions( -DBUILD_YEAR=0 )
	add_definitions( -DBUILD_MONTH=0 )
else ()
	if (UNIX)
		execute_process(COMMAND "date" "+%d" OUTPUT_VARIABLE RESULT_DAY)
		execute_process(COMMAND "date" "+%m" OUTPUT_VARIABLE RESULT_MONTH)
		execute_process(COMMAND "date" "+%Y" OUTPUT_VARIABLE RESULT_YEAR)
		
		add_definitions( -DBUILD_DAY=0 )
		add_definitions( -DBUILD_YEAR=0 )
		add_definitions( -DBUILD_MONTH=0 )
		
		#string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
		message(STATUS "Current date : ${RESULT_DAY} / ${RESULT_MONTH} / ${RESULT_YEAR}")
	else()
		message(SEND_ERROR "date not implemented")
		add_definitions( -DBUILD_DAY=0 )
		add_definitions( -DBUILD_YEAR=0 )
		add_definitions( -DBUILD_MONTH=0 )
	endif()
endif()

add_definitions( -DBUILD_HOUR=0 )
add_definitions( -DBUILD_MINUTE=0 )
add_definitions( -DBUILD_SECOND=0 )

#Create a static Lib:
add_library(date STATIC ${src_files} )


