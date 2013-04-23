#!/usr/bin/python
import lutinModule
import lutinTools

def Create(target):
	if target.name=="Windows":
		myModule = lutinModule.module(__file__, 'z', 'LIBRARY')
		myModule.AddSrcFile([
			"zlib/adler32.c",
			"zlib/crc32.c",
			"zlib/deflate.c",
			"zlib/infback.c",
			"zlib/inffast.c",
			"zlib/inflate.c",
			"zlib/inftrees.c",
			"zlib/trees.c",
			"zlib/zutil.c",
			"zlib/compress.c",
			"zlib/uncompr.c",
			"zlib/gzclose.c",
			"zlib/gzlib.c",
			"zlib/gzread.c",
			"zlib/gzwrite.c"])
		
		myModule.AddExportPath(lutinTools.GetCurrentPath(__file__))
		myModule.AddExportPath(lutinTools.GetCurrentPath(__file__) + "/zlib")
		
		myModule.CompileFlags_CC([
			"-D_LARGEFILE64_SOURCE=1",
			"-DHAVE_HIDDEN"])
		
		# add the currrent module at the 
		return myModule
	else:
		myModule = lutinModule.module(__file__, 'z', 'PREBUILD')
		
		myModule.AddExportflag_LD('-lz')
		# add the currrent module at the 
		return myModule


