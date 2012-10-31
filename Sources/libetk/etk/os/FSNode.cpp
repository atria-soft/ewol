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
		baseFolderDataUser += "/.";
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
	m_rights(0),
	m_PointerFile(NULL)
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
	m_rights(0),
	m_PointerFile(NULL)
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
	TK_DBG_MODE("1 :Set Name : " << newName );
	
	// generate destination name in case of the input error
	etk::UString destFilename;
	if (newName.Size() == 0) {
		// if no name ==> go to the root Folder
		destFilename = "ROOT:";
	} else {
		destFilename = newName;
	}
	
	#ifdef __TARGET_OS__Windows
		bool isRoofFolder = false;
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
		TK_DBG_MODE("2 : Get file Name : " << destFilename << "start with 'X:/'=" << isRoofFolder);
		if(true == isRoofFolder) {
			destFilename.Remove(0, 3);
	#else
		TK_DBG_MODE("2 : Get file Name : " << destFilename << "start with '/'=" << destFilename.StartWith('/'));
		if(true == destFilename.StartWith('/')){
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
	} else if (true == destFilename.StartWith(baseFolderHome) ) {
		destFilename.Remove(0, baseFolderHome.Size());
		m_type = etk::FSN_TYPE_HOME;
	} else {
		// nothing to remove
		//Other type is Relative : 
		m_type = etk::FSN_TYPE_RELATIF;
	}
	m_userFileName = destFilename;
	TK_DBG_MODE("3 : parse done : [" << m_type << "->\"" << m_userFileName << "\"");
	
	// Now we reduce the path with all un-needed
	m_userFileName = etk::tool::SimplifyPath(m_userFileName);
	TK_DBG_MODE("4 : Path simplification : [" << m_type << "->\"" << m_userFileName << "\"");
	
	#ifdef __TARGET_OS__Android
		// Get on android the property of the file ID ... in zip ... ==> maybe done this later ...
		if (m_type == etk::FSN_TYPE_DATA) {
			etk::UString tmpFilename = baseFolderData + m_userFileName;
			for (int iii=0; iii<s_APKnbFiles; iii++) {
				const char* name = zip_get_name(s_APKArchive, iii, 0);
				if (name == NULL) {
					TK_ERROR("Can not get pointer on file in the APK file id " << iii);
					continue;
				}
				if (tmpFilename == name) {
					m_idZipFile = iii;
					break;
				}
			}
			if (-1 == m_idZipFile) {
				TK_ERROR("File Does not existed ... in APK : \"" << tmpFilename << "\"");
			} else {
				TK_DBG_MODE("File existed ... in APK : \"" << tmpFilename << "\" ==> id=" << m_idZipFile);
			}
		}
	#endif
	
	// TODO : Get rights
	// TODO : Check if it is a file or a folder ...
}


etk::UString etk::FSNode::GetFileSystemName(void) const
{
	etk::UString output;
	switch (m_type)
	{
		default:
		case etk::FSN_TYPE_UNKNOW:
			output = baseFolderHome;
			break;
		case etk::FSN_TYPE_DIRECT:
			output = "/";
			break;
		case etk::FSN_TYPE_RELATIF:
			{
				// Get the command came from the running of the program : 
				char cCurrentPath[FILENAME_MAX];
				if (!getcwd(cCurrentPath, FILENAME_MAX)) {
					TK_WARNING("Can not get the curent path");
					output = "/";
				}
				cCurrentPath[FILENAME_MAX - 1] = '\0';
				output = cCurrentPath;
				output += "/";
			}
			break;
		case etk::FSN_TYPE_HOME:
			output = baseFolderHome;
			break;
		case etk::FSN_TYPE_DATA:
			output = baseFolderData;
			break;
		case etk::FSN_TYPE_USER_DATA:
			output = baseFolderDataUser;
			break;
		case etk::FSN_TYPE_CACHE:
			output = baseFolderCache;
			break;
		case etk::FSN_TYPE_THEME:
			output = baseFolderData + "/theme/";
			break;
	}
	output += m_userFileName;
	return output;
}


/*
	All Right of the file
*/
bool etk::FSNode::Exist(void)
{
	#ifdef __TARGET_OS__Android
	if (etk::FSN_TYPE_DATA == m_type) {
		if (m_idZipFile >= -1  && m_idZipFile < s_APKnbFiles) {
			return true;
		}
		return false;
	}
	#endif
	FILE *myFile=NULL;
	etk::UString myCompleateName=GetFileSystemName();
	
	myFile=fopen(myCompleateName.c_str(),"rb");
	if(NULL == myFile) {
		TK_DEBUG("try to open : " << myCompleateName.c_str());
		return false;
	}
	fclose(myFile);
	return true;
}

bool etk::FSNode::IsFile(void)
{
	return ((m_rights&etk::RIGHT_FILE)!=0)?true:false;
}

bool etk::FSNode::IsFolder(void)
{
	return ((m_rights&etk::RIGHT_FOLDER)!=0)?true:false;
}

bool etk::FSNode::IsLink(void)
{
	return ((m_rights&etk::RIGHT_LINK)!=0)?true:false;
}

// User
bool etk::FSNode::IsUserReadable(void)
{
	return ((m_rights&etk::RIGHT_USER_READ)!=0)?true:false;
}

bool etk::FSNode::IsUserWritable(void)
{
	return ((m_rights&etk::RIGHT_USER_WRITE)!=0)?true:false;
}

bool etk::FSNode::IsUserRunable(void)
{
	return ((m_rights&etk::RIGHT_USER_EXECUTE)!=0)?true:false;
}

void etk::FSNode::SetUserReadable(bool newStatus)
{
	
}

void etk::FSNode::SetUserWritable(bool newStatus)
{
	
}

void etk::FSNode::SetUserRunable(bool newStatus)
{
	
}

// group
bool etk::FSNode::IsGroupReadable(void)
{
	return ((m_rights&etk::RIGHT_GROUP_READ)!=0)?true:false;
}

bool etk::FSNode::IsGroupWritable(void)
{
	return ((m_rights&etk::RIGHT_GROUP_WRITE)!=0)?true:false;
}

bool etk::FSNode::IsGroupRunable(void)
{
	return ((m_rights&etk::RIGHT_GROUP_EXECUTE)!=0)?true:false;
}

void etk::FSNode::SetGroupReadable(bool newStatus)
{
	
}

void etk::FSNode::SetGroupWritable(bool newStatus)
{
	
}

void etk::FSNode::SetGroupRunable(bool newStatus)
{
	
}

// other
bool etk::FSNode::IsOtherReadable(void)
{
	return ((m_rights&etk::RIGHT_OTHER_READ)!=0)?true:false;
}

bool etk::FSNode::IsOtherWritable(void)
{
	return ((m_rights&etk::RIGHT_OTHER_WRITE)!=0)?true:false;
}

bool etk::FSNode::IsOtherRunable(void)
{
	return ((m_rights&etk::RIGHT_OTHER_EXECUTE)!=0)?true:false;
}

void etk::FSNode::SetOtherReadable(bool newStatus)
{
	
}

void etk::FSNode::SetOtherWritable(bool newStatus)
{
	
}

void etk::FSNode::SetOtherRunable(bool newStatus)
{
	
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
	etk::UString myCompleateName=GetFileSystemName();
	
	int32_t lastPos = myCompleateName.FindBack('/');
	if (-1 != lastPos) {
		return myCompleateName.Extract(0, lastPos);
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
			output = "THEME:";
			break;
	}
	output += m_userFileName;
	return output;
}


etk::UString etk::FSNode::GetNameFile(void) const
{
	etk::UString myCompleateName=GetFileSystemName();
	
	int32_t lastPos = myCompleateName.FindBack('/');
	if (-1 != lastPos) {
		return myCompleateName.Extract(lastPos+1);
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
uint64_t etk::FSNode::TimeCreated(void)
{
	return 0;
}
uint64_t etk::FSNode::TimeModified(void)
{
	return 0;
}

/*
	Operator :
*/
const etk::FSNode& etk::FSNode::operator=  (const etk::FSNode &obj )
{
	if( this != &obj ) // avoid copy to itself
	{
		m_userFileName = obj.m_userFileName;
		m_type = obj.m_type;
		m_rights = obj.m_rights;
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
	}
	return *this;
}
bool etk::FSNode::operator== (const etk::FSNode &obj ) const
{
	if( this != &obj ) {
		if(    obj.m_userFileName == m_userFileName
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
		default:
			os << "FSN_TYPE_????";
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
etk::Vector<etk::FSNode> etk::FSNode::FolderGetSubList(void)
{
	etk::Vector<etk::FSNode> tmpp;
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
int32_t etk::FSNode::FileSize(void)
{
	#ifdef __TARGET_OS__Android
	if (etk::FILE_TYPE_DATA == m_type) {
		if (true == LoadDataZip()) {
			return m_zipDataSize;
		}
		return 0;
	}
	#endif
	FILE *myFile=NULL;
	etk::UString myCompleateName = GetFileSystemName();
	myFile=fopen(myCompleateName.c_str(),"rb");
	if(NULL == myFile) {
		//EWOL_ERROR("Can not find the file " << *this);
		return -1;
	}
	int32_t size = 0;
	fseek(myFile, 0, SEEK_END);
	size = ftell(myFile);
	fseek(myFile, 0, SEEK_SET);
	fclose(myFile);
	return size;
}
bool etk::FSNode::FileOpenRead(void)
{
	#ifdef __TARGET_OS__Android
	if (etk::FILE_TYPE_DATA == m_type) {
		return LoadDataZip();
	}
	#endif
	if (NULL != m_PointerFile) {
		TK_CRITICAL("File Already open : " << *this);
		return true;
	}
	etk::UString myCompleateName = GetFileSystemName();
	m_PointerFile=fopen(myCompleateName.c_str(),"rb");
	if(NULL == m_PointerFile) {
		TK_ERROR("Can not find the file " << *this );
		return false;
	}
	return true;
}
bool etk::FSNode::FileOpenWrite(void)
{
	#ifdef __TARGET_OS__Android
	if (etk::FILE_TYPE_DATA == m_type) {
		return false;
	}
	#endif
	if (NULL != m_PointerFile) {
		TK_CRITICAL("File Already open : " << *this);
		return true;
	}
	etk::UString myCompleateName = GetFileSystemName();
	m_PointerFile=fopen(myCompleateName.c_str(),"wb");
	if(NULL == m_PointerFile) {
		TK_ERROR("Can not find the file " << *this);
		return false;
	}
	return true;
}
bool etk::FSNode::FileClose(void)
{
	#ifdef __TARGET_OS__Android
	if (etk::FILE_TYPE_DATA == m_type) {
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
	if (etk::FILE_TYPE_DATA == m_type) {//char * tmpData = internalDataFiles[iii].data + m_readingOffset;
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
	if (etk::FILE_TYPE_DATA == m_type) {
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
	if (etk::FILE_TYPE_DATA == m_type) {
		TK_CRITICAL("Can not write on data inside APK : " << *this);
		return 0;
	}
	#endif
	return fwrite(data, blockSize, nbBlock, m_PointerFile);
}
bool etk::FSNode::FileSeek(long int offset, int origin)
{
	#ifdef __TARGET_OS__Android
	if (etk::FILE_TYPE_DATA == m_type) {
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
