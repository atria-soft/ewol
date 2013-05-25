#!/usr/bin/python
import lutinModule
import lutinTools
import datetime

def Create(target):
	# module name is 'edn' and type binary.
	myModule = lutinModule.module(__file__, 'date', 'LIBRARY')
	# add the file to compile:
	
	
	myModule.AddSrcFile([
		'date/date.cpp'])
	
	now = datetime.datetime.now()
	
	myModule.CompileFlags_CC([
		'-Wno-write-strings',
		'-Wall',
		"-DBUILD_DAY=\""+str(now.day)+"\"",
		"-DBUILD_MONTH=\""+str(now.month)+"\"",
		"-DBUILD_YEAR=\""+str(now.year)+"\"",
		"-DBUILD_HOUR=\""+str(now.hour)+"\"",
		"-DBUILD_MINUTE=\""+str(now.minute)+"\"",
		"-DBUILD_SECOND=\""+str(now.second)+"\""])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__))
	
	# add the currrent module at the 
	return myModule


