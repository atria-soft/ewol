#!/usr/bin/python
import monkModule
import monkTools as tools

def get_desc():
	return "EWOL main library (Edn Widget on OpenGl Layer)"

def create():
	# module name is 'ewol' and type binary.
	myModule = monkModule.Module(__file__, 'ewol', 'LIBRARY')
	# enable doculentation :
	myModule.set_website("http://heeroyui.github.io/ewol/")
	myModule.set_path(tools.get_current_path(__file__) + "/ewol/")
	myModule.set_path_general_doc(tools.get_current_path(__file__) + "/../doc/")
	# add the currrent module at the 
	return myModule

