/**
 *******************************************************************************
 * @file ewol/theme/EolElement.cpp
 * @brief basic ewol theme eol file element (Sources)
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
#define __class__	"ewol::theme::EolElement"


ewol::theme::EolElement::EolElement(void)
{
	//EWOL_DEBUG("new...");
}

ewol::theme::EolElement::~EolElement(void)
{
	RemoveAll();
}

void ewol::theme::EolElement::RemoveAll(void)
{
	for(int32_t iii=0; iii<m_listColor.Size(); iii++) {
		if (m_listColor[iii] != NULL) {
			delete m_listColor[iii];
			m_listColor[iii] = NULL;
		}
	}
	m_listColor.Clear();
	for(int32_t iii=0; iii<m_listGroup.Size(); iii++) {
		if (m_listGroup[iii] != NULL) {
			delete m_listGroup[iii];
			m_listGroup[iii] = NULL;
		}
	}
	m_listGroup.Clear();
	for(int32_t iii=0; iii<m_listElement.Size(); iii++) {
		if (m_listElement[iii] != NULL) {
			delete m_listElement[iii];
			m_listElement[iii] = NULL;
		}
	}
	m_listElement.Clear();
}

void ewol::theme::EolElement::Parse(TiXmlNode * root)
{
	// clean all internal elements ...
	RemoveAll();
	// get the name ...
	m_name = root->ToElement()->Attribute("name");
	const char * tmp = root->ToElement()->Attribute("ratio");
	if (NULL == tmp) {
		m_ratio=1.0;
	} else {
		double xxx;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf", &xxx);
		m_ratio=xxx;
	}
	etk::String tmpString = root->ToElement()->Attribute("ClipX");
	if (tmpString == "true") {
		m_clipX = true;
	} else {
		m_clipX = false;
	}
	etk::String tmpString2 = root->ToElement()->Attribute("ClipY");
	if (tmpString2 == "true") {
		m_clipY = true;
	} else {
		m_clipY = false;
	}
	
	tmp = root->ToElement()->Attribute("internalElemStart");
	if (NULL == tmp) {
		m_internalElemStart.x=0;
		m_internalElemStart.y=0;
	} else {
		double xxx,yyy;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf;%lf", &xxx,&yyy);
		m_internalElemStart.x=xxx;
		m_internalElemStart.y=yyy;
	}
	tmp = root->ToElement()->Attribute("internalElemStop");
	if (NULL == tmp) {
		m_internalElemStop.x=0;
		m_internalElemStop.y=0;
	} else {
		double xxx,yyy;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf;%lf", &xxx,&yyy);
		m_internalElemStop.x=xxx;
		m_internalElemStop.y=yyy;
	}
	EWOL_INFO("(l " << root->Row() << ") ELEMENT name=\"" << m_name << "\" internalStart(" << m_internalElemStart.x << "," << m_internalElemStart.y << ") to internalStop(" << m_internalElemStop.x << "," << m_internalElemStop.y << ") ratio=" << m_ratio );
	EWOL_INFO("  --------------------- START ---------------------");
	for(TiXmlNode * pNode=root->FirstChild(); NULL!=pNode; pNode = pNode->NextSibling()) {
		if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
			continue;
		}
		etk::String nodeValue = pNode->Value();
		if (nodeValue == "group") {
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
		} else if (nodeValue == "frame") {
			//EWOL_INFO("Find frame ... ");
			etk::String groupNameTmp = pNode->ToElement()->Attribute("name");
			if (groupNameTmp == "") {
				EWOL_ERROR("(l " << pNode->Row() << ") Group with NO name ... (not parsed)");
				// not added it
			} else {
				bool findASameName = false;
				// check if existed ...
				for (int32_t iii=0; iii < m_listElement.Size(); iii++) {
					if(NULL!=m_listElement[iii]) {
						if(m_listElement[iii]->HasName(groupNameTmp) == true) {
							findASameName = true;
							EWOL_WARNING("(l " << pNode->Row() << ") Find a Frame with the same Name : \"" << groupNameTmp <<"\"");
							m_listGroup[iii]->Parse(pNode);
							break;
						}
					}
				}
				if (findASameName == false) {
					//EWOL_DEBUG("(l " << pNode->Row() << ") Add a new Frame : \"" << groupNameTmp <<"\"");
					ewol::theme::EolElementFrame * myGroupTmp = new ewol::theme::EolElementFrame();
					if (NULL != myGroupTmp) {
						myGroupTmp->Parse(pNode);
						m_listElement.PushBack(myGroupTmp);
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
			EWOL_ERROR("(l " << pNode->Row() << ") node not suported : \"" << nodeValue <<"\" must be [group,color,frame]");
		}
	}
	EWOL_INFO("  --------------------- STOP  ---------------------");
}



bool ewol::theme::EolElement::GetColor(etk::String colorName, color_ts & selectedColor)
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

int32_t ewol::theme::EolElement::GetNbFrame(void)
{
	return m_listElement.Size();
}


int32_t ewol::theme::EolElement::GetFrameId(etk::String & frameName)
{
	for (int32_t iii=0; iii < m_listElement.Size(); iii++) {
		if(NULL!=m_listElement[iii]) {
			if(m_listElement[iii]->HasName(frameName) == true) {
				return iii;;
			}
		}
	}
	return -1;
}


etk::String ewol::theme::EolElement::GetName(void)
{
	return m_name;
}


void ewol::theme::EolElement::SetName(etk::String & newName)
{
	m_name = newName;
}


bool ewol::theme::EolElement::HasName(etk::String & newName)
{
	return m_name == newName;
}


void ewol::theme::EolElement::Generate(const ewol::theme::Theme * myTheme, int32_t frameId, ewol::OObject2DTextured & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	if (0 > frameId || frameId > m_listElement.Size()) {
		EWOL_ERROR("Did not find the frame id=" << frameId);
		return;
	}
	if (NULL != m_listElement[frameId]) {
		m_listElement[frameId]->Generate(myTheme, this, newObject, posX, posY, sizeX, sizeY);
	}
}



