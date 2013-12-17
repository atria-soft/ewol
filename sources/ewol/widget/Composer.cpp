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
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"ewol::widget::Composer"

ewol::widget::Composer::Composer(void) {
	// nothing to do ...
}

ewol::widget::Composer::Composer(enum composerMode _mode, const std::string& _fileName) {
	addObjectType("ewol::widget::Composer");
	switch(_mode) {
		case ewol::widget::Composer::None:
			// nothing to do ...
			break;
		case ewol::widget::Composer::String:
			loadFromString(_fileName);
			break;
		case ewol::widget::Composer::file:
			loadFromFile(_fileName);
			break;
	}
}

ewol::widget::Composer::~Composer(void) {
	
}

bool ewol::widget::Composer::loadFromFile(const std::string& _fileName) {
	exml::Document doc;
	if (doc.load(_fileName) == false) {
		EWOL_ERROR(" can not load file XML : " << _fileName);
		return false;
	}
	exml::Element* root = (exml::Element*)doc.getNamed("composer");
	if (NULL == root ) {
		// Maybe a multiple node XML for internal config:
		root = doc.toElement();
		if (NULL == root ) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l ?) main node not find: \"composer\" ...");
			return false;
		}
		if (root->size() == 0) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l ?) no node in the Container XML element.");
			return false;
		}
	}
	// call upper class to parse his elements ...
	ewol::widget::Container::loadXML(root);
	
	return true;
}

bool ewol::widget::Composer::loadFromString(const std::string& _composerXmlString) {
	exml::Document doc;
	if (doc.parse(_composerXmlString) == false) {
		EWOL_ERROR(" can not load file XML string...");
		return false;
	}
	exml::Element* root = (exml::Element*)doc.getNamed("composer");
	if (NULL == root ) {
		// Maybe a multiple node XML for internal config:
		root = doc.toElement();
		if (NULL == root ) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l ?) main node not find: \"composer\" ...");
			return false;
		}
		if (root->size() == 0) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l ?) no node in the Container XML element.");
			return false;
		}
	}
	// call upper class to parse his elements ...
	ewol::widget::Container::loadXML(root);
	
	return true;
}


void ewol::widget::Composer::registerOnEventNameWidget(const std::string& _subWidgetName,
                                                       const char * _eventId,
                                                       const char * _eventIdgenerated,
                                                       const std::string& _overloadData) {
	registerOnEventNameWidget(this, _subWidgetName, _eventId, _eventIdgenerated, _overloadData);
}

void ewol::widget::Composer::registerOnEventNameWidget(ewol::Object * _destinationObject,
                                                       const std::string& _subWidgetName,
                                                       const char * _eventId,
                                                       const char * _eventIdgenerated,
                                                       const std::string& _overloadData) {
	ewol::Widget* tmpWidget = getWidgetNamed(_subWidgetName);
	if (NULL != tmpWidget) {
		EWOL_DEBUG("Find widget named : \"" << _subWidgetName << "\" register event=\"" << _eventId << "\"");
		tmpWidget->registerOnEvent(_destinationObject, _eventId, _eventIdgenerated, _overloadData);
	} else {
		EWOL_WARNING("[" << getId() << "] {" << getObjectType() << "} Can not register event : \"" << _eventId << "\" the widget named=\"" << _subWidgetName << "\" does not exist");
	}
}
