/**
 *******************************************************************************
 * @file etk/Mutex.h
 * @brief Ewol Tool Kit : basic mutex system (header)
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

#ifndef __ETK_MUTEX_H__
#define __ETK_MUTEX_H__

#include <etk/Types.h>

#ifdef __TARGET_OS__Windows
	#include <windows.h>
#else
	#include <pthread.h>
#endif

namespace etk
{
	class Mutex
	{
		private:
			#ifdef __TARGET_OS__Windows
				CRITICAL_SECTION m_mutex;
			#else
				pthread_mutex_t  m_mutex;
			#endif
		public:
			Mutex(void);
			~Mutex(void);
			void Lock(void);
			bool TryLock(void);
			void UnLock(void);
	};
	
};

#endif
