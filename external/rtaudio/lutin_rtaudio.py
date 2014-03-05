#!/usr/bin/python
import lutinModule as module
import lutinTools as tools
import lutinDebug as debug

def get_desc():
	return "rtaudio : Generic wrapper on all audio interface"


def create(target):
	myModule = module.Module(__file__, 'rtaudio', 'LIBRARY')
	
	myModule.add_src_file([
		'rtaudio/RtAudio.cpp'
		])
	
	if target.name=="Windows":
		# ASIO API on Windows
		myModule.add_export_flag_CC(['__WINDOWS_ASIO__'])
		# Windows DirectSound API
		#myModule.add_export_flag_CC(['__WINDOWS_DS__'])
	elif target.name=="Linux":
		# Linux Alsa API
		myModule.add_export_flag_CC(['-D__LINUX_ALSA__'])
		# Linux Jack API
		#myModule.add_export_flag_CC(['-D__UNIX_JACK__'])
		# Linux PulseAudio API
		#myModule.add_export_flag_CC(['-D__LINUX_PULSE__'])
		#depending libs :
		myModule.add_export_flag_LD("-lasound")
		myModule.add_export_flag_LD("-lpthread")
	elif target.name=="MacOs":
		# MacOsX core
		myModule.add_export_flag_CC(['__MACOSX_CORE__'])
		myModule.add_export_flag_LD("-framework CoreAudio")
		myModule.add_export_flag_LD("-framework CoreMIDI")
	else:
		debug.warning("unknow target for RTAudio : " + target.name);
	
	myModule.add_export_path(tools.get_current_path(__file__))
	myModule.add_path(tools.get_current_path(__file__)+"/rtaudio/")
	myModule.add_path(tools.get_current_path(__file__)+"/rtaudio/include/")
	
	# add the currrent module at the 
	return myModule









