/**
 *******************************************************************************
 * @file etk/File.h
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

#include <etk/String.h>

#define MAX_FILE_NAME      (10240)

//http://developer.android.com/guide/topics/data/data-storage.html

namespace etk
{
	typedef enum {
		// user might done abstraction ==> acces of the sdcard when possible ...
		FILE_TYPE_DIRECT,
		
		// depend of the case
		//     - PC      : /usr/shared/programName/
		//     - Android : Internal at the executable file (pointer on static area)
		//     - Apple   : Internal at the executable file
		FILE_TYPE_DATA,
		
		// depend on case
		//     - PC      : ~/.programName/
		//     - Android : /data/data/programName/files/
		//     - Apple   : ????
		FILE_TYPE_USER_DATA,
		
		// depend on case
		//     - PC      : ~/.programName/cache/
		//     - Android : /data/data/programName/cache/
		//     - Apple   : ????
		FILE_TYPE_CACHE,
	} FileType_te;
	
	class File
	{
		public:
			            File(void) { m_lineNumberOpen=0; m_type = etk::FILE_TYPE_DIRECT; m_PointerFile = NULL;}
			            File(etk::String &filename, etk::FileType_te type = etk::FILE_TYPE_DIRECT, int32_t LineNumber = 0);
			            File(const char  *filename, etk::FileType_te type = etk::FILE_TYPE_DIRECT, int32_t LineNumber = 0);
			            File(etk::String &filename, etk::String &folder, etk::FileType_te type = etk::FILE_TYPE_DIRECT, int32_t lineNumber = 0);
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
			void        SetCompleateName(etk::String &newFilename, etk::FileType_te type);
			
			const etk::File& operator=  (const etk::File &etkF );
			bool             operator== (const etk::File &etkF ) const;
			bool             operator!= (const etk::File &etkF ) const;
			friend etk::CCout& operator <<( etk::CCout &os,const etk::File &obj);
			
			FileType_te GetTypeAccess(void) { return m_type; };
			char *      GetDirectPointer(void);
			
			// TODO : IO access of the file :
			bool        fOpenRead(void);
			bool        fOpenWrite(void);
			bool        fClose(void);
			char *      fGets(char * elementLine, int32_t maxData);
			int32_t     fRead(void * data, int32_t blockSize, int32_t nbBlock);
			int32_t     fWrite(void * data, int32_t blockSize, int32_t nbBlock);
		private :
			etk::FileType_te m_type;
			FILE *           m_PointerFile;
			#ifdef DATA_INTERNAL_BINARY
			int32_t          m_idInternal;
			int32_t          m_readingOffset;
			#endif
			etk::String      m_folder;
			etk::String      m_shortFilename;
			int32_t          m_lineNumberOpen;
	};
	
	etk::CCout& operator <<(etk::CCout &os, const etk::File &obj);
	
	void SetBaseFolderData(const char * folder);
	void SetBaseFolderDataUser(const char * folder);
	void SetBaseFolderCache(const char * folder);

}

#endif

