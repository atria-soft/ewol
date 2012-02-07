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

#undef __class__
#define __class__	"ewol::theme::Theme"

#define MAX_LINE_SIZE		(2048)

ewol::theme::Theme::Theme(void)
{
	
}


ewol::theme::Theme::~Theme(void)
{
	
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
		TiXmlDocument XmlDocument;
		// open the curent File
		if (false == newFile.Exist()) {
			EWOL_ERROR("File Does not exist : " << newFile);
			return;
		}
		int32_t fileSize = newFile.Size();
		if (0==fileSize) {
			EWOL_ERROR("This file is empty : " << newFile);
			return;
		}
		if (false == newFile.fOpenRead()) {
			EWOL_ERROR("Can not open the file : " << newFile);
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
		newFile.fRead(fileBuffer, 1, fileSize);
		// close the file:
		newFile.fClose();
		// load the XML from the memory
		XmlDocument.Parse((const char*)fileBuffer, 0, TIXML_ENCODING_UTF8);
		
		TiXmlElement* root = XmlDocument.FirstChildElement( "eol" );
		if (NULL == root ) {
			EWOL_ERROR("(l ?) main node not find: \"eol\" in \"" << newFile << "\"");
			return;
		} else {
			for(TiXmlNode * pNode=root->FirstChild(); NULL!=pNode; pNode = pNode->NextSibling()) {
				if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
					continue;
				}
				etk::String nodeValue = pNode->Value();
				if (nodeValue == "element") {
					//EWOL_INFO("Find ELEMENT ... ");
					etk::String elementNameTmp = pNode->ToElement()->Attribute("name");
					if (elementNameTmp == "") {
						EWOL_ERROR("(l " << pNode->Row() << ") Element with NO name ... (not parsed)");
						// not added it
					} else {
						bool findASameName = false;
						// check if existed ...
						for (int32_t iii=0; iii < m_listElement.Size(); iii++) {
							if(NULL!=m_listElement[iii]) {
								if(m_listElement[iii]->HasName(elementNameTmp) == true) {
									findASameName = true;
									EWOL_WARNING("(l " << pNode->Row() << ") Find a Group with the same Name : \"" << elementNameTmp <<"\"");
									m_listElement[iii]->Parse(pNode);
									break;
								}
							}
						}
						if (findASameName == false) {
							//EWOL_DEBUG("(l " << pNode->Row() << ") Add a new Element : \"" << elementNameTmp <<"\"");
							ewol::theme::EolElement * myElementTmp = new ewol::theme::EolElement();
							if (NULL != myElementTmp) {
								myElementTmp->Parse(pNode);
								m_listElement.PushBack(myElementTmp);
							} else {
								EWOL_ERROR("(l " << pNode->Row() << ") Error Allocation : \"" << nodeValue <<"\"");
							}
						}
					}
				} else if (nodeValue == "group") {
					//EWOL_INFO("Find group ... ");
					etk::String groupNameTmp = pNode->ToElement()->Attribute("name");
					if (groupNameTmp == "") {
						EWOL_ERROR("(l " << pNode->Row() << ") Group with NO name ... (not parsed)");
						// not added it
					} else {
						bool findASameName = false;
						// check if existed ...
						for (int32_t iii=0; iii < m_listGroup.Size(); iii++) {
							if(NULL!=m_listGroup[iii]) {
								if(m_listGroup[iii]->HasName(groupNameTmp) == true) {
									findASameName = true;
									EWOL_WARNING("(l " << pNode->Row() << ") Find a Group with the same Name : \"" << groupNameTmp <<"\"");
									m_listGroup[iii]->Parse(pNode);
									break;
								}
							}
						}
						if (findASameName == false) {
							//EWOL_DEBUG("(l " << pNode->Row() << ") Add a new Group : \"" << groupNameTmp <<"\"");
							ewol::theme::EolElementFrame * myGroupTmp = new ewol::theme::EolElementFrame();
							if (NULL != myGroupTmp) {
								myGroupTmp->Parse(pNode);
								m_listGroup.PushBack(myGroupTmp);
							} else {
								EWOL_ERROR("(l " << pNode->Row() << ") Error Allocation : \"" << nodeValue <<"\"");
							}
						}
					}
				} else if (nodeValue == "color") {
					etk::String colorNameTmp = pNode->ToElement()->Attribute("name");
					if (colorNameTmp == "") {
						EWOL_ERROR("(l " << pNode->Row() << ") Color with NO name ... (not parsed)");
						// not added it
					} else {
						bool findASameName = false;
						// check if existed ...
						for (int32_t iii=0; iii < m_listColor.Size(); iii++) {
							if(NULL!=m_listColor[iii]) {
								if(m_listColor[iii]->HasName(colorNameTmp) == true) {
									findASameName = true;
									EWOL_WARNING("(l " << pNode->Row() << ") Find a color with the same Name : \"" << colorNameTmp <<"\"");
									m_listColor[iii]->Parse(pNode);
									break;
								}
							}
						}
						if (findASameName == false) {
							//EWOL_DEBUG("(l " << pNode->Row() << ") Add a new COLOR : \"" << colorNameTmp <<"\"");
							ewol::theme::EolColor * myColorTmp = new ewol::theme::EolColor();
							if (NULL != myColorTmp) {
								myColorTmp->Parse(pNode);
								m_listColor.PushBack(myColorTmp);
							} else {
								EWOL_ERROR("(l " << pNode->Row() << ") Error Allocation : \"" << nodeValue <<"\"");
							}
						}
					}
				} else {
					EWOL_ERROR("(l " << pNode->Row() << ") node not suported : \"" << nodeValue <<"\" must be [group,color,element]");
				}
			}
		}
		if (NULL != fileBuffer) {
			delete[] fileBuffer;
		}
	}
}


bool ewol::theme::Theme::GetColor(etk::String colorName, color_ts & selectedColor)
{
	for (int32_t iii=0; iii < m_listColor.Size(); iii++) {
		if(NULL!=m_listColor[iii]) {
			if(m_listColor[iii]->HasName(colorName) == true) {
				selectedColor = m_listColor[iii]->Get();
				return true;
			}
		}
	}
	selectedColor.red = 0.0;
	selectedColor.green = 0.0;
	selectedColor.blue = 0.0;
	selectedColor.alpha = 1.0;
	return false;
}

void ewol::theme::Theme::Generate(int32_t id, int32_t frameId, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	if (0 > id || id > m_listElement.Size()) {
		return;
	}
	if (NULL != m_listElement[id]) {
		m_listElement[id]->Generate(this, frameId, newObject, posX, posY, sizeX, sizeY);
	}
}

bool ewol::theme::Theme::GenerateGroup(etk::String groupName, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	if (groupName == "") {
		EWOL_ERROR("Did not find the group name=" << groupName);
		return -1;
	}
	for (int32_t iii=0; iii < m_listGroup.Size(); iii++) {
		if(NULL!=m_listGroup[iii]) {
			if(m_listGroup[iii]->HasName(groupName) == true) {
				// TODO : Call Group display ...
				return true;
			}
		}
	}
	return false;
}


int32_t ewol::theme::Theme::GetNbFrame(int32_t id)
{
	if (0 > id || id > m_listElement.Size()) {
		EWOL_ERROR("Did not find the Element id=" << id);
		return 0;
	}
	if (NULL != m_listElement[id]) {
		return m_listElement[id]->GetNbFrame();
	}
	return 0;
}

int32_t ewol::theme::Theme::GetFrameId(int32_t id, etk::String & frameName)
{
	if (0 > id || id > m_listElement.Size()) {
		EWOL_ERROR("Did not find the Element named=" << frameName);
		return -1;
	}
	if (NULL != m_listElement[id]) {
		return m_listElement[id]->GetFrameId(frameName);
	}
	return -1;
}


int32_t ewol::theme::Theme::GetObjectId(etk::String name)
{
	if (name == "") {
		return -1;
	}
	for (int32_t iii=0; iii < m_listElement.Size(); iii++) {
		if(NULL!=m_listElement[iii]) {
			if(m_listElement[iii]->HasName(name) == true) {
				return iii;
			}
		}
	}
	EWOL_ERROR("Did not find the Element named=" << name);
	return -1;
}
