cmake_minimum_required(VERSION 2.8)

# Declare the project
project(ewol)

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
	ewol/ewol.cpp
	ewol/debug.cpp
	ewol/Dimension.cpp
	ewol/compositing/Compositing.cpp
	ewol/compositing/TextBase.cpp
	ewol/compositing/Text.cpp
	ewol/compositing/TextDF.cpp
	ewol/compositing/Drawing.cpp
	ewol/compositing/Image.cpp
	ewol/compositing/Sprite.cpp
	ewol/compositing/Shaper.cpp
	ewol/compositing/Area.cpp
	ewol/context/clipBoard.cpp
	ewol/context/commandLine.cpp
	ewol/context/ConfigFont.cpp
	ewol/context/Context.cpp
	ewol/context/cursor.cpp
	ewol/context/InputManager.cpp
	ewol/context/X11/Context.cpp
	ewol/event/Entry.cpp
	ewol/event/Time.cpp
	ewol/event/Input.cpp
	ewol/key/keyboard.cpp
	ewol/key/Special.cpp
	ewol/key/status.cpp
	ewol/key/type.cpp
	ewol/object/Config.cpp
	ewol/object/ConfigElement.cpp
	ewol/object/Manager.cpp
	ewol/object/Message.cpp
	ewol/object/MultiCast.cpp
	ewol/object/Object.cpp
	ewol/openGL/openGL.cpp
	ewol/resource/Colored3DObject.cpp
	ewol/resource/ColorFile.cpp
	ewol/resource/ConfigFile.cpp
	ewol/resource/FontFreeType.cpp
	ewol/resource/Image.cpp
	ewol/resource/ImageDF.cpp
	ewol/resource/Manager.cpp
	ewol/resource/Program.cpp
	ewol/resource/Resource.cpp
	ewol/resource/Shader.cpp
	ewol/resource/Texture.cpp
	ewol/resource/TexturedFont.cpp
	ewol/resource/DistanceFieldFont.cpp
	ewol/resource/VirtualBufferObject.cpp
	ewol/widget/ButtonColor.cpp
	ewol/widget/Button.cpp
	ewol/widget/CheckBox.cpp
	ewol/widget/ColorBar.cpp
	ewol/widget/Composer.cpp
	ewol/widget/Container.cpp
	ewol/widget/Container2.cpp
	ewol/widget/ContainerN.cpp
	ewol/widget/ContextMenu.cpp
	ewol/widget/Entry.cpp
	ewol/widget/Gird.cpp
	ewol/widget/Image.cpp
	ewol/widget/Joystick.cpp
	ewol/widget/Label.cpp
	ewol/widget/Layer.cpp
	ewol/widget/List.cpp
	ewol/widget/ListFileSystem.cpp
	ewol/widget/Manager.cpp
	ewol/widget/Menu.cpp
	ewol/widget/meta/ColorChooser.cpp
	ewol/widget/meta/FileChooser.cpp
	ewol/widget/meta/Parameter.cpp
	ewol/widget/meta/ParameterList.cpp
	ewol/widget/meta/StdPopUp.cpp
	ewol/widget/PopUp.cpp
	ewol/widget/ProgressBar.cpp
	ewol/widget/Scroll.cpp
	ewol/widget/Sizer.cpp
	ewol/widget/Slider.cpp
	ewol/widget/Spacer.cpp
	ewol/widget/Widget.cpp
	ewol/widget/WidgetScrolled.cpp
	ewol/widget/Windows.cpp
	ewol/widget/WSlider.cpp
)

add_definitions( -DDEBUG_LEVEL=3 )
add_definitions( -DDEBUG=1 )

#Create a static Lib:
add_library(ewol STATIC ${src_files} )

include_directories(${zlib_SOURCE_DIR}/contrib/)
include_directories(${linearmath_SOURCE_DIR}/bullet/src/)
include_directories(${etk_SOURCE_DIR})
include_directories(${freetype_SOURCE_DIR})
include_directories(${exml_SOURCE_DIR})
include_directories(${ejson_SOURCE_DIR})
include_directories(${egami_SOURCE_DIR})
include_directories(${edtaa3_SOURCE_DIR})
include_directories(${date_SOURCE_DIR})
include_directories(${agg_SOURCE_DIR})

#add_dependencies(ewol linearmath zlib etk freetype exml ejson egami edtaa3 date)

target_link_libraries(ewol linearmath zlib etk freetype exml ejson egami edtaa3 date)


# read version :
file (STRINGS "tag" BUILD_VERSION)
add_definitions( -DEWOL_VERSION="${BUILD_VERSION}" )
add_definitions( -D__TARGET_OS__Linux )

# display all variable ...
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
