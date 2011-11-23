/**
 *******************************************************************************
 * @file guiDoubleBuffer.cpp
 * @brief Gui abstraction layer (Sources)
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


#include <ewol/Debug.h>
#include <base/guiDoubleBuffer.h>

#undef __class__
#define __class__ "guiAbstraction"

void guiAbstraction::Init(int32_t argc, char *argv[])
{
	EWOL_INFO("INIT for DoubleBuffer environement");
}

void guiAbstraction::Run(void)
{
	EWOL_INFO("Start Running");
	EWOL_INFO("Stop Running");
}

void guiAbstraction::UnInit(void)
{
	EWOL_INFO("UN-INIT for DoubleBuffer environement");
}
