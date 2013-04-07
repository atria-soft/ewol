LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

LOCAL_CONFIG_FILES := Config.in ConfigLinux.in

# get the tag of the current project : 
LOCAL_VERSION=$(shell cat $(LOCAL_PATH)/tag)
$(info [TAG:$(LOCAL_MODULE)] $(LOCAL_VERSION))

# name of the dependency
LOCAL_LIBRARIES := etk freetype tinyxml libpng parsersvg
ifeq ("$(CONFIG_BUILD_BULLET)","y")
LOCAL_LIBRARIES += bullet
endif
ifeq ("$(CONFIG_BUILD_LUA)","y")
LOCAL_LIBRARIES += lua
endif
ifeq ("$(CONFIG_BUILD_PORTAUDIO)","y")
LOCAL_LIBRARIES += portaudio
endif
ifeq ("$(CONFIG_BUILD_LIBZIP)","y")
LOCAL_LIBRARIES += libzip
endif

LOCAL_C_INCLUDES := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -framework Cocoa  -framework OpenGL

LOCAL_CFLAGS := -Wno-write-strings \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG)-$(BUILD_DIRECTORY_MODE)\"" \
                -Wall

LOCAL_EXPORT_CFLAGS := 

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

LOCAL_SRC_FILES += ewol/renderer/os/gui.MacOs.cpp \
                   ewol/renderer/os/gui.MacOs.Interface.mm \

#DFGSDFGSFDGDSFGSDFG := \
#                   ewol/renderer/os/gui.MacOs.AppDelegate.mm \
#                   ewol/renderer/os/gui.MacOs.OpenglView.mm

# xcode compilation exemple sources : 
## basic truchage
#ProcessInfoPlistFile /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/Info.plist basicWindowsTestInOpenGL/basicWindowsTestInOpenGL-Info.plist
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#builtin-infoPlistUtility basicWindowsTestInOpenGL/basicWindowsTestInOpenGL-Info.plist -genpkginfo /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/PkgInfo -expandbuildsettings -platform macosx -o /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/Info.plist

## precompile je sais pas trop.pch
#ProcessPCH /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/PrecompiledHeaders/basicWindowsTestInOpenGL-Prefix-apbwwwblavjitifkwqsuuwvalavo/basicWindowsTestInOpenGL-Prefix.pch.pth basicWindowsTestInOpenGL/basicWindowsTestInOpenGL-Prefix.pch normal x86_64 objective-c com.apple.compilers.llvm.clang.1_0.compiler
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#setenv LANG en_US.US-ASCII
#/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x objective-c-header -arch x86_64 -fmessage-length=0 -std=gnu99 -fobjc-arc -Wno-trigraphs -fpascal-strings -O0 -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wno-implicit-atomic-properties -Wno-receiver-is-weak -Wduplicate-method-match -Wformat -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wenum-conversion -Wshorten-64-to-32 -Wpointer-sign -Wno-newline-eof -Wno-selector -Wno-strict-selector-match -Wno-undeclared-selector -Wno-deprecated-implementations -DDEBUG=1 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk -fasm-blocks -fstrict-aliasing -Wprotocol -Wdeprecated-declarations -mmacosx-version-min=10.8 -g -Wno-sign-conversion -iquote /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-generated-files.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-own-target-headers.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-all-target-headers.hmap -iquote /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-project-headers.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/DerivedSources/x86_64 -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/DerivedSources -F/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug --serialize-diagnostics /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/PrecompiledHeaders/basicWindowsTestInOpenGL-Prefix-apbwwwblavjitifkwqsuuwvalavo/basicWindowsTestInOpenGL-Prefix.pch.dia -c /Users/edouarddupin/dev/basicWindowsTestInOpenGL/basicWindowsTestInOpenGL/basicWindowsTestInOpenGL-Prefix.pch -o /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/PrecompiledHeaders/basicWindowsTestInOpenGL-Prefix-apbwwwblavjitifkwqsuuwvalavo/basicWindowsTestInOpenGL-Prefix.pch.pth -MMD -MT dependencies -MF /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/PrecompiledHeaders/basicWindowsTestInOpenGL-Prefix-apbwwwblavjitifkwqsuuwvalavo/basicWindowsTestInOpenGL-Prefix.pch.d

# compile main.m
#CompileC /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/main.o basicWindowsTestInOpenGL/main.m normal x86_64 objective-c com.apple.compilers.llvm.clang.1_0.compiler
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#setenv LANG en_US.US-ASCII
#/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x objective-c -arch x86_64 -fmessage-length=0 -std=gnu99 -fobjc-arc -Wno-trigraphs -fpascal-strings -O0 -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wno-implicit-atomic-properties -Wno-receiver-is-weak -Wduplicate-method-match -Wformat -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wenum-conversion -Wshorten-64-to-32 -Wpointer-sign -Wno-newline-eof -Wno-selector -Wno-strict-selector-match -Wno-undeclared-selector -Wno-deprecated-implementations -DDEBUG=1 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk -fasm-blocks -fstrict-aliasing -Wprotocol -Wdeprecated-declarations -mmacosx-version-min=10.8 -g -Wno-sign-conversion -iquote /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-generated-files.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-own-target-headers.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-all-target-headers.hmap -iquote /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-project-headers.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/DerivedSources/x86_64 -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/DerivedSources -F/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug -include /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/PrecompiledHeaders/basicWindowsTestInOpenGL-Prefix-apbwwwblavjitifkwqsuuwvalavo/basicWindowsTestInOpenGL-Prefix.pch -MMD -MT dependencies -MF /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/main.d --serialize-diagnostics /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/main.dia -c /Users/edouarddupin/dev/basicWindowsTestInOpenGL/basicWindowsTestInOpenGL/main.m -o /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/main.o

## compile app delegates
#CompileC /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/AppDelegate.o basicWindowsTestInOpenGL/AppDelegate.m normal x86_64 objective-c com.apple.compilers.llvm.clang.1_0.compiler
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#setenv LANG en_US.US-ASCII
#/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x objective-c -arch x86_64 -fmessage-length=0 -std=gnu99 -fobjc-arc -Wno-trigraphs -fpascal-strings -O0 -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wno-implicit-atomic-properties -Wno-receiver-is-weak -Wduplicate-method-match -Wformat -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wenum-conversion -Wshorten-64-to-32 -Wpointer-sign -Wno-newline-eof -Wno-selector -Wno-strict-selector-match -Wno-undeclared-selector -Wno-deprecated-implementations -DDEBUG=1 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk -fasm-blocks -fstrict-aliasing -Wprotocol -Wdeprecated-declarations -mmacosx-version-min=10.8 -g -Wno-sign-conversion -iquote /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-generated-files.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-own-target-headers.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-all-target-headers.hmap -iquote /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/basicWindowsTestInOpenGL-project-headers.hmap -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/DerivedSources/x86_64 -I/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/DerivedSources -F/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug -include /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/PrecompiledHeaders/basicWindowsTestInOpenGL-Prefix-apbwwwblavjitifkwqsuuwvalavo/basicWindowsTestInOpenGL-Prefix.pch -MMD -MT dependencies -MF /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/AppDelegate.d --serialize-diagnostics /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/AppDelegate.dia -c /Users/edouarddupin/dev/basicWindowsTestInOpenGL/basicWindowsTestInOpenGL/AppDelegate.m -o /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/AppDelegate.o

# ld : 
#Ld /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/MacOS/basicWindowsTestInOpenGL normal x86_64
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#setenv MACOSX_DEPLOYMENT_TARGET 10.8
#/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -arch x86_64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk -L/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug -F/Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug -filelist /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Intermediates/basicWindowsTestInOpenGL.build/Debug/basicWindowsTestInOpenGL.build/Objects-normal/x86_64/basicWindowsTestInOpenGL.LinkFileList -mmacosx-version-min=10.8 -fobjc-arc -fobjc-link-runtime -framework Cocoa -o /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/MacOS/basicWindowsTestInOpenGL

## copy basic string
#CopyStringsFile /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/Resources/en.lproj/InfoPlist.strings basicWindowsTestInOpenGL/en.lproj/InfoPlist.strings
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#builtin-copyStrings --validate --inputencoding utf-8 --outputencoding UTF-16 --outdir /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/Resources/en.lproj -- basicWindowsTestInOpenGL/en.lproj/InfoPlist.strings

## copy credit
#CpResource basicWindowsTestInOpenGL/en.lproj/Credits.rtf /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/Resources/en.lproj/Credits.rtf
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#builtin-copy -exclude .DS_Store -exclude CVS -exclude .svn -exclude .git -exclude .hg -resolve-src-symlinks /Users/edouarddupin/dev/basicWindowsTestInOpenGL/basicWindowsTestInOpenGL/en.lproj/Credits.rtf /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/Resources/en.lproj

## create main menu xib
#CompileXIB basicWindowsTestInOpenGL/en.lproj/MainMenu.xib
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#setenv XCODE_DEVELOPER_USR_PATH /Applications/Xcode.app/Contents/Developer/usr/bin/..
#/Applications/Xcode.app/Contents/Developer/usr/bin/ibtool --errors --warnings --notices --output-format human-readable-text --compile /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app/Contents/Resources/en.lproj/MainMenu.nib /Users/edouarddupin/dev/basicWindowsTestInOpenGL/basicWindowsTestInOpenGL/en.lproj/MainMenu.xib --sdk /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk
    
## touche app
#Touch /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app
#cd /Users/edouarddupin/dev/basicWindowsTestInOpenGL
#/usr/bin/touch -c /Users/edouarddupin/Library/Developer/Xcode/DerivedData/basicWindowsTestInOpenGL-aqlzuzmwjgolzcctybifonzjwojs/Build/Products/Debug/basicWindowsTestInOpenGL.app


include $(BUILD_STATIC_LIBRARY)
