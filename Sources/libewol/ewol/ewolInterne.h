/**
 *******************************************************************************
 * @file ewol/ewolInterne.h
 * @brief Main include of ewol internal interation(header)
 * @author Edouard DUPIN
 * @date 07/01/2012
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


#ifndef __EWOL_INTERNE_H__
#define __EWOL_INTERNE_H__

#include <etk/Types.h>
#include <etk/String.h>
#include <ewol/Widget.h>
#include <ewol/Windows.h>



namespace ewol {
	void Init(int32_t argc, char *argv[]);
	void UnInit(void);
};



#endif