
# Basic elements
FILE_LIST:= ewol/ewol.cpp \
			ewol/clipBoard.cpp \
			ewol/debug.cpp \
			ewol/config.cpp \
			ewol/commandLine.cpp \
			ewol/key.cpp \
			ewol/cursor.cpp

# Basic Eobject of EWOL
FILE_LIST+= ewol/eObject/EObject.cpp \
			ewol/eObject/EObjectManager.cpp

#openGl Basic access abstraction (for the model matrix and include
FILE_LIST+= ewol/renderer/openGL.cpp

# Operating System interface
FILE_LIST+= ewol/renderer/os/eSystem.cpp \
			ewol/renderer/os/eSystemInput.cpp

# renderer : 
FILE_LIST+= ewol/renderer/resources/Shader.cpp \
			ewol/renderer/resources/Program.cpp \
			ewol/renderer/resources/VirtualBufferObject.cpp \
			ewol/renderer/resources/ConfigFile.cpp \
			ewol/renderer/resources/FontFreeType.cpp \
			ewol/renderer/resources/TexturedFont.cpp \
			ewol/renderer/resources/Mesh.cpp \
			ewol/renderer/resources/MeshObj.cpp \
			ewol/renderer/resources/Texture.cpp \
			ewol/renderer/resources/Image.cpp \
			ewol/renderer/resources/image/ImageBMP.cpp \
			ewol/renderer/resources/image/ImagePNG.cpp \
			ewol/renderer/ResourceManager.cpp

# Audio system
FILE_LIST+= ewol/renderer/audio/audio.cpp \
			ewol/renderer/audio/decWav.cpp

# Compositing
FILE_LIST+= ewol/compositing/Compositing.cpp \
			ewol/compositing/Text.cpp \
			ewol/compositing/Drawing.cpp \
			ewol/compositing/Image.cpp \
			ewol/compositing/Sprite.cpp \
			ewol/compositing/Mesh.cpp \
			ewol/compositing/Shaper.cpp


# all widgets
FILE_LIST+= ewol/widget/Widget.cpp \
			ewol/widget/WidgetManager.cpp \
			ewol/widget/Windows.cpp \
			ewol/widget/Button.cpp \
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

# game mode area :
FILE_LIST+= ewol/widget/Scene.cpp \
			ewol/game/Engine.cpp \
			ewol/game/Element.cpp \
			ewol/game/Bounding.cpp \
			ewol/game/BoundingAABB.cpp \
			ewol/game/BoundingOBB.cpp \
			ewol/game/BoundingPlane.cpp \
			ewol/game/BoundingSphere.cpp \
			ewol/game/Geometry.cpp \
			ewol/game/Gravity.cpp \
			ewol/game/Camera.cpp \
			ewol/game/Map.cpp \
			ewol/game/Mass.cpp \
			ewol/game/Sky.cpp


LOCAL_COPY_FILES := ../data/textured3D.prog:textured3D.prog \
                    ../data/textured3D.frag:textured3D.frag \
                    ../data/textured3D.vert:textured3D.vert \
                    \
                    ../data/color.prog:color.prog \
                    ../data/color.frag:color.frag \
                    ../data/color.vert:color.vert \
                    \
                    ../data/color3.prog:color3.prog \
                    ../data/color3.frag:color3.frag \
                    ../data/color3.vert:color3.vert \
                    \
                    ../data/textured.prog:textured.prog \
                    ../data/textured.frag:textured.frag \
                    ../data/textured.vert:textured.vert \
                    \
                    ../data/text.prog:text.prog \
                    ../data/text.frag:text.frag \
                    ../data/text.vert:text.vert

LOCAL_COPY_FOLDERS := ../data/theme/default/widgetEntry.*:theme/default \
                      ../data/theme/rounded/widgetEntry.*:theme/rounded \
                      ../data/theme/default/widgetButton.*:theme/default \
                      ../data/theme/rounded/widgetButton.*:theme/rounded \
                      
