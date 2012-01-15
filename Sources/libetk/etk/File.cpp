/**
 *******************************************************************************
 * @file etk/File.cpp
 * @brief Ewol Tool Kit : File folder and name abstraction (Sources)
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


#include <etk/Types.h>
#include <etk/DebugInternal.h>
#include <etk/File.h>
#include <unistd.h>

#ifdef DATA_INTERNAL_BINARY
#	include <GeneratedData.h>
#elif defined(DATA_IN_APK)
#	include <stdio.h>
#	include <zip/zip.h>
#endif



#undef __class__
#define __class__	"etk::File"


etk::CCout& etk::operator <<(etk::CCout &os, const etk::File &obj)
{
	os << obj.m_folder;
	os << "/";
	os << obj.m_shortFilename;
	return os;
}

etk::File::File(etk::String &filename, etk::FileType_te type, int32_t LineNumber)
{
	m_lineNumberOpen = LineNumber;
	m_PointerFile = NULL;
	SetCompleateName(filename, type);
}


etk::File::File(const char  *filename, etk::FileType_te type, int32_t LineNumber)
{
	etk::String tmpString = filename;
	m_lineNumberOpen = LineNumber;
	m_PointerFile = NULL;
	SetCompleateName(tmpString, type);
}


etk::File::File(etk::String &filename, etk::String &folder, etk::FileType_te type, int32_t lineNumber)
{
	etk::String tmpString = folder;
	tmpString += '/';
	tmpString += filename;
	m_PointerFile = NULL;
	SetCompleateName(tmpString, type);
	m_lineNumberOpen = lineNumber;
}

etk::File::~File(void)
{
	// nothing to do ...
	if (NULL != m_PointerFile) {
		TK_ERROR("Missing close the file : \"" << GetCompleateName() << "\"");
		fClose();
	}
}


etk::String etk::File::GetFolder(void) const
{
	return m_folder;
}

etk::String etk::File::GetShortFilename(void) const
{
	return m_shortFilename;
}

etk::String etk::File::GetCompleateName(void) const
{
	etk::String out;
	out  = m_folder;
	out += '/';
	out += m_shortFilename;
	return out;
}


const etk::File& etk::File::operator= (const etk::File &etkF )
{
	if( this != &etkF ) // avoid copy to itself
	{
		m_folder = etkF.m_folder;
		m_shortFilename = etkF.m_shortFilename;
		m_lineNumberOpen = etkF.m_lineNumberOpen;
		m_type = etkF.m_type;
		if (NULL != m_PointerFile) {
			TK_ERROR("Missing close the file : \"" << GetCompleateName() << "\"");
			fClose();
		}
		#ifdef DATA_INTERNAL_BINARY
			m_idInternal = etkF.m_idInternal;
			m_readingOffset = 0;
		#elif defined(DATA_IN_APK)
			m_idZipFile = etkF.m_idZipFile;
			m_zipData = NULL;
			m_zipDataSize = 0;
			m_zipReadingOffset = 0;
			//m_zipPointerFile = NULL;
		#endif
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::File::operator== (const etk::File &etkF) const
{
	if( this != &etkF ) {
		if (etkF.GetCompleateName() == GetCompleateName() ) {
			return true;
		} else {
			return false;
		}
		return true;
	}
	return true;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::File::operator!= (const etk::File &etkF) const
{
	return !(*this == etkF);
}


etk::String baseFolderData = "assets/";
#ifdef DATA_IN_APK
static etk::String s_fileAPK = "";

static struct zip * s_APKArchive = NULL;
static int32_t      s_APKnbFiles = 0;


static void loadAPK (const char* apkPath)
{
	TK_DEBUG("Loading APK \"" << apkPath << "\"");
	s_APKArchive = zip_open(apkPath, 0, NULL);
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
etk::String baseFolderDataUser = "~/.tmp/userData/";
etk::String baseFolderCache = "~/.tmp/cache/";
// for specific device contraint : 
void etk::SetBaseFolderData(const char * folder)
{
	#ifdef DATA_INTERNAL_BINARY
		for(int32_t iii=0; iii<internalDataFilesSize; iii++) {
			TK_DEBUG("Internal date : \"" << internalDataFiles[iii].filename << "\" size=" << internalDataFiles[iii].fileLenght);
		}
	#elif defined(DATA_IN_APK)
		baseFolderData = "assets/";
		s_fileAPK = folder;
		loadAPK(s_fileAPK.c_str());
	#else
		baseFolderData = folder;
	#endif
}
void etk::SetBaseFolderDataUser(const char * folder)
{
	baseFolderDataUser = folder;
}
void etk::SetBaseFolderCache(const char * folder)
{
	baseFolderCache = folder;
}


void etk::File::SetCompleateName(etk::String &newFilename, etk::FileType_te type)
{
	char buf[MAX_FILE_NAME];
	memset(buf, 0, MAX_FILE_NAME);
	char * ok;
	#ifdef DATA_INTERNAL_BINARY
	m_idInternal = -1;
	#elif defined(DATA_IN_APK)
	m_idZipFile = -1;
	m_zipData = NULL;
	m_zipDataSize = 0;
	m_zipReadingOffset = 0;
	//m_zipPointerFile = NULL;
	#endif
	// Reset ALL DATA : 
	m_folder = "";
	m_shortFilename = "";
	m_lineNumberOpen = 0;
	TK_VERBOSE("1 :Set Name : " << newFilename );
	etk::String destFilename;
	if (newFilename.Size() == 0) {
		destFilename = "no-name";
	} else {
		destFilename = newFilename;
	}
	TK_VERBOSE("2 : Get file Name : " << destFilename );
	if ('/' == *destFilename.c_str()) {
		m_type = etk::FILE_TYPE_DIRECT;
		if (type != etk::FILE_TYPE_DIRECT) {
			TK_WARNING("Incompatible type with a file=\"" << newFilename << "\" ==> force it in direct mode ...");
		}
	} else {
		if (type == etk::FILE_TYPE_DIRECT) {
			//TK_WARNING("Incompatible type with a file=\"" << newFilename << "\" ==> force it in FILE_TYPE_DATA mode ...");
			//m_type = etk::FILE_TYPE_DATA;
			m_type = etk::FILE_TYPE_DIRECT;
			// add current path : 
			// Get the command came from the running of the program : 
			char cCurrentPath[FILENAME_MAX];
			if (!getcwd(cCurrentPath, FILENAME_MAX)) {
				return;
			}
			cCurrentPath[FILENAME_MAX - 1] = '\0';
			etk::String tmpFilename = destFilename;
			destFilename = cCurrentPath;
			destFilename += '/';
			destFilename += tmpFilename;
			
			
		} else {
			m_type = type;
		}
	}
	bool needUnpack = false;
	#if ETK_DEBUG_LEVEL > 2
	char *mode = NULL;
	#endif
	switch (m_type)
	{
		case etk::FILE_TYPE_DATA:
			{
				#if ETK_DEBUG_LEVEL > 2
				mode = "FILE_TYPE_DATA";
				#endif
				#ifdef DATA_INTERNAL_BINARY
					for(int32_t iii=0; iii<internalDataFilesSize; iii++) {
						if (destFilename == internalDataFiles[iii].filename) {
							m_idInternal = iii;
							break;
						}
					}
					if (-1 == m_idInternal) {
						TK_ERROR("File Does not existed ... in memory : \"" << destFilename << "\"");
					}
				#elif defined(DATA_IN_APK)
					etk::String tmpFilename = baseFolderData + destFilename;
					for (int iii=0; iii<s_APKnbFiles; iii++) {
						const char* name = zip_get_name(s_APKArchive, iii, 0);
						if (name == NULL) {
							return;
						}
						if (tmpFilename == name) {
							m_idZipFile = iii;
							break;
						}
					}
					if (-1 == m_idZipFile) {
						TK_ERROR("File Does not existed ... in APK : \"" << tmpFilename << "\"");
					} else {
						TK_INFO("File existed ... in APK : \"" << tmpFilename << "\" ==> id=" << m_idZipFile);
					}
				#else
					//etk::String tmpFilename = destFilename;
					//destFilename = baseFolderData;
					//destFilename += tmpFilename;
				#endif
			}
			break;
		case etk::FILE_TYPE_USER_DATA:
			{
				#if ETK_DEBUG_LEVEL > 2
				mode = "FILE_TYPE_USER_DATA";
				#endif
				etk::String tmpFilename = destFilename;
				destFilename = baseFolderDataUser;
				destFilename += tmpFilename;
			}
			needUnpack = true;
			break;
		case etk::FILE_TYPE_CACHE:
			{
				#if ETK_DEBUG_LEVEL > 2
				mode = "FILE_TYPE_CACHE";
				#endif
				etk::String tmpFilename = destFilename;
				destFilename = baseFolderCache;
				destFilename += tmpFilename;
			}
			needUnpack = true;
			break;
		default:
			// nothing to do ...
			#if ETK_DEBUG_LEVEL > 2
			mode = "FILE_TYPE_DIRECT";
			#endif
			needUnpack = true;
			break;
	}
	TK_VERBOSE("3 : Get file Name : " << destFilename );
	if (true == needUnpack) {
		// Get the real Path of the current File
		ok = realpath(destFilename.c_str(), buf);
		if (!ok) {
			int32_t lastPos = destFilename.FindBack('/');
			if (-1 != lastPos) {
				// Get the FileName
				etk::String tmpFilename = destFilename.Extract(lastPos+1);
				destFilename.Remove(lastPos, destFilename.Size() - lastPos);
				TK_VERBOSE("try to find :\"" << destFilename << "\" / \"" << tmpFilename << "\" ");
				ok = realpath(destFilename.c_str(), buf);
				if (!ok) {
					TK_VERBOSE("Can not find real Path name of \"" << destFilename << "\"");
					m_shortFilename = tmpFilename;
					m_folder        = destFilename;
				} else {
					// ALL is OK ...
					m_shortFilename = tmpFilename;
					m_folder        = destFilename;
				}
			} else {
				TK_WARNING("file : \"" << destFilename << "\" ==> No data???");
				// Basic ERROR ...
				m_shortFilename = destFilename;
			}
		} else {
			destFilename = buf;
			int32_t lastPos = destFilename.FindBack('/');
			if (-1 != lastPos) {
				m_shortFilename = destFilename.Extract(lastPos+1);
				m_folder        = destFilename.Extract(0, lastPos);
			} else {
				// Basic ERROR ...
				TK_WARNING("file : \"" << destFilename << "\" ==> No data???");
				m_shortFilename = destFilename;
			}
		}
	} else {
		int32_t lastPos = destFilename.FindBack('/');
		if (-1 != lastPos) {
			m_shortFilename = destFilename.Extract(lastPos+1);
			m_folder        = destFilename.Extract(0, lastPos);
		} else {
			// Basic ERROR ...
			TK_WARNING("file : \"" << destFilename << "\" ==> No data???");
			m_shortFilename = destFilename;
		}
	}
	TK_DEBUG("Set FileName :\"" << m_folder << "\" / \"" << m_shortFilename << "\" mode=" << mode);
}

int32_t etk::File::GetLineNumber(void)
{
	return m_lineNumberOpen;
}

void etk::File::SetLineNumber(int32_t newline)
{
	m_lineNumberOpen = newline;
}

bool etk::File::HasExtention(void)
{
	int32_t lastPos = m_shortFilename.FindBack('.');
	if(    -1 != lastPos                       // not find the .
	    && 0  != lastPos                       // Find a . at the fist position .jdlskjdfklj ==> hiden file
	    && m_shortFilename.Size() != lastPos ) // Remove file ended with .
	{
		return true;
	} else {
		return false;
	}
}


etk::String etk::File::GetExtention(void)
{
	etk::String tmpExt = "";
	int32_t lastPos = m_shortFilename.FindBack('.');
	if(    -1 != lastPos                       // not find the .
	    && 0  != lastPos                       // Find a . at the fist position .jdlskjdfklj ==> hiden file
	    && m_shortFilename.Size() != lastPos ) // Remove file ended with .
	{
		// Get the FileName
		tmpExt = m_shortFilename.Extract(lastPos+1);
	}
	return tmpExt;
}

#ifdef DATA_IN_APK
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
		TK_DEBUG("LOAD data from the files : \"" << GetCompleateName() << "\"");
		TK_DEBUG("         name=" << zipFileProperty.name);
		TK_DEBUG("         index=" << zipFileProperty.index);
		TK_DEBUG("         crc=" << zipFileProperty.crc);
		TK_DEBUG("         mtime=" << zipFileProperty.mtime);
		TK_DEBUG("         size=" << zipFileProperty.size);
		TK_DEBUG("         comp_size=" << zipFileProperty.comp_size);
		TK_DEBUG("         comp_method=" << zipFileProperty.comp_method);
		TK_DEBUG("         encryption_method=" << zipFileProperty.encryption_method);
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

int32_t etk::File::Size(void)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			return internalDataFiles[m_idInternal].fileLenght;
		}
		return 0;
	}
	#elif defined(DATA_IN_APK)
	if (etk::FILE_TYPE_DATA == m_type) {
		if (true == LoadDataZip()) {
			return m_zipDataSize;
		}
		return 0;
	}
	#endif
	FILE *myFile=NULL;
	etk::String myCompleateName = baseFolderData + GetCompleateName();
	myFile=fopen(myCompleateName.c_str(),"rb");
	if(NULL == myFile) {
		//EWOL_ERROR("Can not find the file name=\"" << m_folder << "\" / \"" << m_shortFilename << "\"");
		return -1;
	}
	int32_t size = 0;
	fseek(myFile, 0, SEEK_END);
	size = ftell(myFile);
	fseek(myFile, 0, SEEK_SET);
	fclose(myFile);
	return size;
}


bool etk::File::Exist(void)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			return true;
		}
		return false;
	}
	#elif defined(DATA_IN_APK)
	if (etk::FILE_TYPE_DATA == m_type) {
		if (m_idZipFile >= -1  && m_idZipFile < s_APKnbFiles) {
			return true;
		}
		return false;
	}
	#endif
	FILE *myFile=NULL;
	etk::String myCompleateName = baseFolderData + GetCompleateName();
	myFile=fopen(myCompleateName.c_str(),"rb");
	if(NULL == myFile) {
		return false;
	}
	fclose(myFile);
	return true;
}



bool etk::File::fOpenRead(void)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		m_readingOffset = 0;
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			TK_DEBUG("Open file : " << GetCompleateName() << " with size=" << internalDataFiles[m_idInternal].fileLenght << " Octets");
			return true;
		}
		return false;
	}
	#elif defined(DATA_IN_APK)
	if (etk::FILE_TYPE_DATA == m_type) {
		return LoadDataZip();
	}
	#endif
	if (NULL != m_PointerFile) {
		TK_CRITICAL("File Already open : \"" << GetCompleateName() << "\"");
		return true;
	}
	etk::String myCompleateName = baseFolderData + GetCompleateName();
	m_PointerFile=fopen(myCompleateName.c_str(),"rb");
	if(NULL == m_PointerFile) {
		TK_ERROR("Can not find the file name=\"" << GetCompleateName() << "\"");
		return false;
	}
	return true;
}

bool etk::File::fOpenWrite(void)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		m_readingOffset = 0;
		return false;
	}
	#elif defined(DATA_IN_APK)
	if (etk::FILE_TYPE_DATA == m_type) {
		return false;
	}
	#endif
	if (NULL != m_PointerFile) {
		TK_CRITICAL("File Already open : \"" << GetCompleateName() << "\"");
		return true;
	}
	etk::String myCompleateName = baseFolderData + GetCompleateName();
	m_PointerFile=fopen(myCompleateName.c_str(),"wb");
	if(NULL == m_PointerFile) {
		TK_ERROR("Can not find the file name=\"" << GetCompleateName() << "\"");
		return false;
	}
	return true;
}

bool etk::File::fClose(void)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		m_readingOffset = 0;
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			return true;
		}
		return false;
	}
	#elif defined(DATA_IN_APK)
	if (etk::FILE_TYPE_DATA == m_type) {
		if (NULL == m_zipData) {
			TK_CRITICAL("File Already closed : \"" << GetCompleateName() << "\"");
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
		TK_CRITICAL("File Already closed : \"" << GetCompleateName() << "\"");
		return false;
	}
	fclose(m_PointerFile);
	m_PointerFile = NULL;
	return true;
}

char * etk::File::fGets(char * elementLine, int32_t maxData)
{
	memset(elementLine, 0, maxData);
	#ifdef DATA_INTERNAL_BINARY
	char * element = elementLine;
	if (etk::FILE_TYPE_DATA == m_type) {
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			//char * tmpData = internalDataFiles[iii].data + m_readingOffset;
			if (m_readingOffset>internalDataFiles[m_idInternal].fileLenght) {
				element[0] = '\0';
				return NULL;
			}
			while (internalDataFiles[m_idInternal].data[m_readingOffset] != '\0') {
				if(    internalDataFiles[m_idInternal].data[m_readingOffset] == '\n'
				    || internalDataFiles[m_idInternal].data[m_readingOffset] == '\r')
				{
					*element = internalDataFiles[m_idInternal].data[m_readingOffset];
					element++;
					m_readingOffset++;
					*element = '\0';
					return elementLine;
				}
				*element = internalDataFiles[m_idInternal].data[m_readingOffset];
				element++;
				m_readingOffset++;
				if (m_readingOffset>internalDataFiles[m_idInternal].fileLenght) {
					*element = '\0';
					return elementLine;
				}
			}
		}
		elementLine[0] = '\0';
		return NULL;
	}
	#elif defined(DATA_IN_APK)
	char * element = elementLine;
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
		}
		return NULL;
	}
	#endif
	return fgets(elementLine, maxData, m_PointerFile);
}

int32_t etk::File::fRead(void * data, int32_t blockSize, int32_t nbBlock)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			int32_t dataToRead = blockSize * nbBlock;
			if (dataToRead + m_readingOffset > internalDataFiles[m_idInternal].fileLenght) {
				nbBlock = ((internalDataFiles[m_idInternal].fileLenght - m_readingOffset) / blockSize);
				dataToRead = blockSize * nbBlock;
			}
			memcpy(data, &internalDataFiles[m_idInternal].data[m_readingOffset], dataToRead);
			m_readingOffset +=dataToRead;
			return nbBlock;
		}
		return 0;
	}
	#elif defined(DATA_IN_APK)
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

int32_t etk::File::fWrite(void * data, int32_t blockSize, int32_t nbBlock)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		TK_CRITICAL("Can not write on data inside memory : \"" << GetCompleateName() << "\"");
		return 0;
	}
	#elif defined(DATA_IN_APK)
	if (etk::FILE_TYPE_DATA == m_type) {
		TK_CRITICAL("Can not write on data inside APK : \"" << GetCompleateName() << "\"");
		return 0;
	}
	#endif
	return fwrite(data, blockSize, nbBlock, m_PointerFile);
}


bool etk::File::fSeek(long int offset, int origin)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			int32_t positionEnd = 0;
			switch(origin) {
				case SEEK_END:
					positionEnd = internalDataFiles[m_idInternal].fileLenght;
					break;
				case SEEK_CUR:
					positionEnd = m_readingOffset;
					break;
				default:
					positionEnd = 0;
					break;
			}
			positionEnd += offset;
			if (positionEnd < 0) {
				positionEnd = 0;
			} else if (positionEnd > internalDataFiles[m_idInternal].fileLenght) {
				positionEnd = internalDataFiles[m_idInternal].fileLenght;
			}
			m_readingOffset = positionEnd;
			return true;
		}
		return false;
	}
	#elif defined(DATA_IN_APK)
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


char * etk::File::GetDirectPointer(void)
{
	#ifdef DATA_INTERNAL_BINARY
	if (etk::FILE_TYPE_DATA == m_type) {
		if (m_idInternal >= -1  && m_idInternal < internalDataFilesSize) {
			return (char*)internalDataFiles[m_idInternal].data;
		}
	}
	#elif defined(DATA_IN_APK)
	if (etk::FILE_TYPE_DATA == m_type) {
		if (NULL == m_zipData) {
			return NULL;
		}
		return m_zipData;
	}
	#endif
	return NULL;
}
