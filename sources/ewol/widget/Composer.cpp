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
	exml::Document doc;
	if (doc.Load(_fileName)==false) {
		EWOL_ERROR(" can not load file XML : " << _fileName);
		return false;
	}
	exml::Element* root = (exml::Element*)doc.GetNamed("composer");
	if (NULL == root ) {
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} (l ?) main node not find: \"composer\" ...");
		return false;
	}
	// call upper class to parse his elements ...
	widget::Container::LoadXML(root);
	
	return true;
}

bool widget::Composer::LoadFromString(const etk::UString& _composerXmlString)
{
	exml::Document doc;
	if (doc.Parse(_composerXmlString)==false) {
		EWOL_ERROR(" can not load file XML string...");
		return false;
	}
	exml::Element* root = (exml::Element*)doc.GetNamed("composer");
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
