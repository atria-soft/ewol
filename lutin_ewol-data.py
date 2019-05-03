#!/usr/bin/python
import realog.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "ewol is a widget management library"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	my_module.copy_path('data/theme/shape/square/*','theme/shape/square')
	my_module.copy_path('data/theme/default/*','theme/shape/square/')
	my_module.copy_path('data/theme/shape/round/*','theme/shape/round')
	my_module.copy_path('data/theme/color/black/*','theme/color/black')
	my_module.copy_path('data/theme/color/white/*','theme/color/white')
	my_module.copy_path('data/textured.*','')
	my_module.copy_path('data/texturedNoMaterial.*','')
	my_module.copy_path('data/text.*','')
	my_module.copy_path('data/simple3D.*','')
	my_module.copy_path('data/color.*','')
	my_module.copy_path('data/color3.*','')
	my_module.copy_path('data/textured3D2.*','')
	my_module.copy_path('data/textured3D.*','')
	my_module.copy_path('data/texturedDF.*','')
	my_module.copy_path('data/fontDistanceField/*','fontDistanceField')
	my_module.copy_path('data/translate/*','translate/ewol')
	my_module.copy_path('data/ewol-gui-*.xml')
	return True

