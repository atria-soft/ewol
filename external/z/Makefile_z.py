#!/usr/bin/python
import module

myModule = module.module(__file__, 'zlib', 'LIBRARY')

myModule.AddExportflag_LD('-lz')

# add the currrent module at the 
module.AddModule(myModule)