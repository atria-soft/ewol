
# Basic elements
FILE_LIST = ewol/ewol.cpp \
			ewol/ClipBoard.cpp \
			ewol/Debug.cpp \
			ewol/ShortCutManager.cpp \
			ewol/ResourceManager.cpp

FILE_LIST+= ewol/openGL/openGL.cpp \
			ewol/openGL/Shader.cpp \
			ewol/openGL/Program.cpp \
			ewol/openGL/VirtualBufferObject.cpp


# Gui interface
FILE_LIST+= ewol/os/eSystem.cpp \
			ewol/os/eSystemInput.cpp

# Basic Eobject of EWOL
FILE_LIST+= ewol/eObject/EObject.cpp \
			ewol/eObject/EObjectManager.cpp

# Game elements
FILE_LIST+= ewol/game/GameElement.cpp \
			ewol/game/GameElementLua.cpp \
			ewol/game/SceneElement.cpp

# Object abstraction for OpenGl
FILE_LIST+= ewol/oObject/OObject.cpp \
			ewol/oObject/2DTextColored.cpp \
			ewol/oObject/2DTextShader.cpp \
			ewol/oObject/2DColored.cpp \
			ewol/oObject/2DTextured.cpp \
			ewol/oObject/3DTextured.cpp \
			ewol/oObject/Sprite.cpp

# texture management
FILE_LIST+= ewol/texture/Texture.cpp \
			ewol/texture/TextureFile.cpp \
			ewol/texture/TextureBMP.cpp \
			ewol/texture/TexturePNG.cpp \
			ewol/texture/TextureSVG.cpp

# fonst system
FILE_LIST+= ewol/font/FontManager.cpp \
			ewol/font/FontFreeType.cpp \
			ewol/font/TexturedFont.cpp \
			ewol/font/DistantFieldFont.cpp

# Mesh management
FILE_LIST+= ewol/Mesh/Mesh.cpp \
			ewol/Mesh/MeshObj.cpp


# all widgets
FILE_LIST+= ewol/widget/Widget.cpp \
			ewol/widget/WidgetManager.cpp \
			ewol/widget/Windows.cpp \
			ewol/widget/Button.cpp \
			ewol/widget/ButtonImage.cpp \
			ewol/widget/Image.cpp \
			ewol/widget/ButtonColor.cpp \
			ewol/widget/CheckBox.cpp \
			ewol/widget/ColorBar.cpp \
			ewol/widget/ContextMenu.cpp \
			ewol/widget/Drawable.cpp \
			ewol/widget/Entry.cpp \
			ewol/widget/Joystick.cpp \
			ewol/widget/Label.cpp \
			ewol/widget/List.cpp \
			ewol/widget/ListFileSystem.cpp \
			ewol/widget/Layer.cpp \
			ewol/widget/Menu.cpp \
			ewol/widget/PopUp.cpp \
			ewol/widget/ProgressBar.cpp \
			ewol/widget/Scene.cpp \
			ewol/widget/SizerHori.cpp \
			ewol/widget/SizerVert.cpp \
			ewol/widget/Slider.cpp \
			ewol/widget/WSlider.cpp \
			ewol/widget/Spacer.cpp \
			ewol/widget/WidgetScrolled.cpp \
			ewol/widget/meta/FileChooser.cpp \
			ewol/widget/meta/ColorChooser.cpp \
			ewol/widget/meta/Parameter.cpp \
			ewol/widget/meta/ParameterList.cpp

# Audio system
FILE_LIST+= ewol/audio/audio.cpp \
			ewol/audio/decWav.cpp


LOCAL_COPY_FILES := ../../share/textured3D.prog:textured3D.prog \
                    ../../share/textured3D.frag:textured3D.frag \
                    ../../share/textured3D.vert:textured3D.vert \
                    \
                    ../../share/color.prog:color.prog \
                    ../../share/color.frag:color.frag \
                    ../../share/color.vert:color.vert \
                    \
                    ../../share/textured.prog:textured.prog \
                    ../../share/textured.frag:textured.frag \
                    ../../share/textured.vert:textured.vert \
                    \
                    ../../share/theme/default/widgetEntry.prog:theme/default/widgetEntry.prog \
                    ../../share/theme/default/widgetEntry.frag:theme/default/widgetEntry.frag \
                    ../../share/theme/default/widgetEntry.vert:theme/default/widgetEntry.vert \
                    \
                    ../../share/theme/rounded/widgetEntry.prog:theme/rounded/widgetEntry.prog \
                    ../../share/theme/rounded/widgetEntry.frag:theme/rounded/widgetEntry.frag \
                    ../../share/theme/rounded/widgetEntry.vert:theme/rounded/widgetEntry.vert

