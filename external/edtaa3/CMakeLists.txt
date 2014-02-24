cmake_minimum_required(VERSION 2.8)

# Declare the project
project(edtaa3)

# set output path:
set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(.)

#Create src file list
set(src_files
	edtaa3/edtaa3func.c
)

add_definitions( -DDEBUG_LEVEL=3 )
add_definitions( -DDEBUG=1 )

#Create a static Lib:
add_library(edtaa3 STATIC ${src_files} )


