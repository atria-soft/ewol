

FILE_LIST = \
			etk/Debug.cpp \
			etk/DebugInternal.cpp \
			etk/unicode.cpp \
			etk/unicodeTable.cpp \
			etk/UString.cpp \
			etk/Stream.cpp \
			etk/RegExp.cpp \
			etk/tool.cpp
			


FILE_LIST+= \
			etk/math/Matrix4.cpp


FILE_LIST+= \
			etk/os/FSNode.cpp \
			etk/os/FSNodeRight.cpp \
			etk/os/Memory.cpp \



ifeq ("$(TARGET_OS)","Windows")
	FILE_LIST += etk/os/Mutex.Windows.cpp
	FILE_LIST += etk/os/Semaphore.Windows.cpp
else
	FILE_LIST += etk/os/Mutex.Generic.cpp
	FILE_LIST += etk/os/Semaphore.Generic.cpp
endif
