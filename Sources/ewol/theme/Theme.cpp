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
		if(    line[iii]=='\0'
		    || line[iii]=='\r'
		    || line[iii]=='\n') {
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


static int32_t Offset(char * line)
{
	int32_t offset = 0;
	int32_t spaceCount = 0;
	for(int32_t iii=0; iii<MAX_LINE_SIZE; iii++) {
		if(    line[iii]=='\0') {
			break;
		}
		if(    line[iii]!='\t'
		    && line[iii]!=' ') {
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
	return offset;
}


static int32_t LineLen(char * pointer)
{
	int32_t nbElement = 0;
	while(    *pointer != '\0'
	       && *pointer != '\n'
	       && *pointer != '\r') {
		nbElement++;
		pointer++;
	}
	return nbElement;
}


static char* NextLineStart(char * pointer)
{
	bool find = false;
	while(*pointer != '\0') {
		if(    *pointer == '\n'
		    || *pointer == '\r') {
			find = true;
		} else if (true==find){
			return pointer;
		}
		pointer++;
	}
	return NULL;
}

static int32_t LenSpecificIndent(char * pointer, int32_t identSize)
{
	char * pointerStart = pointer;
	while(NULL!=pointer) {
		// check comment //
		if (false == IsComment(pointer)) {
			// need to parse the element...
			int32_t offset = Offset(pointer);
			if (identSize > offset) {
				break;
			}
		}
		pointer=NextLineStart(pointer);
	}
	return pointer - pointerStart;
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
		int32_t fileSize = newFile.Size();
		char *  fileData;
		int32_t lineID=1;
		FILE * file = fopen(newFile.GetCompleateName().c_str(),"r");
		if(NULL == file) {
			EWOL_ERROR("Can not OPEN the file name=\"" << newFile << "\"");
			return;
		}
		fileData = new char[fileSize+1];
		if (NULL == fileData) {
			fclose(file);
			EWOL_ERROR("Can not Allocate DATA ... file=\"" << newFile << "\"");
			return;
		}
		if (fileSize != (int64_t)fread(fileData, 1, fileSize, file) ) {
			fclose(file);
			EWOL_ERROR("Error to load file DATA ... file=\"" << newFile << "\"");
			return;
		}
		// end of the string ...
		fileData[fileSize] = '\0';
		// close unneded open file
		fclose(file);
		// get the next line : 
		char * elementLine = fileData;
		int32_t lineLen = LineLen(elementLine);
		// load all element of the file ...
		if(    3 > lineLen
		    && (    elementLine[0] != 'e'
		         || elementLine[1] != 'o'
		         || elementLine[2] != 'l') ) {
			EWOL_ERROR("Start file in error " << newFile << " ==> will start with eol \\n");
			delete[] fileData;
			return;
		}
		// TODO : On devrait décapitalise le fichier ... pour eviter toutes erreur de CAST ... (A voir ... maais je ne suis pas sur...)
		// TODO : Remove comment Lines ...
		enum {MODE_UNKNOW, MODE_TEXT, MODE_BINARY} fileMode = MODE_UNKNOW;
		bool SupportedVersion=false;
		lineID++;
		elementLine=NextLineStart(elementLine);
		while(NULL!=elementLine) {
			// get the current Line lenght:
			lineLen = LineLen(elementLine);
			// check comment //
			if (true == IsComment(elementLine)) {
				EWOL_VERBOSE("(l=" << lineID << ") Find a comment : \"" << etk::String(elementLine, lineLen) << "\"");
			} else {
				// need to parse the element...
				int32_t offset = Offset(elementLine);
				if (-1 == offset) {
					EWOL_ERROR("(l=" << lineID << ") indentation error 4 spaces or 1 tab : \"" << etk::String(elementLine, lineLen) << "\"");
					delete[] fileData;
					return;
				} else if (0==offset) {
					if (0==strncmp(elementLine, "Mode=Text", 9)) {
						fileMode = MODE_TEXT;
						EWOL_INFO("(l=" << lineID << ") getting Mode : Text");
					} else if (0==strncmp(elementLine, "Mode=Bin", 8)) {
						fileMode = MODE_BINARY;
						EWOL_INFO("(l=" << lineID << ") getting Mode : Binary");
					} else if (0==strncmp(elementLine, "Version=1.0", 11)) {
						SupportedVersion = true;
						EWOL_INFO("(l=" << lineID << ") getting Version 1.0");
					} else if (lineLen>=3 && elementLine[0] == '<') {
						break;
						EWOL_INFO("(l=" << lineID << ") getting ELEMENT <>       (indent=" << offset << ") : \"" << etk::String(elementLine,lineLen) << "\"");
					} else{
						EWOL_WARNING("(l=" << lineID << ") Un-Interpreted Line : \"" << etk::String(elementLine, lineLen) << "\"");
					}
				} else {
					EWOL_ERROR("(l=" << lineID << ") Not Permeted before the first <...> : \"" << etk::String(elementLine, lineLen) << "\"");
					delete[] fileData;
					return;
				}
			}
			lineID++;
			elementLine=NextLineStart(elementLine);
		}
		// Check Version And mode ...
		
		if (fileMode != MODE_TEXT || SupportedVersion==false) {
			EWOL_ERROR("==> Not Supported Type of file Mode and/or version of file Mode .... ");
			delete[] fileData;
			return;
		}
		
		
		while(NULL!=elementLine) {
			// get the current Line lenght:
			lineLen = LineLen(elementLine);
			// check comment //
			if (true == IsComment(elementLine)) {
				EWOL_VERBOSE("(l=" << lineID << ") Find a comment : \"" << etk::String(elementLine, lineLen) << "\"");
			} else {
				// need to parse the element...
				int32_t offset = Offset(elementLine);
				if (-1 == offset) {
					EWOL_ERROR("(l=" << lineID << ") indentation error 4 spaces or 1 tab : \"" << etk::String(elementLine, lineLen) << "\"");
					delete[] fileData;
					return;
				} else if (0==offset) {
					if (lineLen>=3 && elementLine[0] == '<') {
						int32_t nbElement = 1;
						bool find = false;
						while(nbElement < lineLen) {
							if (elementLine[nbElement] == '>') {
								find = true;
								break;
							}
							nbElement++;
						}
						if (true == find) {
							EWOL_INFO("(l=" << lineID << ") getting ELEMENT <" << etk::String(&elementLine[1],nbElement-1)<< ">");
							int32_t lenMM = LenSpecificIndent(NextLineStart(elementLine), 1);
							EWOL_INFO("(l=" << lineID+1 << ") sub Parse : \"" << etk::String(NextLineStart(elementLine), lenMM)<< "\"");
							
						} else {
							EWOL_WARNING("(l=" << lineID << ") Un-Interpreted Line : \"" << etk::String(elementLine, lineLen) << "\"");
						}
					} else {
						EWOL_WARNING("(l=" << lineID << ") Un-Interpreted Line : \"" << etk::String(elementLine, lineLen) << "\"");
					}
				} else {
					/*elementLine += offset;
					lineLen -= offset:
					if (elementLine[0] == '<') {
						
					} else if (elementLine[0] == '[') {
						EWOL_INFO("(l=" << lineID << ") getting block []         (indent=" << offset << ") : \"" << etk::String(elementLine,lineLen) << "\"");
					} else if (elementLine[0] == '{') {
						EWOL_INFO("(l=" << lineID << ") getting drawing basic {} (indent=" << offset << ") : \"" << etk::String(elementLine,lineLen) << "\"");
					} else {
						EWOL_DEBUG("(l=" << lineID << ") data OTHER ...          (indent=" << offset << ") : \"" << etk::String(elementLine,lineLen) << "\"");
					}
					*/
					//EWOL_WARNING("(l=" << lineID << ") Un-Interpreted Line : (indent=" << offset << ") : \"" << etk::String(elementLine,lineLen) << "\"");
				}
			}
			lineID++;
			elementLine=NextLineStart(elementLine);
		}
		delete[] fileData;
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
