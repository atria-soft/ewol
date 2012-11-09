/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/os/Semaphore.h>
#include <etk/DebugInternal.h>
#include <sys/time.h>

etk::Semaphore::Semaphore(uint32_t nbBasicElement, uint32_t nbMessageMax)
{
	// create interface mutex :
	int ret = pthread_mutex_init(&m_mutex, NULL);
	TK_ASSERT(ret == 0, "Error creating Mutex ...");
	// create contition :
	ret = pthread_cond_init(&m_condition, NULL);
	TK_ASSERT(ret == 0, "Error creating Condition ...");
	if (ret != 0) {
		ret = pthread_mutex_destroy(&m_mutex);
		TK_ASSERT(ret == 0, "Error destroying Mutex ...");
	}
	m_maximum = nbMessageMax;
	m_data = nbBasicElement;
}


etk::Semaphore::~Semaphore(void)
{
	// Remove condition
	int ret = pthread_cond_destroy(&m_condition);
	TK_ASSERT(ret == 0, "Error destroying Condition ...");
	// Remove Mutex
	ret = pthread_mutex_destroy(&m_mutex);
	TK_ASSERT(ret == 0, "Error destroying Mutex ...");
}

uint32_t etk::Semaphore::GetCount(void)
{
	int32_t tmpData = 0;
	pthread_mutex_lock(&m_mutex);
	tmpData = m_data;
	pthread_mutex_unlock(&m_mutex);
	return tmpData;
}

void etk::Semaphore::Post(void)
{
	pthread_mutex_lock(&m_mutex);
	if (m_data>=m_maximum) {
		m_data = m_maximum;
	} else {
		m_data++;
	}
	// send message
	pthread_cond_broadcast(&m_condition);
	pthread_mutex_unlock(&m_mutex);
}


void etk::Semaphore::Wait(void)
{
	pthread_mutex_lock(&m_mutex);
	while(m_data == 0) {
		pthread_cond_wait(&m_condition, &m_mutex);
	}
	m_data--;
	pthread_mutex_unlock(&m_mutex);
}


bool etk::Semaphore::Wait(uint32_t timeOutInUs)
{
	pthread_mutex_lock(&m_mutex);
	if(m_data == 0) {
		struct timeval tp;
		struct timespec ts;
		gettimeofday(&tp,NULL);
		uint64_t totalTimeUS = tp.tv_sec * 1000000 + tp.tv_usec;
		totalTimeUS += timeOutInUs;
		ts.tv_sec = totalTimeUS / 1000000;
		ts.tv_nsec = (totalTimeUS%1000000) * 1000;
		int ret = pthread_cond_timedwait(&m_condition, &m_mutex, &ts);
		if (ret !=0) { //== ETIMEOUT) {
			pthread_mutex_unlock(&m_mutex);
			return false;
		}
	}
	m_data--;
	pthread_mutex_unlock(&m_mutex);
	return true;
}

