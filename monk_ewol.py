#!/usr/bin/python
import monkModule
import monkTools as tools
import os

def get_desc():
	return "EWOL main library (Edn Widget on OpenGl Layer)"

def create():
	# module name is 'ewol' and type binary.
	myModule = monkModule.Module(__file__, 'ewol', 'LIBRARY')
	# enable doculentation :
	myModule.set_website("http://atria-soft.github.io/ewol/")
	myModule.set_website_sources("http://github.com/atria-soft/ewol/")
	myModule.set_path(os.path.join(tools.get_current_path(__file__), "ewol"))
	myModule.set_path_general_doc(os.path.join(tools.get_current_path(__file__), "doc"))
	# add the currrent module at the 
	return myModule

