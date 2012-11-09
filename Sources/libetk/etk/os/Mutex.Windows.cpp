/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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

