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


#undef __class__
#define __class__	"etk::File"


etk::CCout& etk::operator <<(etk::CCout &os, const etk::File &obj)
{
	os << obj.m_folder;
	os << "/";
	os << obj.m_shortFilename;
	return os;
}

etk::File::File(etk::String &filename, int32_t LineNumber)
{
	m_lineNumberOpen = LineNumber;
	SetCompleateName(filename);
}


etk::File::File(const char  *filename, int32_t LineNumber)
{
	etk::String tmpString = filename;
	m_lineNumberOpen = LineNumber;
	SetCompleateName(tmpString);
}


etk::File::File(etk::String &filename, etk::String &folder, int32_t lineNumber)
{
	etk::String tmpString = folder;
	tmpString += '/';
	tmpString += filename;
	SetCompleateName(tmpString);
	m_lineNumberOpen = lineNumber;
}

etk::File::~File(void)
{
	// nothing to do ...
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


void etk::File::SetCompleateName(etk::String &newFilename)
{
	char buf[MAX_FILE_NAME];
	memset(buf, 0, MAX_FILE_NAME);
	char * ok;
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
	if ('/' != *destFilename.c_str()) {
		// Get the command came from the running of the program : 
		char cCurrentPath[FILENAME_MAX];
		#if __PLATFORM__ == Android
			strcpy(cCurrentPath, "./");
		#else
		if (!getcwd(cCurrentPath, FILENAME_MAX)) {
			return;
		}
		#endif
		cCurrentPath[FILENAME_MAX - 1] = '\0';
		etk::String tmpFilename = destFilename;
		destFilename = cCurrentPath;
		destFilename += '/';
		destFilename += tmpFilename;
	}
	TK_VERBOSE("3 : Get file Name : " << destFilename );
	
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
	TK_VERBOSE("Set FileName :\"" << m_folder << "\" / \"" << m_shortFilename << "\" ");
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



int32_t etk::File::Size(void)
{
	FILE *myFile=NULL;
	etk::String myCompleateName = GetCompleateName();
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
	FILE *myFile=NULL;
	etk::String myCompleateName = GetCompleateName();
	myFile=fopen(myCompleateName.c_str(),"rb");
	if(NULL == myFile) {
		return false;
	}
	fclose(myFile);
	return true;
}
