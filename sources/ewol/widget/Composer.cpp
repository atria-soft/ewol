/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Composer.h>
#include <etk/os/FSNode.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"ewol::widget::Composer"

ewol::widget::Composer::Composer() {
	addObjectType("ewol::widget::Composer");
	// nothing to do ...
}

void ewol::widget::Composer::init(enum composerMode _mode, const std::string& _fileName) {
	ewol::widget::Container::init();
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

ewol::widget::Composer::~Composer() {
	
}

bool ewol::widget::Composer::loadFromFile(const std::string& _fileName) {
	exml::Document doc;
	if (doc.load(_fileName) == false) {
		EWOL_ERROR(" can not load file XML : " << _fileName);
		return false;
	}
	exml::Element* root = (exml::Element*)doc.getNamed("composer");
	if (root == nullptr) {
		// Maybe a multiple node XML for internal config:
		root = doc.toElement();
		if (root == nullptr) {
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
	if (root == nullptr) {
		// Maybe a multiple node XML for internal config:
		root = doc.toElement();
		if (root == nullptr) {
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
	requestUpdateSize();
	return true;
}


void ewol::widget::Composer::registerOnEventNameWidget(const std::string& _subWidgetName,
                                                       const char * _eventId,
                                                       const char * _eventIdgenerated,
                                                       const std::string& _overloadData) {
	registerOnEventNameWidget(shared_from_this(), _subWidgetName, _eventId, _eventIdgenerated, _overloadData);
}

void ewol::widget::Composer::registerOnEventNameWidget(const std::shared_ptr<ewol::Object>& _destinationObject,
                                                       const std::string& _subWidgetName,
                                                       const char * _eventId,
                                                       const char * _eventIdgenerated,
                                                       const std::string& _overloadData) {
	std::shared_ptr<ewol::Widget> tmpWidget = getWidgetNamed(_subWidgetName);
	if (tmpWidget != nullptr) {
		EWOL_DEBUG("Find widget named : \"" << _subWidgetName << "\" register event=\"" << _eventId << "\"");
		tmpWidget->registerOnEvent(_destinationObject, _eventId, _eventIdgenerated, _overloadData);
	} else {
		EWOL_WARNING("[" << getId() << "] {" << getObjectType() << "} Can not register event : \"" << _eventId << "\" the widget named=\"" << _subWidgetName << "\" does not exist");
	}
}
