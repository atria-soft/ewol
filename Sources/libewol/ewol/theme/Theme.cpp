/**
 *******************************************************************************
 * @file ewol/theme/Theme.cpp
 * @brief basic ewol Theme basic class (Sources)
 * @author Edouard DUPIN
 * @date 23/11/2011
 * @par Project
 * ewol
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

#include <tinyXML/tinyxml.h>

#include <ewol/theme/Theme.h>
#include <ewol/Debug.h>

#undef __class__
#define __class__	"theme"


void ewol::theme::Load(etk::File & localFile)
{
	if (localFile.HasExtention() == false) {
		EWOL_ERROR("can not load theme with file, has no extention : " << localFile);
		return;
	}
	if (localFile.GetExtention() != "eol") {
		EWOL_ERROR("can not load theme with file, has not extention .eol : " << localFile);
		return;
	}
	if (false == localFile.Exist()) {
		EWOL_ERROR("File does not Exist ... " << localFile);
		return;
	} else {
		TiXmlDocument XmlDocument;
		// open the curent File
		if (false == localFile.Exist()) {
			EWOL_ERROR("File Does not exist : " << localFile);
			return;
		}
		int32_t fileSize = localFile.Size();
		if (0==fileSize) {
			EWOL_ERROR("This file is empty : " << localFile);
			return;
		}
		if (false == localFile.fOpenRead()) {
			EWOL_ERROR("Can not open the file : " << localFile);
			return;
		}
		// allocate data
		char * fileBuffer = new char[fileSize+5];
		if (NULL == fileBuffer) {
			EWOL_ERROR("Error Memory allocation size=" << fileSize);
			return;
		}
		memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
		// load data from the file :
		localFile.fRead(fileBuffer, 1, fileSize);
		// close the file:
		localFile.fClose();
		// load the XML from the memory
		XmlDocument.Parse((const char*)fileBuffer, 0, TIXML_ENCODING_UTF8);
		
		TiXmlElement* root = XmlDocument.FirstChildElement( "ewolTheme" );
		if (NULL == root ) {
			EWOL_ERROR("(l ?) main node not find: \"ewolTheme\" in \"" << localFile << "\"");
			return;
		} else {
			for(TiXmlNode * pNode=root->FirstChild(); NULL!=pNode; pNode = pNode->NextSibling()) {
				if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
					continue;
				}
				EWOL_TODO("default theme loading");
				//EWOL_ERROR("(l " << pNode->Row() << ") node not suported : \"" << nodeValue <<"\" must be [???,???]");
			}
		}
		if (NULL != fileBuffer) {
			delete[] fileBuffer;
		}
	}
}
