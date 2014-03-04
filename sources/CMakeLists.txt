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
	ewol/ewol.h
	ewol/debug.cpp
	ewol/debug.h
	ewol/Dimension.cpp
	ewol/Dimension.h
	ewol/compositing/Compositing.cpp
	ewol/compositing/Compositing.h
	ewol/compositing/TextBase.cpp
	ewol/compositing/TextBase.h
	ewol/compositing/Text.cpp
	ewol/compositing/Text.h
	ewol/compositing/TextDF.cpp
	ewol/compositing/TextDF.h
	ewol/compositing/Drawing.cpp
	ewol/compositing/Drawing.h
	ewol/compositing/Image.cpp
	ewol/compositing/Image.h
	ewol/compositing/Sprite.cpp
	ewol/compositing/Sprite.h
	ewol/compositing/Shaper.cpp
	ewol/compositing/Shaper.h
	ewol/compositing/Area.cpp
	ewol/compositing/Area.h
	ewol/context/clipBoard.cpp
	ewol/context/clipBoard.h
	ewol/context/commandLine.cpp
	ewol/context/commandLine.h
	ewol/context/ConfigFont.cpp
	ewol/context/ConfigFont.h
	ewol/context/Context.cpp
	ewol/context/Context.h
	ewol/context/cursor.cpp
	ewol/context/cursor.h
	ewol/context/InputManager.cpp
	ewol/context/InputManager.h
	ewol/event/Entry.cpp
	ewol/event/Entry.h
	ewol/event/Time.cpp
	ewol/event/Time.h
	ewol/event/Input.cpp
	ewol/event/Input.h
	ewol/key/keyboard.cpp
	ewol/key/keyboard.h
	ewol/key/Special.cpp
	ewol/key/Special.h
	ewol/key/status.cpp
	ewol/key/status.h
	ewol/key/type.cpp
	ewol/key/type.h
	ewol/object/Config.cpp
	ewol/object/Config.h
	ewol/object/ConfigElement.cpp
	ewol/object/ConfigElement.h
	ewol/object/Manager.cpp
	ewol/object/Manager.h
	ewol/object/Message.cpp
	ewol/object/Message.h
	ewol/object/MultiCast.cpp
	ewol/object/MultiCast.h
	ewol/object/Object.cpp
	ewol/object/Object.h
	ewol/openGL/openGL.cpp
	ewol/openGL/openGL.h
	ewol/resource/Colored3DObject.cpp
	ewol/resource/Colored3DObject.h
	ewol/resource/ColorFile.cpp
	ewol/resource/ColorFile.h
	ewol/resource/ConfigFile.cpp
	ewol/resource/ConfigFile.h
	ewol/resource/FontFreeType.cpp
	ewol/resource/FontFreeType.h
	ewol/resource/Image.cpp
	ewol/resource/Image.h
	ewol/resource/ImageDF.cpp
	ewol/resource/ImageDF.h
	ewol/resource/Manager.cpp
	ewol/resource/Manager.h
	ewol/resource/Program.cpp
	ewol/resource/Program.h
	ewol/resource/Resource.cpp
	ewol/resource/Resource.h
	ewol/resource/Shader.cpp
	ewol/resource/Shader.h
	ewol/resource/Texture.cpp
	ewol/resource/Texture.h
	ewol/resource/TexturedFont.cpp
	ewol/resource/TexturedFont.h
	ewol/resource/DistanceFieldFont.cpp
	ewol/resource/DistanceFieldFont.h
	ewol/resource/VirtualBufferObject.cpp
	ewol/resource/VirtualBufferObject.h
	ewol/widget/ButtonColor.cpp
	ewol/widget/ButtonColor.h
	ewol/widget/Button.cpp
	ewol/widget/Button.h
	ewol/widget/CheckBox.cpp
	ewol/widget/CheckBox.h
	ewol/widget/ColorBar.cpp
	ewol/widget/ColorBar.h
	ewol/widget/Composer.cpp
	ewol/widget/Composer.h
	ewol/widget/Container.cpp
	ewol/widget/Container.h
	ewol/widget/Container2.cpp
	ewol/widget/Container2.h
	ewol/widget/ContainerN.cpp
	ewol/widget/ContainerN.h
	ewol/widget/ContextMenu.cpp
	ewol/widget/ContextMenu.h
	ewol/widget/Entry.cpp
	ewol/widget/Entry.h
	ewol/widget/Gird.cpp
	ewol/widget/Gird.h
	ewol/widget/Image.cpp
	ewol/widget/Image.h
	ewol/widget/Joystick.cpp
	ewol/widget/Joystick.h
	ewol/widget/Label.cpp
	ewol/widget/Label.h
	ewol/widget/Layer.cpp
	ewol/widget/Layer.h
	ewol/widget/List.cpp
	ewol/widget/List.h
	ewol/widget/ListFileSystem.cpp
	ewol/widget/ListFileSystem.h
	ewol/widget/Manager.cpp
	ewol/widget/Manager.h
	ewol/widget/Menu.cpp
	ewol/widget/Menu.h
	ewol/widget/meta/ColorChooser.cpp
	ewol/widget/meta/ColorChooser.h
	ewol/widget/meta/FileChooser.cpp
	ewol/widget/meta/FileChooser.h
	ewol/widget/meta/Parameter.cpp
	ewol/widget/meta/Parameter.h
	ewol/widget/meta/ParameterList.cpp
	ewol/widget/meta/ParameterList.h
	ewol/widget/meta/StdPopUp.cpp
	ewol/widget/meta/StdPopUp.h
	ewol/widget/PopUp.cpp
	ewol/widget/PopUp.h
	ewol/widget/ProgressBar.cpp
	ewol/widget/ProgressBar.h
	ewol/widget/Scroll.cpp
	ewol/widget/Scroll.h
	ewol/widget/Sizer.cpp
	ewol/widget/Sizer.h
	ewol/widget/Slider.cpp
	ewol/widget/Slider.h
	ewol/widget/Spacer.cpp
	ewol/widget/Spacer.h
	ewol/widget/Widget.cpp
	ewol/widget/Widget.h
	ewol/widget/WidgetScrolled.cpp
	ewol/widget/WidgetScrolled.h
	ewol/widget/Windows.cpp
	ewol/widget/Windows.h
	ewol/widget/WSlider.cpp
	ewol/widget/WSlider.h
)

add_definitions( -DDEBUG_LEVEL=3 )
add_definitions( -DDEBUG=1 )
message(STATUS "APPLE=${APPLE}")
message(STATUS "UNIX=${UNIX}")

if (APPLE)
	add_definitions( -D__TARGET_OS__MacOs )
	set(src_files-specific
		ewol/context/MacOs/Context.cpp
		ewol/context/MacOs/Context.h
		ewol/context/MacOs/Interface.mm
		ewol/context/MacOs/Interface.h
		ewol/context/MacOs/Windows.mm
		ewol/context/MacOs/Windows.h
		ewol/context/MacOs/OpenglView.mm
		ewol/context/MacOs/OpenglView.h
		ewol/context/MacOs/AppDelegate.mm
		ewol/context/MacOs/AppDelegate.h
	)
elseif (UNIX)
	add_definitions( -D__TARGET_OS__Linux )
	set(src_files-specific
		ewol/context/X11/Context.cpp
		ewol/context/X11/Context.h
	)
elseif (WIN32)
	add_definitions( -D__TARGET_OS__Windows )
	set(src_files-specific
		ewol/context/Windows/Context.cpp
		ewol/context/Windows/Context.h
	)
endif ()

#Create a static Lib:
add_library(ewol STATIC ${src_files} ${src_files-specific})

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

# display all variable ...
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
