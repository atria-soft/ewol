/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/os/Semaphore.h>
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

