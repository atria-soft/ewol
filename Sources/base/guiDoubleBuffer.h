/**
 *******************************************************************************
 * @file guiDoubleBuffer.h
 * @brief Gui abstraction layer (header)
 * @author Edouard DUPIN
 * @date 20/10/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */


#ifndef __GUI_ABSTRACTION_H__
#define __GUI_ABSTRACTION_H__

#include <etkTypes.h>

namespace guiAbstraction
{
	void Init(int32_t argc, char *argv[]);
	void Run(void);
	void UnInit(void);
};



#endif
