/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_FILE_SYSTEM_NODE_H__
#define __ETK_FILE_SYSTEM_NODE_H__

#include <etk/UString.h>
#include <etk/os/FSNodeRight.h>

#define MAX_FILE_NAME      (10240)

//http://developer.android.com/guide/topics/data/data-storage.html

namespace etk
{
	
	typedef enum {
		FSN_UNKNOW,
		FSN_BLOCK,
		FSN_CHARACTER,
		FSN_FOLDER,
		FSN_FIFO,
		FSN_LINK,
		FSN_FILE,
		FSN_SOCKET,
	} typeNode_te;
	
	etk::CCout& operator <<(etk::CCout &os, const etk::typeNode_te &obj);
	
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
		//     - try on FSN_TYPE_EWOL_DATA/theme/themeName/xxx ==> later when the lib will be accessible in packages
		//     and jump to the default theme file
		//     - try on FSN_TYPE_USER_DATA/theme/default/xxx
		//     - try on FSN_TYPE_DATA/theme/default/xxx
		//     - try on FSN_TYPE_EWOL_DATA/theme/default/xxx ==> later when the lib will be accessible in packages
		FSN_TYPE_THEME,
		FSN_TYPE_THEME_DATA
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
			etk::UString     m_systemFileName;     //!< the compleate filename for the system
			FSNType_te       m_type;               //!< the Type of data requested by the User
			typeNode_te      m_typeNode;           //!< type of the current file/Folder/Link
			etk::FSNodeRight m_rights;             //!< IO right of the current file
			// specific when file Access :
			FILE *           m_PointerFile;
			uint64_t         m_timeCreate;
			uint64_t         m_timeModify;
			uint64_t         m_timeAccess;
			uint32_t         m_idOwner;
			uint32_t         m_idGroup;
		private:
			etk::UString     GetFileSystemName(void) const;
			etk::UString     GetFileSystemNameTheme(void);
			void             PrivateSetName(etk::UString& newName);
			bool             DirectExistFile(etk::UString tmpFileNameDirect, bool checkInAPKIfNeeded = false);
			
			// Now we generate the real FS path:
			void GenerateFileSystemPath(void);
			// now we get all the right if the file existed:
			void UpdateFileSystemProperty(void);
			
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
			bool Exist(void) { return (m_typeNode!=etk::FSN_UNKNOW); };
			typeNode_te GetNodeType(void) { return m_typeNode; };
			etk::FSNodeRight GetRight(void) { return m_rights; };
			bool SetRight(etk::FSNodeRight newRight);
			
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
			// File time properties
			uint64_t     TimeCreated(void) const;
			etk::UString TimeCreatedString(void) const;
			uint64_t     TimeModified(void) const;
			etk::UString TimeModifiedString(void) const;
			uint64_t     TimeAccessed(void) const;
			etk::UString TimeAccessedString(void) const;
			
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
			int32_t                    FolderCount(void);
			etk::Vector<etk::FSNode *> FolderGetSubList(bool showHidenFile=true,
			                                            bool getFolderAndOther=true,
			                                            bool getFile=true,
			                                            bool temporaryFile=true);
			etk::FSNode                FolderGetParent(void); // ordered by name ...
			
			/*
				File Specific :
			*/
			bool         FileHasExtention(void);
			etk::UString FileGetExtention(void);
			uint64_t     FileSize(void);
			bool         FileOpenRead(void);
			bool         FileOpenWrite(void);
			bool         FileClose(void);
			char*        FileGets(char * elementLine, int32_t maxData);
			// TODO : Set the return and the size of block in 64 bits
			int32_t      FileRead(void * data, int32_t blockSize, int32_t nbBlock);
			int32_t      FileWrite(void * data, int32_t blockSize, int32_t nbBlock);
			bool         FileSeek(long int offset, int origin);
		private:
			void SortElementList(etk::Vector<etk::FSNode *> &list);
	};
	
	etk::CCout& operator <<(etk::CCout &os, const etk::FSNode &obj);
	
	void SetBaseFolderData(const char * folder);
	void SetBaseFolderDataUser(const char * folder);
	void SetBaseFolderCache(const char * folder);
	void InitDefaultFolder(const char * applName);
	etk::UString GetUserHomeFolder(void);
	
	namespace theme
	{
		// TODO : Add an INIT ...
		// set the Folder of a subset of a theme ...
		void       SetName(etk::UString refName, etk::UString folderName);
		// get the folder from a Reference theme
		etk::UString GetName(etk::UString refName);
		// get the list of all the theme folder availlable in the user Home/appl
		etk::Vector<etk::UString> List(void);
	};
};

#endif

