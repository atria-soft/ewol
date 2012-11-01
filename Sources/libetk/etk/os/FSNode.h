/**
 *******************************************************************************
 * @file etk/os/FSNode.h
 * @brief Ewol Tool Kit : File System node access abstraction (header)
 * @author Edouard DUPIN
 * @date 31/10/2012
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

#ifndef __ETK_FILE_SYSTEM_NODE_H__
#define __ETK_FILE_SYSTEM_NODE_H__

#include <etk/UString.h>

#define MAX_FILE_NAME      (10240)

//http://developer.android.com/guide/topics/data/data-storage.html

namespace etk
{
	// Right Flags :
	enum {
		RIGHT_OTHER_EXECUTE = 1 << 0,
		RIGHT_OTHER_WRITE   = 1 << 1,
		RIGHT_OTHER_READ    = 1 << 2,
		RIGHT_GROUP_EXECUTE = 1 << 3,
		RIGHT_GROUP_WRITE   = 1 << 4,
		RIGHT_GROUP_READ    = 1 << 5,
		RIGHT_USER_EXECUTE  = 1 << 6,
		RIGHT_USER_WRITE    = 1 << 7,
		RIGHT_USER_READ     = 1 << 8,
		RIGHT_FILE          = 1 << 9,
		RIGHT_FOLDER        = 1 << 10,
		RIGHT_LINK          = 1 << 12,
		RIGHT_PARSE_DONE    = 1 << 31  //!< a simple flag to know if the right has been checked
	};

	typedef enum {
		FSN_TYPE_UNKNOW,
		// user might done abstraction ==> acces of the sdcard when possible ...
		FSN_TYPE_DIRECT,
		FSN_TYPE_RELATIF,
		
		// depend on case
		//     - PC      : ~/
		//     - Android : /sdcard/
		//     - Apple   : ????
		FSN_TYPE_HOME,
		
		// depend of the case
		//     - PC      : /usr/shared/programName/
		//     - Android : Internal at the executable file (pointer on static area)
		//     - Apple   : Internal at the executable file
		FSN_TYPE_DATA,
		
		// depend on case
		//     - PC      : ~/.local/programName/
		//     - Android : /data/data/programName/files/
		//     - Apple   : ????
		FSN_TYPE_USER_DATA,
		
		// depend on case
		//     - PC      : ~/.programName/cache/
		//     - Android : /data/data/programName/cache/
		//     - Apple   : ????
		FSN_TYPE_CACHE,
		
		// depend on case
		//     - try on FSN_TYPE_USER_DATA/theme/themeName/xxx
		//     - try on FSN_TYPE_DATA/theme/themeName/xxx
		//     - try on FSN_TYPE_EWOL_DATA/theme/themeName/xxx
		//     and jump to the default theme file
		//     - try on FSN_TYPE_USER_DATA/theme/default/xxx
		//     - try on FSN_TYPE_DATA/theme/default/xxx
		//     - try on FSN_TYPE_EWOL_DATA/theme/default/xxx
		FSN_TYPE_THEME,
	} FSNType_te;
	
	etk::CCout& operator <<(etk::CCout &os, const etk::FSNType_te &obj);
	
	/*
	note : The filename can be
		Direct mode :
			DIRECT:/sdfsdfsdf/
			/XX ==> for Linux / Mac / Android
			[a-zA-Z]:/xxx ==> for Windows
		
		Data mode :
			DATA:folder/File.ccc
		
		User data mode :
			USERDATA:folder/File.ccc
		
		Cache Data :
			CACHE:folder/File.ccc
		
		Theme data :
			THEME:folder/file.xxx
		
		Get the root folder :
			ROOT:
			/
			[a-zA-Z]: ==> create more risk ...
		
		Get the Home folder :
			HOME:
			~
	*/
	class FSNode
	{
		private:
			etk::UString     m_userFileName;       //!< the name requested by the User
			FSNType_te       m_type;               //!< the Type of data requested by the User
			//etk::UString     m_realFileSystemName; //!< the real FS name
			uint32_t         m_rights;             //!< IO right of the current file
			// specific when file Access :
			FILE *           m_PointerFile;
		private:
			etk::UString     GetFileSystemName(void) const;
			void             PrivateSetName(etk::UString& newName);
		private:
			#ifdef __TARGET_OS__Android
				bool         LoadDataZip(void);
				int32_t      m_idZipFile;
				char *       m_zipData;
				int32_t      m_zipDataSize;
				int32_t      m_zipReadingOffset;
			#endif
			
		public:
			FSNode(void);
			FSNode(etk::UString nodeName);
			~FSNode(void);
			/*
				All Right of the file
			*/
			bool Exist(void);
			bool IsFile(void);
			bool IsFolder(void);
			bool IsLink(void);
			// User
			bool IsUserReadable(void);
			bool IsUserWritable(void);
			bool IsUserRunable(void);
			void SetUserReadable(bool newStatus);
			void SetUserWritable(bool newStatus);
			void SetUserRunable(bool newStatus);
			// group
			bool IsGroupReadable(void);
			bool IsGroupWritable(void);
			bool IsGroupRunable(void);
			void SetGroupReadable(bool newStatus);
			void SetGroupWritable(bool newStatus);
			void SetGroupRunable(bool newStatus);
			// other
			bool IsOtherReadable(void);
			bool IsOtherWritable(void);
			bool IsOtherRunable(void);
			void SetOtherReadable(bool newStatus);
			void SetOtherWritable(bool newStatus);
			void SetOtherRunable(bool newStatus);
			
			
			/*
				Common API :
			*/
			void         SetName(etk::UString newName);
			etk::UString GetNameFolder(void) const;
			etk::UString GetName(void) const;
			etk::UString GetNameFile(void) const;
			etk::UString GetRelativeFolder(void) const;
			bool         Touch(void);
			FSNType_te   GetTypeAccess(void) { return m_type; };
			bool         Remove(void);
			uint64_t     TimeCreated(void);
			uint64_t     TimeModified(void);
			
			/*
				Operator :
			*/
			const etk::FSNode& operator=  (const etk::FSNode &obj );
			bool               operator== (const etk::FSNode &obj ) const;
			bool               operator!= (const etk::FSNode &obj ) const;
			friend etk::CCout& operator <<( etk::CCout &os,const etk::FSNode &obj);
			
			/*
				Folder specific :
			*/
			int32_t                  FolderCount(void);
			etk::Vector<etk::FSNode> FolderGetSubList(void);
			etk::FSNode              FolderGetParent(void); // ordered by name ...
			
			/*
				File Specific :
			*/
			bool         FileHasExtention(void);
			etk::UString FileGetExtention(void);
			int32_t      FileSize(void);
			bool         FileOpenRead(void);
			bool         FileOpenWrite(void);
			bool         FileClose(void);
			char*        FileGets(char * elementLine, int32_t maxData);
			int32_t      FileRead(void * data, int32_t blockSize, int32_t nbBlock);
			int32_t      FileWrite(void * data, int32_t blockSize, int32_t nbBlock);
			bool         FileSeek(long int offset, int origin);
	};
	
	etk::CCout& operator <<(etk::CCout &os, const etk::FSNode &obj);
	
	void SetBaseFolderData(const char * folder);
	void SetBaseFolderDataUser(const char * folder);
	void SetBaseFolderCache(const char * folder);
	void InitDefaultFolder(const char * applName);
	etk::UString GetUserHomeFolder(void);
	

}

#endif

