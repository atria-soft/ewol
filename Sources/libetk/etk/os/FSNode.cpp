/**
 *******************************************************************************
 * @file etk/os/FSNode.cpp
 * @brief Ewol Tool Kit : File System node access abstraction (Sources)
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


#include <etk/Types.h>
#include <etk/DebugInternal.h>
#include <etk/os/FSNode.h>
#include <unistd.h>
#include <stdlib.h>
#include <etk/tool.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
	#include <sys/stat.h>
}

#ifdef __TARGET_OS__Android
#	include <stdio.h>
#	include <zip/zip.h>
#endif


// zip file of the apk file for Android ==> set to zip file apk access
static etk::UString s_fileAPK = "";
static etk::UString baseApplName = "ewolNoName";
#if defined(__TARGET_OS__Android)
	static etk::UString baseFolderHome     = "/sdcard/";                 // home folder
	static etk::UString baseFolderData     = "assets/";                  // program Data
	static etk::UString baseFolderDataUser = "/sdcard/.tmp/userData/";   // Data specific user (local modification)
	static etk::UString baseFolderCache    = "/sdcard/.tmp/cache/";      // Temporary data (can be removed the next time)
#elif defined(__TARGET_OS__Windows)
	static etk::UString baseFolderHome     = "c:/test";                  // home folder
	static etk::UString baseFolderData     = "c:/test/share/";           // program Data
	static etk::UString baseFolderDataUser = "c:/test/userData/";        // Data specific user (local modification)
	static etk::UString baseFolderCache    = "c:/Windows/Temp/ewol/";    // Temporary data (can be removed the next time)
#else
	static etk::UString baseFolderHome     = "~";                  // home folder
	static etk::UString baseFolderData     = "share/";             // program Data
	static etk::UString baseFolderDataUser = "~/.tmp/userData/";   // Data specific user (local modification)
	static etk::UString baseFolderCache    = "~/.tmp/cache/";      // Temporary data (can be removed the next time)
#endif


#ifdef __TARGET_OS__Android
	static struct zip * s_APKArchive = NULL;
	static int32_t      s_APKnbFiles = 0;
	static void loadAPK(etk::UString& apkPath)
	{
		TK_DEBUG("Loading APK \"" << apkPath << "\"");
		s_APKArchive = zip_open(apkPath.c_str(), 0, NULL);
		TK_ASSERT(s_APKArchive != NULL, "Error loading APK ...  \"" << apkPath << "\"");
		//Just for debug, print APK contents
		s_APKnbFiles = zip_get_num_files(s_APKArchive);
		TK_INFO("List all files in the APK : " << s_APKnbFiles << " files");
		for (int iii=0; iii<s_APKnbFiles; iii++) {
			const char* name = zip_get_name(s_APKArchive, iii, 0);
			if (name == NULL) {
				TK_ERROR("Error reading zip file name at index " << iii << " : \"" << zip_strerror(s_APKArchive) << "\"");
				return;
			}
			TK_INFO("    File " << iii << " : \"" << name << "\"");
		}
	}
#endif

// for specific device contraint : 
void etk::SetBaseFolderData(const char * folder)
{
	#ifdef __TARGET_OS__Android
		baseFolderData = "assets/";
		s_fileAPK = folder;
		loadAPK(s_fileAPK);
	#else
		TK_WARNING("Not Availlable Outside Android");
	#endif
}

void etk::SetBaseFolderDataUser(const char * folder)
{
	#ifdef __TARGET_OS__Android
		baseFolderDataUser = folder;
	#else
		TK_WARNING("Not Availlable Outside Android");
	#endif
}

void etk::SetBaseFolderCache(const char * folder)
{
	#ifdef __TARGET_OS__Android
		baseFolderCache = folder;
	#else
		TK_WARNING("Not Availlable Outside Android");
	#endif
}


void etk::InitDefaultFolder(const char * applName)
{
	baseApplName = applName;
	char * basicPath = getenv("HOME");
	if (NULL == basicPath) {
		TK_ERROR("ERROR while trying to get the path of the home folder");
		#if defined(__TARGET_OS__Windows)
			baseFolderHome = "c:/";
		#else
			baseFolderHome = "~";
		#endif
	} else {
		baseFolderHome = basicPath;
	}
	#ifndef __TARGET_OS__Android
		
		#ifdef MODE_RELEASE
			baseFolderData  = "/usr/share/";
		#else
			char cCurrentPath[FILENAME_MAX];
			if (!getcwd(cCurrentPath, FILENAME_MAX)) {
				baseFolderData = "./out/Linux/debug/staging/usr/share/";
			} else {
				cCurrentPath[FILENAME_MAX - 1] = '\0';
				baseFolderData  = cCurrentPath;
				baseFolderData += "/out/Linux/debug/staging/usr/share/";
			}
		#endif
		baseFolderData += baseApplName;
		baseFolderData += "/";
		
		baseFolderDataUser  = baseFolderHome;
		baseFolderDataUser += "/.local/share/";
		baseFolderDataUser += baseApplName;
		baseFolderDataUser += "/";
		
		baseFolderCache  = "/tmp/";
		baseFolderCache += baseApplName;
		baseFolderCache += "/";
	#endif
	#ifdef MODE_RELEASE
		if (strncmp("ewolApplNoName",applName, 256) != 0) {
			// start log
		}
	#endif
	TK_INFO("baseFolderHome     : \"" << baseFolderHome << "\"");
	TK_INFO("baseFolderData     : \"" << baseFolderData << "\"");
	TK_INFO("baseFolderDataUser : \"" << baseFolderDataUser << "\"");
	TK_INFO("baseFolderCache    : \"" << baseFolderCache << "\"");
}

etk::UString etk::GetUserHomeFolder(void)
{
	return baseFolderHome;
}



#ifdef __TARGET_OS__Android
bool etk::File::LoadDataZip(void)
{
	if (NULL != m_zipData) {
		return true;
	} else {
		struct zip_file * m_zipPointerFile= zip_fopen_index(s_APKArchive, m_idZipFile, 0);
		if (NULL == m_zipPointerFile) {
			TK_ERROR("Can not find the file name=\"" << GetCompleateName() << "\"");
			return false;
		}
		// get the fileSize .... end read all the data from the zip files
		struct zip_stat zipFileProperty;
		zip_stat_init(&zipFileProperty);
		zip_stat_index(s_APKArchive, m_idZipFile, 0, &zipFileProperty);
		TK_VERBOSE("LOAD data from the files : \"" << GetCompleateName() << "\"");
		/*
		TK_DEBUG("         name=" << zipFileProperty.name);
		TK_DEBUG("         index=" << zipFileProperty.index);
		TK_DEBUG("         crc=" << zipFileProperty.crc);
		TK_DEBUG("         mtime=" << zipFileProperty.mtime);
		TK_DEBUG("         size=" << zipFileProperty.size);
		TK_DEBUG("         comp_size=" << zipFileProperty.comp_size);
		TK_DEBUG("         comp_method=" << zipFileProperty.comp_method);
		TK_DEBUG("         encryption_method=" << zipFileProperty.encryption_method);
		*/
		m_zipDataSize = zipFileProperty.size;
		m_zipData = new char[m_zipDataSize +10];
		if (NULL == m_zipData) {
			TK_ERROR("File allocation ERROR : \"" << GetCompleateName() << "\"");
			zip_fclose(m_zipPointerFile);
			return false;
		}
		memset(m_zipData, 0, m_zipDataSize +10);
		int32_t sizeTmp = zip_fread(m_zipPointerFile, m_zipData, m_zipDataSize);
		if (sizeTmp != m_zipDataSize) {
			TK_ERROR("File load data ERROR : \"" << GetCompleateName() << "\"");
			zip_fclose(m_zipPointerFile);
			delete[] m_zipData;
			return false;
		}
		zip_fclose(m_zipPointerFile);
		m_zipPointerFile = NULL;
		return true;
	}
}
#endif



#undef __class__
#define __class__	"FSNode"


etk::FSNode::FSNode(void) :
	m_userFileName(""),
	m_type(etk::FSN_TYPE_UNKNOW),
	m_typeNode(etk::FSN_UNKNOW),
	m_PointerFile(NULL),
	m_timeCreate(0),
	m_timeModify(0),
	m_timeAccess(0)
	#ifdef __TARGET_OS__Android
		, m_idZipFile(-1),
		m_zipData(NULL),
		m_zipDataSize(-1),
		m_zipReadingOffset(-1)
	#endif
{
	SetName("~");
}


etk::FSNode::FSNode(etk::UString nodeName) :
	m_userFileName(""),
	m_type(etk::FSN_TYPE_UNKNOW),
	m_typeNode(etk::FSN_UNKNOW),
	m_PointerFile(NULL),
	m_timeCreate(0),
	m_timeModify(0),
	m_timeAccess(0)
	#ifdef __TARGET_OS__Android
		, m_idZipFile(-1),
		m_zipData(NULL),
		m_zipDataSize(-1),
		m_zipReadingOffset(-1)
	#endif
{
	PrivateSetName(nodeName);
}


etk::FSNode::~FSNode(void)
{
	if(    NULL != m_PointerFile
	#ifdef __TARGET_OS__Android
	    || NULL != m_zipData
	#endif
	  ) {
		TK_ERROR("Missing to close the file : \"" << *this << "\"");
		FileClose();
	}
}

static etk::UString SimplifyPathAbstractPath(etk::UString input)
{
	int32_t findStartPos = input.FindForward('/') + 1;
	int32_t findPos = input.FindForward('/', findStartPos);
	//TK_DEBUG("Siplify : \"" << input << "\"");
	int32_t preventBadCode = 0;
	while (findPos!=-1)
	{
		//TK_DEBUG("      string=\"" << input << "\"");
		//TK_DEBUG("      '/' @" << findPos);
		if (input.Size()<findPos+1) {
			// no more element ...
			break;
		}
		if(    input[findPos+1] == '/'
		    || (    input[findPos+1] == '.'
		         && input.Size()==findPos+2 )) {
			// cleane the element path
			input.Remove(findPos+1, 1);
			//TK_DEBUG("      Remove // string=\"" << input << "\"");
		} else {
			if (input.Size()<findPos+2) {
				// no more element ...
				break;
			}
			if(    input[findPos+1] == '.'
			    && input[findPos+2] == '.') {
				// cleane the element path
				input.Remove(findStartPos, findPos+3 - findStartPos );
				//TK_DEBUG("      Remove xxx/.. string=\"" << input << "\"");
			} else if(    input[findPos+1] == '.'
			           && input[findPos+2] == '/') {
				// cleane the element path
				input.Remove(findPos+1, 2);
				//TK_DEBUG("      Remove ./ string=\"" << input << "\"");
			} else {
				findStartPos = findPos+1;
			}
		}
		findPos = input.FindForward('/', findStartPos);
		preventBadCode++;
		if (preventBadCode>5000) {
			TK_CRITICAL("ERROR when getting the small path ... this is loop prevention...");
			break;
		}
	}
	/*
	#ifndef __TARGET_OS__Windows
		// for the target that supported the Realpath system :
		char buf[MAX_FILE_NAME];
		memset(buf, 0, MAX_FILE_NAME);
		char * ok = realpath(input.c_str(), buf);
		if (!ok) {
			TK_ERROR("Error to get the real path");
			input = "/";
		} else {
			input = buf;
		}
	#endif
	*/
	//TK_DEBUG("   ==> \"" << input << "\"");
	return input;
}


static void SortElementList(etk::Vector<etk::FSNode *> &list)
{
	// TODO : ...
	/*
	etk::Vector<ewol::FSNode *> tmpList = list;
	list.Clear();
	for(int32_t iii=0; iii<tmpList.Size(); iii++) {
		if (NULL != tmpList[iii]) {
			int32_t findPos = 0;
			for(int32_t jjj=0; jjj<list.Size(); jjj++) {
				//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
				if (list[jjj]!=NULL) {
					if (tmpList[iii]->m_name > list[jjj]->m_name) {
						findPos = jjj+1;
					}
				}
			}
			//EWOL_DEBUG("position="<<findPos);
			list.Insert(findPos, tmpList[iii]);
		}
	}
	*/
}



//#define TK_DBG_MODE TK_VERBOSE
#define TK_DBG_MODE TK_DEBUG
void etk::FSNode::PrivateSetName(etk::UString& newName)
{
	if(    NULL != m_PointerFile
	#ifdef __TARGET_OS__Android
	    || NULL != m_zipData
	#endif
	  ) {
		TK_ERROR("Missing to close the file : \"" << *this << "\"");
		FileClose();
	}
	// set right at NULL ...
	m_rights = 0;
	
	#ifdef __TARGET_OS__Android
		m_idZipFile = -1;
		m_zipData = NULL;
		m_zipDataSize = 0;
		m_zipReadingOffset = 0;
	#endif
	// Reset ALL DATA :
	m_userFileName = "";
	m_type = etk::FSN_TYPE_UNKNOW;
	TK_DBG_MODE("1 : Set Name :              \"" << newName << "\"");
	
	// generate destination name in case of the input error
	etk::UString destFilename;
	if (newName.Size() == 0) {
		// if no name ==> go to the root Folder
		destFilename = "ROOT:";
	} else {
		destFilename = newName;
	}
	
	bool isRoofFolder = false;
	#ifdef __TARGET_OS__Windows
		for (char iii='a' ; iii<='z' iii++) {
			char tmpVal[10];
			char tmpValMaj[10];
			sprintf(tmpVal, "%c:/", iii);
			sprintf(tmpValMaj, "%c:/", iii+'A'-'a');
			if(    true == destFilename.StartWith(tmpVal)
			    || true == destFilename.StartWith(tmpValMaj)) {
				isRoofFolder = true;
				break;
			}
		}
	#else
		isRoofFolder = destFilename.StartWith('/');
	#endif
	if (true == destFilename.StartWith(baseFolderHome) ) {
		destFilename.Remove(0, baseFolderHome.Size());
		m_type = etk::FSN_TYPE_HOME;
	} else if(true == isRoofFolder) {
		#ifdef __TARGET_OS__Windows
			destFilename.Remove(0, 3);
		#else
			destFilename.Remove(0, 1);
		#endif
		m_type = etk::FSN_TYPE_DIRECT;
	} else if(    true == destFilename.StartWith("ROOT:")
	           || true == destFilename.StartWith("root:") ) {
		destFilename.Remove(0, 5);
		m_type = etk::FSN_TYPE_DIRECT;
	} else if(    true == destFilename.StartWith("DIRECT:")
	           || true == destFilename.StartWith("direct:") ) {
		destFilename.Remove(0, 7);
		m_type = etk::FSN_TYPE_DIRECT;
	} else if(    true == destFilename.StartWith("DATA:")
	           || true == destFilename.StartWith("data:") ) {
		destFilename.Remove(0, 5);
		m_type = etk::FSN_TYPE_DATA;
	} else if(    true == destFilename.StartWith("USERDATA:")
	           || true == destFilename.StartWith("userdata:") ) {
		destFilename.Remove(0, 9);
		m_type = etk::FSN_TYPE_USER_DATA;
	} else if(    true == destFilename.StartWith("CACHE:")
	           || true == destFilename.StartWith("cache:") ) {
		destFilename.Remove(0, 6);
		m_type = etk::FSN_TYPE_CACHE;
	} else if(    true == destFilename.StartWith("THEME:")
	           || true == destFilename.StartWith("theme:") ) {
		destFilename.Remove(0, 6);
		m_type = etk::FSN_TYPE_THEME;
	} else if(true == destFilename.StartWith("~")) {
		destFilename.Remove(0, 1);
		m_type = etk::FSN_TYPE_HOME;
	} else if(    true == destFilename.StartWith("HOME:")
	           || true == destFilename.StartWith("home:") ) {
		destFilename.Remove(0, 5);
		m_type = etk::FSN_TYPE_HOME;
	} else {
		// nothing to remove
		//Other type is Relative : 
		m_type = etk::FSN_TYPE_RELATIF;
	}
	m_userFileName = destFilename;
	TK_DBG_MODE("3 : parse done :            [" << m_type << "]->\"" << m_userFileName << "\"");
	
	// Now we reduce the path with all un-needed ../ and other thinks ...
	// TODO : Do it whith link and the other sub thinks ...
	m_userFileName = SimplifyPathAbstractPath(m_userFileName);
	TK_DBG_MODE("4 : Path simplification :   [" << m_type << "]->\"" << m_userFileName << "\"");
	
	// Now we generate the real FS path:
	GenerateFileSystemPath();
	TK_DBG_MODE("5 : file System Real name : \"" << m_systemFileName << "\"");
	
	// now we get all the right if the file existed:
	UpdateFileSystemProperty();
	TK_DBG_MODE("6 : type :                  [" << m_typeNode << "]  right :" << m_rights);
}


bool DirectCheckFile(etk::UString tmpFileNameDirect, bool checkInAPKIfNeeded=false)
{
	#ifdef __TARGET_OS__Android
	if (true == checkInAPKIfNeeded) {
		for (int iii=0; iii<s_APKnbFiles; iii++) {
			const char* name = zip_get_name(s_APKArchive, iii, 0);
			if (name == NULL) {
				TK_ERROR("Can not get pointer on file in the APK file id " << iii);
				continue;
			}
			if (tmpFileNameDirect == name) {
				return true
			}
		}
		return false;
	}
	#endif
	// tmpStat Buffer :
	struct stat statProperty;
	if (-1 == stat(tmpFileNameDirect.c_str(), &statProperty)) {
		return false;
	}
	return true;
}

// Now we generate the real FS path:
void etk::FSNode::GenerateFileSystemPath(void)
{
	switch (m_type)
	{
		default:
		case etk::FSN_TYPE_UNKNOW:
			m_systemFileName = baseFolderHome;
			break;
		case etk::FSN_TYPE_DIRECT:
			m_systemFileName = "/";
			break;
		case etk::FSN_TYPE_RELATIF:
			{
				// Get the command came from the running of the program : 
				char cCurrentPath[FILENAME_MAX];
				if (!getcwd(cCurrentPath, FILENAME_MAX)) {
					TK_WARNING("Can not get the curent path");
					cCurrentPath[0] = '/';
					cCurrentPath[1] = '\0';
				}
				cCurrentPath[FILENAME_MAX - 1] = '\0';
				m_systemFileName = cCurrentPath;
				m_systemFileName += "/";
			}
			break;
		case etk::FSN_TYPE_HOME:
			m_systemFileName = baseFolderHome;
			break;
		case etk::FSN_TYPE_DATA:
			m_systemFileName = baseFolderData;
			break;
		case etk::FSN_TYPE_USER_DATA:
			m_systemFileName = baseFolderDataUser;
			break;
		case etk::FSN_TYPE_CACHE:
			m_systemFileName = baseFolderCache;
			break;
		case etk::FSN_TYPE_THEME:
		case etk::FSN_TYPE_THEME_DATA:
			{
				//etk::UString myCompleateName=baseFolderData + "/theme/";
				etk::UString themeName("");
				etk::UString basicName(m_userFileName);
				int32_t firstPos = m_userFileName.FindForward(':');
				if (-1 != firstPos) {
					// we find a theme name : We extracted it :
					themeName = m_userFileName.Extract(0, firstPos);
					basicName = m_userFileName.Extract(firstPos+1);
				}
				TK_DBG_MODE(" THEME party : \"" << themeName << "\" => \"" << basicName << "\"");
				// search the corect folder : 
				if (themeName == "") {
					TK_WARNING("no theme name detected : set it to \"default\"");
				} else {
					// Selected theme :
					// check in the user data :
					m_systemFileName = baseFolderDataUser + "theme/" + themeName + "/" + basicName;
					if (true==DirectCheckFile(m_systemFileName)) {
						return;
					}
					// check in the Appl data :
					m_systemFileName = baseFolderData + "theme/" + themeName + "/" + basicName;
					if (true==DirectCheckFile(m_systemFileName, true)) {
						m_type = etk::FSN_TYPE_THEME_DATA;
						return;
					}
				}
				themeName = "default";
				// default theme :
				// check in the user data :
				m_systemFileName = baseFolderDataUser + "theme/" + themeName + "/" + basicName;
				if (true==DirectCheckFile(m_systemFileName)) {
					return;
				}
				// check in the Appl data : In every case we return this one ...
				m_systemFileName = baseFolderData + "theme/" + themeName + "/" + basicName;
				m_type = etk::FSN_TYPE_THEME_DATA;
				return;
			}
			break;
	}
	m_systemFileName += m_userFileName;

}
// now we get all the right if the file existed:
void etk::FSNode::UpdateFileSystemProperty(void)
{
	// clean general properties : 
	m_rights.Clear();
	m_timeCreate = 0;
	m_timeModify = 0;
	m_timeAccess = 0;
	m_idOwner = 0;
	m_idGroup = 0;
	// File type is not knowns ...
	m_typeNode=FSN_UNKNOW;
	
	#ifdef __TARGET_OS__Android
	if(    m_type == etk::FSN_TYPE_DATA
	    || m_type == etk::FSN_TYPE_THEME_DATA) {
		for (int iii=0; iii<s_APKnbFiles; iii++) {
			const char* name = zip_get_name(s_APKArchive, iii, 0);
			if (name == NULL) {
				TK_ERROR("Can not get pointer on file in the APK file id " << iii);
				continue;
			}
			if (tmpFileNameDirect == name) {
				m_idZipFile = iii;
				break;
			}
		}
		if(   -1 == m_idZipFile
		   || m_idZipFile >= s_APKnbFiles)) {
			TK_ERROR("File Does not existed ... in APK : \"" << tmpFilename << "\"");
			return;
		}
		// note : Zip does not support other think than file ...
		// TODO : Suport folder parsing ...
		m_typeNode=FSN_FILE;
		m_rights.IsUserReadable(true);
		// TODO : Set the time of the file (time program compilation)
		// TODO : Set the USER ID in the group and the user Id ...
		TK_DBG_MODE("File existed ... in APK : \"" << tmpFilename << "\" ==> id=" << m_idZipFile);
		return;
	}
	#endif
	// tmpStat Buffer :
	struct stat statProperty;
	if (-1 == stat(m_systemFileName.c_str(), &statProperty)) {
		//Normal case when the file does not exist ... ==> the it was in unknow mode ...
		return;
	}
	
	switch (statProperty.st_mode & S_IFMT) {
		case S_IFBLK:  m_typeNode=etk::FSN_BLOCK;     break;
		case S_IFCHR:  m_typeNode=etk::FSN_CHARACTER; break;
		case S_IFDIR:  m_typeNode=etk::FSN_FOLDER;    break;
		case S_IFIFO:  m_typeNode=etk::FSN_FIFO;      break;
		case S_IFLNK:  m_typeNode=etk::FSN_LINK;      break;
		case S_IFREG:  m_typeNode=etk::FSN_FILE;      break;
		case S_IFSOCK: m_typeNode=etk::FSN_SOCKET;    break;
		default:       m_typeNode=etk::FSN_UNKNOW;    break;
	}
	// Right
	m_rights = statProperty.st_mode;
	m_idOwner = (int32_t) statProperty.st_uid;
	m_idGroup = (int32_t) statProperty.st_gid;
	m_timeCreate = statProperty.st_ctime;
	m_timeModify = statProperty.st_mtime;
	m_timeAccess = statProperty.st_atime;
	return;
}


/*
	All Right of the file
*/
bool etk::FSNode::SetRight(etk::FSNodeRight newRight)
{
	// TODO : ...
	TK_ERROR("Can not set the new rights ...");
	return false;
}


/*
	Common API :
*/
void etk::FSNode::SetName(etk::UString newName)
{
	PrivateSetName(newName);
}


etk::UString etk::FSNode::GetNameFolder(void) const
{
	int32_t lastPos = m_systemFileName.FindBack('/');
	if (-1 != lastPos) {
		return m_systemFileName.Extract(0, lastPos);
	}
	return "";
}


etk::UString etk::FSNode::GetName(void) const
{
	etk::UString output;
	switch (m_type)
	{
		default:
		case etk::FSN_TYPE_UNKNOW:
			output = "HOME:";
			break;
		case etk::FSN_TYPE_DIRECT:
			output = "/";
			break;
		case etk::FSN_TYPE_RELATIF:
			output = "";
			break;
		case etk::FSN_TYPE_HOME:
			output = "~";
			break;
		case etk::FSN_TYPE_DATA:
			output = "DATA:";
			break;
		case etk::FSN_TYPE_USER_DATA:
			output = "USERDATA";
			break;
		case etk::FSN_TYPE_CACHE:
			output = "CACHE";
			break;
		case etk::FSN_TYPE_THEME:
		case etk::FSN_TYPE_THEME_DATA:
			output = "THEME:";
			break;
	}
	output += m_userFileName;
	return output;
}


etk::UString etk::FSNode::GetNameFile(void) const
{
	int32_t lastPos = m_systemFileName.FindBack('/');
	if (-1 != lastPos) {
		return m_systemFileName.Extract(lastPos+1);
	}
	return "";
}

etk::UString etk::FSNode::GetRelativeFolder(void) const
{
	etk::UString tmppp = GetName();
	int32_t lastPos = tmppp.FindBack('/');
	if (-1 != lastPos) {
		return tmppp.Extract(0, lastPos+1);
	}
	lastPos = tmppp.FindBack(':');
	if (-1 != lastPos) {
		return tmppp.Extract(0, lastPos+1);
	}
	return "";
}


bool etk::FSNode::Touch(void)
{
	return false;
}
bool etk::FSNode::Remove(void)
{
	return false;
}
uint64_t etk::FSNode::TimeCreated(void) const
{
	return m_timeCreate;
}

etk::UString etk::FSNode::TimeCreatedString(void) const
{
	time_t tmpVal = (int32_t)m_timeCreate;
	return ctime(&tmpVal);
}

uint64_t etk::FSNode::TimeModified(void) const
{
	return m_timeModify;
}

etk::UString etk::FSNode::TimeModifiedString(void) const
{
	time_t tmpVal = (int32_t)m_timeModify;
	return ctime(&tmpVal);
}

uint64_t etk::FSNode::TimeAccessed(void) const
{
	return m_timeAccess;
}

etk::UString etk::FSNode::TimeAccessedString(void) const
{
	time_t tmpVal = (int32_t)m_timeAccess;
	return ctime(&tmpVal);
}

/*
	Operator :
*/
const etk::FSNode& etk::FSNode::operator=  (const etk::FSNode &obj )
{
	if( this != &obj ) // avoid copy to itself
	{
		if(    NULL != m_PointerFile
		#ifdef __TARGET_OS__Android
		    || NULL != m_zipData
		#endif
		   ) {
			TK_ERROR("Missing close the file : " << *this);
			FileClose();
			m_PointerFile = NULL;
		}
		#ifdef __TARGET_OS__Android
			m_idZipFile = etkF.m_idZipFile;
			m_zipData = NULL;
			m_zipDataSize = 0;
			m_zipReadingOffset = 0;
		#endif
		etk::UString tmppp = obj.GetName();
		PrivateSetName(tmppp);
	}
	return *this;
}
bool etk::FSNode::operator== (const etk::FSNode &obj ) const
{
	if( this != &obj ) {
		if(    obj.m_userFileName == m_userFileName
		    && obj.m_systemFileName == m_systemFileName
		    && obj.m_type == m_type ) {
			return true;
		} else {
			return false;
		}
		return true;
	}
	return true;
}

bool etk::FSNode::operator!= (const etk::FSNode &obj ) const
{
	return !(*this == obj);
}

etk::CCout& etk::operator <<(etk::CCout &os, const etk::FSNode &obj)
{
	os << "[" << obj.m_type << "]->\"" << obj.m_userFileName << "\"";
	return os;
}

etk::CCout& etk::operator <<(etk::CCout &os, const etk::FSNType_te &obj)
{
	switch (obj)
	{
		case etk::FSN_TYPE_UNKNOW:
			os << "FSN_TYPE_UNKNOW";
			break;
		case etk::FSN_TYPE_DIRECT:
			os << "FSN_TYPE_DIRECT";
			break;
		case etk::FSN_TYPE_RELATIF:
			os << "FSN_TYPE_RELATIF";
			break;
		case etk::FSN_TYPE_HOME:
			os << "FSN_TYPE_HOME";
			break;
		case etk::FSN_TYPE_DATA:
			os << "FSN_TYPE_DATA";
			break;
		case etk::FSN_TYPE_USER_DATA:
			os << "FSN_TYPE_USER_DATA";
			break;
		case etk::FSN_TYPE_CACHE:
			os << "FSN_TYPE_CACHE";
			break;
		case etk::FSN_TYPE_THEME:
			os << "FSN_TYPE_THEME";
			break;
		case etk::FSN_TYPE_THEME_DATA:
			os << "FSN_TYPE_THEME(DATA)";
			break;
		default:
			os << "FSN_TYPE_????";
			break;
	}
	return os;
}

etk::CCout& etk::operator <<(etk::CCout &os, const etk::typeNode_te &obj)
{
	switch (obj)
	{
		case etk::FSN_UNKNOW:
			os << "FSN_UNKNOW";
			break;
		case etk::FSN_BLOCK:
			os << "FSN_BLOCK";
			break;
		case etk::FSN_CHARACTER:
			os << "FSN_CHARACTER";
			break;
		case etk::FSN_FOLDER:
			os << "FSN_FOLDER";
			break;
		case etk::FSN_FIFO:
			os << "FSN_FIFO";
			break;
		case etk::FSN_LINK:
			os << "FSN_LINK";
			break;
		case etk::FSN_FILE:
			os << "FSN_FILE";
			break;
		case etk::FSN_SOCKET:
			os << "FSN_SOCKET";
			break;
		default:
			os << "FSN_????";
			break;
	}
	return os;
}

/*
	Folder specific :
*/
int32_t etk::FSNode::FolderCount(void)
{
	return 0;
}
etk::Vector<etk::FSNode *> etk::FSNode::FolderGetSubList(bool showHidenFile, bool getFolderAndOther, bool getFile, bool temporaryFile)
{
	etk::Vector<etk::FSNode*> tmpp;
	if (m_typeNode != etk::FSN_FOLDER ) {
		return tmpp;
	}
	
	// regenerate the next list :
	etk::FSNode * tmpEmement;
	DIR *dir = NULL;
	struct dirent *ent = NULL;
	dir = opendir(m_systemFileName.c_str());
	if (dir != NULL) {
		// for each element in the drectory...
		while ((ent = readdir(dir)) != NULL) {
			etk::UString tmpName(ent->d_name);
			TK_DEBUG(" search in folder\"" << tmpName << "\"");
			if(    tmpName=="." 
			    || tmpName==".." ) {
				// do nothing ...
				continue;
			}
			if(    false == tmpName.StartWith(".")
			    || true == showHidenFile) {
				tmpEmement = new etk::FSNode(GetRelativeFolder()+tmpName);
				if (NULL == tmpEmement) {
					TK_ERROR("allocation error ... of ewol::FSNode");
					continue;
				}
				if(tmpEmement->GetNodeType() == etk::FSN_FILE) {
					if (true == getFile) {
						tmpp.PushBack(tmpEmement);
					} else {
						delete(tmpEmement);
						tmpEmement = NULL;
					}
				} else if (getFolderAndOther) {
					tmpp.PushBack(tmpEmement);
				} else {
					delete(tmpEmement);
					tmpEmement = NULL;
				}
			}
		}
		closedir(dir);
	} else {
		TK_ERROR("could not open directory : \"" << *this << "\"");
	}
	
	// reorder the files
	SortElementList(tmpp);
	
	return tmpp;
}
etk::FSNode etk::FSNode::FolderGetParent(void)
{
	etk::FSNode tmpp;
	return tmpp;
}

/*
	File Specific :
*/
bool etk::FSNode::FileHasExtention(void)
{
	int32_t lastPos = m_userFileName.FindBack('.');
	if(    -1 != lastPos                       // not find the .
	    && 0  != lastPos                       // Find a . at the fist position .jdlskjdfklj ==> hiden file
	    && m_userFileName.Size() != lastPos ) // Remove file ended with .
	{
		return true;
	} else {
		return false;
	}
}

etk::UString etk::FSNode::FileGetExtention(void)
{
	etk::UString tmpExt = "";
	int32_t lastPos = m_userFileName.FindBack('.');
	if(    -1 != lastPos                       // not find the .
	    && 0  != lastPos                       // Find a . at the fist position .jdlskjdfklj ==> hiden file
	    && m_userFileName.Size() != lastPos ) // Remove file ended with .
	{
		// Get the FileName
		tmpExt = m_userFileName.Extract(lastPos+1);
	}
	return tmpExt;
}

uint64_t etk::FSNode::FileSize(void)
{
	if (etk::FSN_FILE != m_typeNode) {
		TK_ERROR("pppppppppppppppppppppp" << m_typeNode);
		return 0;
	}
	#ifdef __TARGET_OS__Android
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {
		if (true == LoadDataZip()) {
			return m_zipDataSize;
		}
		return 0;
	}
	#endif
	// Note : this is a proper methode to get the file size for Big files ... otherwithe the size is limited at 2^31 bytes
	// tmpStat Buffer :
	struct stat statProperty;
	if (-1 == stat(m_systemFileName.c_str(), &statProperty)) {
		//Normal case when the file does not exist ... ==> the it was in unknow mode ...
		TK_ERROR("mlkmlkmlkmlkmlkmlk");
		return 0;
	}
	TK_INFO(" file size : " << (int64_t)statProperty.st_size << " bytes");
	if ((uint64_t)statProperty.st_size<=0) {
		return 0;
	}
	return (uint64_t)statProperty.st_size;
}


bool etk::FSNode::FileOpenRead(void)
{
	#ifdef __TARGET_OS__Android
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {
		return LoadDataZip();
	}
	#endif
	if (NULL != m_PointerFile) {
		TK_CRITICAL("File Already open : " << *this);
		return true;
	}
	m_PointerFile=fopen(m_systemFileName.c_str(),"rb");
	if(NULL == m_PointerFile) {
		TK_ERROR("Can not find the file " << *this );
		return false;
	}
	return true;
}
bool etk::FSNode::FileOpenWrite(void)
{
	#ifdef __TARGET_OS__Android
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {
		return false;
	}
	#endif
	if (NULL != m_PointerFile) {
		TK_CRITICAL("File Already open : " << *this);
		return true;
	}
	m_PointerFile=fopen(m_systemFileName.c_str(),"wb");
	if(NULL == m_PointerFile) {
		TK_ERROR("Can not find the file " << *this);
		return false;
	}
	return true;
}
bool etk::FSNode::FileClose(void)
{
	#ifdef __TARGET_OS__Android
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {
		if (NULL == m_zipData) {
			TK_CRITICAL("File Already closed : " << *this);
			return false;
		}
		delete[] m_zipData;
		m_zipData = NULL;
		m_zipDataSize = 0;
		m_zipReadingOffset = 0;
		return true;
	}
	#endif
	if (NULL == m_PointerFile) {
		TK_CRITICAL("File Already closed : " << *this);
		return false;
	}
	fclose(m_PointerFile);
	m_PointerFile = NULL;
	return true;
}
char* etk::FSNode::FileGets(char * elementLine, int32_t maxData)
{
	memset(elementLine, 0, maxData);
	#ifdef __TARGET_OS__Android
	char * element = elementLine;
	int32_t outSize = 0;
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {//char * tmpData = internalDataFiles[iii].data + m_readingOffset;
		if (NULL == m_zipData) {
			element[0] = '\0';
			return NULL;
		}
		if (m_zipReadingOffset>m_zipDataSize) {
			element[0] = '\0';
			return NULL;
		}
		while (m_zipData[m_zipReadingOffset] != '\0') {
			if(    m_zipData[m_zipReadingOffset] == '\n'
			    || m_zipData[m_zipReadingOffset] == '\r')
			{
				*element = m_zipData[m_zipReadingOffset];
				element++;
				m_zipReadingOffset++;
				*element = '\0';
				return elementLine;
			}
			*element = m_zipData[m_zipReadingOffset];
			element++;
			m_zipReadingOffset++;
			if (m_zipReadingOffset>m_zipDataSize) {
				*element = '\0';
				return elementLine;
			}
			// check maxData Size ...
			if (outSize>=maxData-1) {
				*element = '\0';
				return elementLine;
			}
			outSize++;
		}
		if (outSize==0) {
			return NULL;
		} else {
			// send last line
			return elementLine;
		}
	}
	#endif
	return fgets(elementLine, maxData, m_PointerFile);
}
int32_t etk::FSNode::FileRead(void * data, int32_t blockSize, int32_t nbBlock)
{
	#ifdef __TARGET_OS__Android
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {
		if (NULL == m_zipData) {
			((char*)data)[0] = '\0';
			return 0;
		}
		int32_t dataToRead = blockSize * nbBlock;
		if (dataToRead + m_zipReadingOffset > m_zipDataSize) {
			nbBlock = ((m_zipDataSize - m_zipReadingOffset) / blockSize);
			dataToRead = blockSize * nbBlock;
		}
		memcpy(data, &m_zipData[m_zipReadingOffset], dataToRead);
		m_zipReadingOffset += dataToRead;
		return nbBlock;
	}
	#endif
	return fread(data, blockSize, nbBlock, m_PointerFile);
}
int32_t etk::FSNode::FileWrite(void * data, int32_t blockSize, int32_t nbBlock)
{
	#ifdef __TARGET_OS__Android
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {
		TK_CRITICAL("Can not write on data inside APK : " << *this);
		return 0;
	}
	#endif
	return fwrite(data, blockSize, nbBlock, m_PointerFile);
}
bool etk::FSNode::FileSeek(long int offset, int origin)
{
	#ifdef __TARGET_OS__Android
	if(    etk::FSN_TYPE_DATA == m_type
	    || etk::FSN_TYPE_THEME_DATA == m_type) {
		if (NULL == m_zipData) {
			return false;
		}
		int32_t positionEnd = 0;
		switch(origin) {
			case SEEK_END:
				positionEnd = m_zipDataSize;
				break;
			case SEEK_CUR:
				positionEnd = m_zipReadingOffset;
				break;
			default:
				positionEnd = 0;
				break;
		}
		positionEnd += offset;
		if (positionEnd < 0) {
			positionEnd = 0;
		} else if (positionEnd > m_zipDataSize) {
			positionEnd = m_zipDataSize;
		}
		m_zipReadingOffset = positionEnd;
		return true;
	}
	#endif
	fseek(m_PointerFile, offset, origin);
	if(ferror(m_PointerFile)) {
		return false;
	} else {
		return true;
	}
}

