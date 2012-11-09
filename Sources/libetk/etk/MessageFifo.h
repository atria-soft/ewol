/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_MESSAGE_FIFO_H__
#define __ETK_MESSAGE_FIFO_H__

#include <etk/os/Mutex.h>
#include <etk/os/Semaphore.h>
#include <etk/Vector.h>

namespace etk
{
	template<class MY_TYPE=int32_t> class MessageFifo
	{
		private :
			etk::Mutex           m_mutex;
			etk::Semaphore       m_semaphore;
			etk::Vector<MY_TYPE> m_data;
		public :
			MessageFifo(void)
			{
				// nothing to do ...
			};
			~MessageFifo(void)
			{
				// nothing to do ...
			};
			
			bool Wait(MY_TYPE &data)
			{
				m_mutex.Lock();
				// Check if data is not previously here
				while(0==m_data.Size()) {
					m_mutex.UnLock();
					m_semaphore.Wait();
					m_mutex.Lock();
				}
				// End Waiting message :
				if (0<m_data.Size()) {
					// copy element :
					data = m_data[0];
					// remove element :
					m_data.Erase(0);
					// remove lock
					m_mutex.UnLock();
					return true;
				}
				return false;
			};
			bool Wait(MY_TYPE &data, uint32_t timeOutInUs)
			{
				m_mutex.Lock();
				// Check if data is not previously here
				while(0==m_data.Size()) {
					m_mutex.UnLock();
					if (false == m_semaphore.Wait(timeOutInUs)) {
						return false;
					}
					m_mutex.Lock();
				}
				// End Waiting message :
				if (0<m_data.Size()) {
					// copy element :
					data = m_data[0];
					// remove element :
					m_data.Erase(0);
					// remove lock
					m_mutex.UnLock();
					return true;
				}
				return false;
			};
			int32_t Count(void)
			{
				m_mutex.Lock();
				int32_t nbElement = m_data.Size();
				m_mutex.UnLock();
				return nbElement;
			};
			void Post(MY_TYPE &data)
			{
				m_mutex.Lock();
				m_data.PushBack(data);
				m_semaphore.Post();
				m_mutex.UnLock();
			};
			void Clean(void)
			{
				m_mutex.Lock();
				// remove data
				m_data.Clear();
				m_mutex.UnLock();
				// remove semaphore
				m_semaphore.Wait(0);
			};
	};
};

#endif
