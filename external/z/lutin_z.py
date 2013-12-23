#!/usr/bin/python
import lutinModule as module
import lutinTools as tools

def get_desc():
	return "z-lib library (for windows)"


def create(target):
	if target.name=="Windows":
		myModule = module.Module(__file__, 'z', 'LIBRARY')
		myModule.add_src_file([
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
		
		myModule.add_export_path(tools.get_current_path(__file__))
		myModule.add_export_path(tools.get_current_path(__file__) + "/zlib")
		
		myModule.compile_flags_CC([
			"-D_LARGEFILE64_SOURCE=1",
			"-DHAVE_HIDDEN"])
		
		# add the currrent module at the 
		return myModule
	else:
		myModule = module.Module(__file__, 'z', 'PREBUILD')
		
		myModule.add_export_flag_LD('-lz')
		# add the currrent module at the 
		return myModule


