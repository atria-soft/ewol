/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_FILE_SYSTEM_NODE_RIGHT_H__
#define __ETK_FILE_SYSTEM_NODE_RIGHT_H__

#include <etk/UString.h>

namespace etk
{
	class FSNodeRight
	{
		private:
			uint16_t m_rights;
		public:
			FSNodeRight(void);
			FSNodeRight(int16_t newRight);
			~FSNodeRight(void) { };
			// copy operator :
			const etk::FSNodeRight& operator=  (const etk::FSNodeRight &obj );
			// set right :
			const etk::FSNodeRight& operator=  (const int32_t newVal );
			
			void Clear(void) { m_rights = 0; };
			// User
			bool IsUserReadable(void) const;
			bool IsUserWritable(void) const;
			bool IsUserRunable(void) const;
			void SetUserReadable(bool newStatus);
			void SetUserWritable(bool newStatus);
			void SetUserRunable(bool newStatus);
			// group
			bool IsGroupReadable(void) const;
			bool IsGroupWritable(void) const;
			bool IsGroupRunable(void) const;
			void SetGroupReadable(bool newStatus);
			void SetGroupWritable(bool newStatus);
			void SetGroupRunable(bool newStatus);
			// other
			bool IsOtherReadable(void) const;
			bool IsOtherWritable(void) const;
			bool IsOtherRunable(void) const;
			void SetOtherReadable(bool newStatus);
			void SetOtherWritable(bool newStatus);
			void SetOtherRunable(bool newStatus);
			
			etk::UString GetRight(void) const;
	};
	etk::CCout& operator <<(etk::CCout &os, const etk::FSNodeRight &obj);
};

#endif

