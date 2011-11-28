/**
 *******************************************************************************
 * @file ewol/theme/EolElementFrame.cpp
 * @brief basic ewol theme eol file element (specific frame) (Sources)
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
#include <ewol/theme/EolElementFrame.h>

#undef __class__
#define __class__	"ewol::theme::EolElementFrame"



ewol::theme::EolElementFrame::EolElementFrame(void)
{
	//EWOL_DEBUG("new...");
}

ewol::theme::EolElementFrame::~EolElementFrame(void)
{
	RemoveAll();
}

void ewol::theme::EolElementFrame::RemoveAll(void)
{
	for(int32_t iii=0; iii<m_description.Size(); iii++) {
		if (m_description[iii] != NULL) {
			delete m_description[iii];
			m_description[iii] = NULL;
		}
	}
	m_description.Clear();
}

void ewol::theme::EolElementFrame::Parse(TiXmlNode * root)
{
	RemoveAll();
	m_name = root->ToElement()->Attribute("name");
	EWOL_INFO("(l " << root->Row() << ") Group name=\"" << m_name << "\" " );
	for(TiXmlNode * pNode=root->FirstChild(); NULL!=pNode; pNode = pNode->NextSibling()) {
		if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
			continue;
		}
		etk::String nodeValue = pNode->Value();
		ewol::theme::EolBase * myBaseTmp = NULL;
		if (nodeValue == "line") {
			//EWOL_INFO("Find baseElement Line");
			myBaseTmp = new ewol::theme::EolBaseLine();
			if (NULL == myBaseTmp) {
				EWOL_ERROR("(l " << pNode->Row() << ")     Error Allocation : \"" << nodeValue <<"\"");
			}
		} else if (nodeValue == "rect") {
			//EWOL_INFO("Find baseElement Rectangle");
			myBaseTmp = new ewol::theme::EolBaseRect();
			if (NULL == myBaseTmp) {
				EWOL_ERROR("(l " << pNode->Row() << ")     Error Allocation : \"" << nodeValue <<"\"");
			}
		} else if (nodeValue == "link") {
			EWOL_WARNING("(l " << pNode->Row() << ")     Not Parsed now : \"" << nodeValue <<"\"");
		} else if (nodeValue == "triangle") {
			EWOL_WARNING("(l " << pNode->Row() << ")     Not Parsed now : \"" << nodeValue <<"\"");
		} else if (nodeValue == "polygone") {
			EWOL_WARNING("(l " << pNode->Row() << ")     Not Parsed now : \"" << nodeValue <<"\"");
		}  else if (nodeValue == "circle") {
			EWOL_WARNING("(l " << pNode->Row() << ")     Not Parsed now : \"" << nodeValue <<"\"");
		}  else if (nodeValue == "circlePart") {
			EWOL_WARNING("(l " << pNode->Row() << ")     Not Parsed now : \"" << nodeValue <<"\"");
		} else {
			EWOL_ERROR("(l " << pNode->Row() << ")     Node not suported : \"" << nodeValue <<"\" must be [line,rect,link,triangle,polygone,circle,circlePart]");
		}
		if (NULL != myBaseTmp) {
			myBaseTmp->Parse(pNode);
			m_description.PushBack(myBaseTmp);
		}
	}
	
}


etk::String ewol::theme::EolElementFrame::GetName(void)
{
	return m_name;
}


void ewol::theme::EolElementFrame::SetName(etk::String & newName)
{
	m_name = newName;
}


bool ewol::theme::EolElementFrame::HasName(etk::String & newName)
{
	return m_name == newName;
}


void ewol::theme::EolElementFrame::Generate(const ewol::theme::Theme * myTheme, const ewol::theme::EolElement * myElement, ewol::OObject2DTextured & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	
}





