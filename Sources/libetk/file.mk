

FILE_LIST = \
			etk/Debug.cpp \
			etk/DebugInternal.cpp \
			etk/Memory.cpp \
			etk/unicode.cpp \
			etk/unicodeTable.cpp \
			etk/UString.cpp \
			etk/Stream.cpp \
			etk/File.cpp \
			etk/RegExp.cpp \
			etk/tool.cpp

ifeq ("$(TARGET_OS)","Windows")
	FILE_LIST += etk/Mutex.Windows.cpp
	FILE_LIST += etk/Semaphore.Windows.cpp
else
	FILE_LIST += etk/Mutex.Generic.cpp
	FILE_LIST += etk/Semaphore.Generic.cpp
endif
