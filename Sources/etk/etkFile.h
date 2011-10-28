/**
 *******************************************************************************
 * @file etkFile.h
 * @brief Ewol Tool Kit : File folder and name abstraction (header)
 * @author Edouard DUPIN
 * @date 16/07/2011
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

#ifndef __ETK_FILE_H__
#define __ETK_FILE_H__

#include <etkString.h>

#define MAX_FILE_NAME      (10240)

namespace etk
{
	class File
	{
		public:
			            File(void) { m_lineNumberOpen=0; }
			            File(etk::String &filename, int32_t LineNumber = 0);
			            File(const char  *filename, int32_t LineNumber = 0);
			            File(etk::String &filename, etk::String &folder, int32_t lineNumber = 0);
			            ~File(void);
			etk::String GetFolder(void) const;
			etk::String GetShortFilename(void) const;
			etk::String GetCompleateName(void) const;
			bool        HasExtention(void);
			etk::String GetExtention(void);
			int32_t     Size(void);
			bool        Exist(void);
			int32_t     GetLineNumber(void);
			void        SetLineNumber(int32_t newline);
			void        SetCompleateName(etk::String &newFilename);
			
			const etk::File& operator=  (const etk::File &etkF );
			bool             operator== (const etk::File &etkF ) const;
			bool             operator!= (const etk::File &etkF ) const;
			friend std::ostream& operator <<( std::ostream &os,const etk::File &obj);
			
		private :
			etk::String m_folder;
			etk::String m_shortFilename;
			int32_t     m_lineNumberOpen;
	};
	
	std::ostream& operator <<(std::ostream &os, const etk::File &obj);
	
}

#endif

