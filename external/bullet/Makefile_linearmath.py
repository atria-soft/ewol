#!/usr/bin/python
# --------------------------------------------------------
# -- Linear Math librairy
# --------------------------------------------------------
import module
import buildTools

myModule = module.module(__file__, 'linearmath', 'LIBRARY')

myModule.AddSrcFile([
	'bullet/src/LinearMath/btQuickprof.cpp',
	'bullet/src/LinearMath/btGeometryUtil.cpp',
	'bullet/src/LinearMath/btAlignedAllocator.cpp',
	'bullet/src/LinearMath/btSerializer.cpp',
	'bullet/src/LinearMath/btConvexHull.cpp',
	'bullet/src/LinearMath/btPolarDecomposition.cpp',
	'bullet/src/LinearMath/btVector3.cpp',
	'bullet/src/LinearMath/btConvexHullComputer.cpp'])

myModule.CompileFlags_CC([
	'-Wno-write-strings',
	'-DHAVE_CONFIG_H',
	'-O2'])

myModule.AddExportPath(buildTools.GetCurrentPath(__file__)+"/bullet/src/")

# add the currrent module at the 
module.AddModule(myModule)






