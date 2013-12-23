#!/usr/bin/python
import lutinModule as module
import lutinTools as tools
import datetime

def get_desc():
	return "Date buid date of the program"


def create(target):
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'date', 'LIBRARY')
	# add the file to compile:
	
	
	myModule.add_src_file([
		'date/date.cpp'])
	
	now = datetime.datetime.now()
	
	myModule.compile_flags_CC([
		'-Wno-write-strings',
		'-Wall',
		"-DBUILD_DAY=\""+str(now.day)+"\"",
		"-DBUILD_MONTH=\""+str(now.month)+"\"",
		"-DBUILD_YEAR=\""+str(now.year)+"\"",
		"-DBUILD_HOUR=\""+str(now.hour)+"\"",
		"-DBUILD_MINUTE=\""+str(now.minute)+"\"",
		"-DBUILD_SECOND=\""+str(now.second)+"\""])
	
	myModule.add_export_path(tools.get_current_path(__file__))
	
	# add the currrent module at the 
	return myModule


