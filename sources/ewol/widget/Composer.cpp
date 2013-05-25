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
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} File Does not exist : " << fileName);
		return false;
	}
	int32_t fileSize = fileName.FileSize();
	if (0==fileSize) {
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} This file is empty : " << fileName);
		return false;
	}
	if (false == fileName.FileOpenRead()) {
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Can not open the file : " << fileName);
		return false;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Error Memory allocation size=" << fileSize);
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
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} (l ?) main node not find: \"composer\" ...");
		return false;
	}
	// call upper class to parse his elements ...
	widget::Container::LoadXML(root);
	
	return true;
}


void widget::Composer::RegisterOnEventNameWidget(const etk::UString& _subWidgetName,
                                                 const char * _eventId,
                                                 const char * _eventIdgenerated,
                                                 const etk::UString& _overloadData)
{
	RegisterOnEventNameWidget(this, _subWidgetName, _eventId, _eventIdgenerated, _overloadData);
}

void widget::Composer::RegisterOnEventNameWidget(ewol::EObject * _destinationObject,
                                                 const etk::UString& _subWidgetName,
                                                 const char * _eventId,
                                                 const char * _eventIdgenerated,
                                                 const etk::UString& _overloadData)
{
	ewol::Widget* tmpWidget = GetWidgetNamed(_subWidgetName);
	if (NULL != tmpWidget) {
		//EWOL_DEBUG("Find widget named : \"" << _subWidgetName << "\" register event=\"" << _eventId << "\"");
		tmpWidget->RegisterOnEvent(_destinationObject, _eventId, _eventIdgenerated, _overloadData);
	} else {
		EWOL_WARNING("[" << GetId() << "] {" << GetObjectType() << "} Can not register event : \"" << _eventId << "\" the widget named=\"" << _subWidgetName << "\" does not exist");
	}
}
