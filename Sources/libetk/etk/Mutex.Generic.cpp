/**
 *******************************************************************************
 * @file etk/Mutex.Generic.cpp
 * @brief Ewol Tool Kit : basic mutex system (Sources) ==> Pthread implementation
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

#include <etk/Mutex.h>
#include <etk/DebugInternal.h>

etk::Mutex::Mutex(void)
{
	// create interface mutex :
	int ret = pthread_mutex_init(&m_mutex, NULL);
	TK_ASSERT(ret == 0, "Error creating Mutex ...");
}


etk::Mutex::~Mutex(void)
{
	// Remove mutex
	int ret = pthread_mutex_destroy(&m_mutex);
	TK_ASSERT(ret == 0, "Error destroying Mutex ...");
}


void etk::Mutex::Lock(void)
{
	pthread_mutex_lock(&m_mutex);
}


bool etk::Mutex::TryLock(void)
{
	return pthread_mutex_trylock(&m_mutex) != 0;
}


void etk::Mutex::UnLock(void)
{
	pthread_mutex_unlock(&m_mutex);
}

