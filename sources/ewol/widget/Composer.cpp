/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Composer.h>
#include <etk/os/FSNode.h>
#include <ewol/widget/WidgetManager.h>


widget::Composer::Composer(void)
{
	// nothing to do ...
}

widget::Composer::Composer(widget::Composer::composerMode_te mode, const etk::UString& fileName)
{
	switch(mode) {
		case widget::Composer::None:
			// nothing to do ...
			break;
		case widget::Composer::String:
			LoadFromString(fileName);
			break;
		case widget::Composer::File:
			LoadFromFile(fileName);
			break;
	}
}


widget::Composer::~Composer(void)
{
	
}

bool widget::Composer::LoadFromFile(const etk::UString& _fileName)
{
	// open the curent File
	etk::FSNode fileName(_fileName);
	if (false == fileName.Exist()) {
		EWOL_ERROR("File Does not exist : " << fileName);
		return false;
	}
	int32_t fileSize = fileName.FileSize();
	if (0==fileSize) {
		EWOL_ERROR("This file is empty : " << fileName);
		return false;
	}
	if (false == fileName.FileOpenRead()) {
		EWOL_ERROR("Can not open the file : " << fileName);
		return false;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		EWOL_ERROR("Error Memory allocation size=" << fileSize);
		return false;
	}
	memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	fileName.FileRead(fileBuffer, 1, fileSize);
	// close the file:
	fileName.FileClose();
	
	bool ret = CommonLoadXML((const char*)fileBuffer);
	
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
	return ret;
}

bool widget::Composer::LoadFromString(const etk::UString& composerXmlString)
{
	etk::Char tmpData = composerXmlString.c_str();
	return CommonLoadXML(tmpData);
}

bool widget::Composer::CommonLoadXML(const char* data)
{
	if (NULL==data) {
		return false;
	}
	TiXmlDocument XmlDocument;
	// load the XML from the memory
	XmlDocument.Parse(data, 0, TIXML_ENCODING_UTF8);
	
	TiXmlElement* root = XmlDocument.FirstChildElement("composer");
	if (NULL == root ) {
		EWOL_ERROR("(l ?) main node not find: \"composer\" ...");
		return false;
	}
	// remove previous elements ...
	RemoveSubWidget();
	ewol::Widget::LoadXML(root);
	
	for(TiXmlNode * pNode = root->FirstChild() ;
	    NULL != pNode ;
	    pNode = pNode->NextSibling() ) {
		if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
			// nothing to do, just proceed to next step
			continue;
		}
		etk::UString widgetName = pNode->Value();
		if (ewol::widgetManager::Exist(widgetName) == false) {
			EWOL_ERROR("(l "<<pNode->Row()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << ewol::widgetManager::List() << "]" );
			continue;
		}
		if (NULL != GetSubWidget()) {
			EWOL_ERROR("(l "<<pNode->Row()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
			continue;
		}
		ewol::Widget* tmpWidget = ewol::widgetManager::Create(widgetName);
		if (tmpWidget == NULL) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add widget :
		SetSubWidget(tmpWidget);
		if (false == tmpWidget->LoadXML(pNode)) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}


