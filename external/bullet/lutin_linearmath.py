#!/usr/bin/python
# --------------------------------------------------------
# -- Linear Math librairy
# --------------------------------------------------------
import lutinModule as module
import lutinTools as tools

def get_desc():
	return "Bullet lib linar Mathematic interface"


def create(target):
	myModule = module.Module(__file__, 'linearmath', 'LIBRARY')
	#remove compilation warning (specific for external libs):
	myModule.remove_compile_warning()
	
	myModule.add_src_file([
		'bullet/src/LinearMath/btQuickprof.cpp',
		'bullet/src/LinearMath/btGeometryUtil.cpp',
		'bullet/src/LinearMath/btAlignedAllocator.cpp',
		'bullet/src/LinearMath/btSerializer.cpp',
		'bullet/src/LinearMath/btConvexHull.cpp',
		'bullet/src/LinearMath/btPolarDecomposition.cpp',
		'bullet/src/LinearMath/btVector3.cpp',
		'bullet/src/LinearMath/btConvexHullComputer.cpp'])
	
	myModule.compile_flags_CC([
		'-Wno-write-strings',
		'-DHAVE_CONFIG_H',
		'-O2'])
	
	myModule.add_export_path(tools.get_current_path(__file__)+"/bullet/src/")
	
	# add the currrent module at the 
	return myModule






