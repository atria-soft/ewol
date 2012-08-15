/**
 *******************************************************************************
 * @file etk/Semaphore.Generic.cpp
 * @brief Ewol Tool Kit : basic semaphore system (Sources) ==> Pthread implementation
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

#include <etk/Semaphore.h>
#include <etk/DebugInternal.h>

etk::Semaphore::Semaphore(uint32_t nbBasicElement, uint32_t nbMessageMax)
{
	// create interface mutex :
	m_semaphore = CreateSemaphore(NULL, nbBasicElement, nbMessageMax, NULL);
	TK_ASSERT(m_semaphore != 0, "Error creating SEMAPHORE ...");
}


etk::Semaphore::~Semaphore(void)
{
	CloseHandle(m_semaphore);
}

uint32_t etk::Semaphore::GetCount(void)
{
	LONG tmpData = 0;
	ReleaseSemaphore(m_semaphore, 0, &tmpData);
	return tmpData;
}

void etk::Semaphore::Post(void)
{
	ReleaseSemaphore(m_semaphore, 1, NULL);
}


void etk::Semaphore::Wait(void)
{
	WaitForSingleObject(m_semaphore, INFINITE);
}


bool etk::Semaphore::Wait(uint32_t timeOutInUs)
{
	DWORD result = WaitForSingleObject(m_semaphore, timeOutInUs);
	if (result == WAIT_FAILED) {
		TK_ERROR("Failed to wait for semaphore ");
		return false;
	} else {
		return result == WAIT_OBJECT_0;
	}
}

