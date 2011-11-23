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

#include <ewol/theme/Theme.h>

#undef __class__
#define __class__	"ewol::theme::Theme"

#define MAX_LINE_SIZE		(2048)

ewol::theme::Theme::Theme(void)
{
	
}


ewol::theme::Theme::~Theme(void)
{
	
}

static bool IsComment(char * line)
{
	for(int32_t iii=0; iii<MAX_LINE_SIZE; iii++) {
		if(    line[iii]=='\r'
		    || line[iii]=='\n') {
			line[iii]='\0';
			break;
		}
	}
	for(int32_t iii=0; iii<MAX_LINE_SIZE; iii++) {
		if(    line[iii]=='\0') {
			// enpty line ...
			return true; //==> same a a comment ...
		}
		if(    line[iii]!='\t'
		    && line[iii]!=' ') {
			if(line[iii]=='#') {
				// find a comment
				return true;
			} else {
				return false;
			}
		}
	}
	// empty line ...
	return true;
}

static int32_t ClipOffset(char * line)
{
	int32_t offset = 0;
	int32_t spaceCount = 0;
	int32_t copyOffset = 0;
	for(int32_t iii=0; iii<MAX_LINE_SIZE; iii++) {
		if(    line[iii]=='\0') {
			break;
		}
		if(    line[iii]!='\t'
		    && line[iii]!=' ') {
			// end of parsing
			copyOffset = iii;
			break;
		} else {
			if(line[iii]==' ') {
				spaceCount++;
				if (spaceCount == 4) {
					spaceCount = 0;
					offset++;
				}
			} else {
				spaceCount = 0;
				offset++;
			}
		}
	}
	if (spaceCount != 0) {
		return -1;
	}
	memmove(line, line + copyOffset, MAX_LINE_SIZE-copyOffset);
	return offset;
}

void ewol::theme::Theme::Load(etk::File & newFile, bool defaultTheme)
{
	if (newFile.HasExtention() == false) {
		EWOL_ERROR("can not load theme with file, has no extention : " << newFile);
		return;
	}
	if (newFile.GetExtention() != "eol") {
		EWOL_ERROR("can not load theme with file, has not extention .eol : " << newFile);
		return;
	}
	if (false == newFile.Exist()) {
		EWOL_ERROR("File does not Exist ... " << newFile);
		return;
	} else {
		char elementLine[MAX_LINE_SIZE];
		int32_t lineID=1;
		FILE * file = fopen(newFile.GetCompleateName().c_str(),"r");
		if(NULL == file) {
			EWOL_ERROR("Can not OPEN the file name=\"" << newFile << "\"");
			return;
		}
		// load all element of the file ...
		if (NULL != fgets(elementLine, MAX_LINE_SIZE, file)) {
			if(    elementLine[0] != 'e'
			    || elementLine[1] != 'o'
			    || elementLine[2] != 'l') {
				EWOL_ERROR("Start file in error " << newFile << " ==> will start with eol \\n");
				return;
			}
		}
		lineID++;
		while (NULL != fgets(elementLine, MAX_LINE_SIZE, file) )
		{
			if (true == IsComment(elementLine)) {
				EWOL_VERBOSE("(l=" << lineID << ") Find a comment : \"" << elementLine << "\"");
			} else {
				// need to parse the element...
				int32_t offset = ClipOffset(elementLine);
				if (offset == -1) {
					EWOL_ERROR("(l=" << lineID << ") indentation error 4 spaces or 1 tab : \"" << elementLine << "\"");
					return;
				} else {
					if (elementLine[0] == '<') {
						EWOL_INFO("(l=" << lineID << ") getting ELEMENT <>       (indent=" << offset << ") : \"" << elementLine << "\"");
					} else if (elementLine[0] == '[') {
						EWOL_INFO("(l=" << lineID << ") getting block []         (indent=" << offset << ") : \"" << elementLine << "\"");
					} else if (elementLine[0] == '{') {
						EWOL_INFO("(l=" << lineID << ") getting drawing basic {} (indent=" << offset << ") : \"" << elementLine << "\"");
					} else {
						EWOL_DEBUG("(l=" << lineID << ") data OTHER ...          (indent=" << offset << ") : \"" << elementLine << "\"");
					}
				}
			}
			lineID++;
		}
	}
}


void ewol::theme::Theme::Generate(int32_t id, int32_t frameId, OObject2DTextured & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	
}

int32_t ewol::theme::Theme::GetNbFrame(int32_t id)
{
	return 0;
}

int32_t ewol::theme::Theme::GetFrameId(int32_t id, etk::String & frameName)
{
	return 0;
}


int32_t ewol::theme::Theme::GetObjectId(etk::String name)
{
	return -1;
}
