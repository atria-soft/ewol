/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_SEMAPHORE_H__
#define __ETK_SEMAPHORE_H__

#include <etk/Types.h>

#ifdef __TARGET_OS__Windows
	#include <windows.h>
#else
	#include <pthread.h>
#endif

namespace etk
{
	class Semaphore
	{
		private:
			#ifdef __TARGET_OS__Windows
				HANDLE           m_semaphore;
			#else
				pthread_mutex_t  m_mutex;
				pthread_cond_t   m_condition;
				uint32_t         m_data;
				uint32_t         m_maximum;
			#endif
		public:
			Semaphore(uint32_t nbBasicElement=0, uint32_t nbMessageMax=1);
			~Semaphore(void);
			uint32_t GetCount(void);
			void Post(void);
			void Wait(void);
			// wait with a timeout in us; return true if get the semaphore
			bool Wait(uint32_t timeOutInUs);
	};
	
};

#endif
