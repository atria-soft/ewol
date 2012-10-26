/**
 *******************************************************************************
 * @file etk/Mutex.Windows.cpp
 * @brief Ewol Tool Kit : basic mutex system (Sources) ==> windows implementation
 * @author Edouard DUPIN
 * @date 15/08/2012
 * @par Project
 * Ewol TK
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

#include <etk/os/Mutex.h>

etk::Mutex::Mutex(void)
{
	InitializeCriticalSection(&m_mutex);
}


etk::Mutex::~Mutex(void)
{
	DeleteCriticalSection(&m_mutex);
}


void etk::Mutex::Lock(void)
{
	EnterCriticalSection(&m_mutex);
}


bool etk::Mutex::TryLock(void)
{
	return TryEnterCriticalSection(&m_mutex) != 0;
}


void etk::Mutex::UnLock(void)
{
	LeaveCriticalSection(&m_mutex);
}

