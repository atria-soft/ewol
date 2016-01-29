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
#include <ewol/context/Context.h>

#undef __class__
#define __class__	"ewol::widget::Composer"

ewol::widget::Composer::Composer() {
	addObjectType("ewol::widget::Composer");
	// nothing to do ...
}

std::shared_ptr<ewol::Widget> ewol::widget::composerGenerate(enum ewol::widget::Composer::composerMode _mode, const std::string& _data) {
	ewol::widget::Manager& widgetManager = ewol::getContext().getWidgetManager();
	if (_data == "") {
		return nullptr;
	}
	std::shared_ptr<exml::Document> doc = exml::Document::create();
	switch(_mode) {
		case ewol::widget::Composer::None:
			EWOL_ERROR("Not specify the type for compositing dynamic creation");
			return nullptr;
		case ewol::widget::Composer::String:
			if (doc->parse(_data) == false) {
				EWOL_ERROR(" can not load file XML string...");
				return nullptr;
			}
			break;
		case ewol::widget::Composer::file:
			if (doc->load(_data) == false) {
				EWOL_ERROR(" can not load file XML : " << _data);
				return nullptr;
			}
			break;
	}
	std::shared_ptr<const exml::Element> root = doc->toElement();
	if (root->size() == 0) {
		EWOL_ERROR(" (l ?) No node in the XML file/string.");
		return nullptr;
	}
	if (root->size() > 1) {
		EWOL_WARNING(" (l ?) More than 1 node in the XML file/string. (JUST parse the first)");
	}
	std::shared_ptr<const exml::Element> pNode = root->getElement(0);
	if (pNode == nullptr) {
		EWOL_ERROR(" (l ?) No node in the XML file/string. {2}");
		return nullptr;
	}
	std::string widgetName = pNode->getValue();
	if (widgetManager.exist(widgetName) == false) {
		EWOL_ERROR("(l "<<pNode->getPos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << widgetManager.list() << "]" );
		return nullptr;
	}
	EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
	std::shared_ptr<ewol::Widget> tmpWidget = widgetManager.create(widgetName);
	if (tmpWidget == nullptr) {
		EWOL_ERROR ("(l "<<pNode->getPos()<<") Can not create the widget : \"" << widgetName << "\"");
		return nullptr;
	}
	if (tmpWidget->loadXML(pNode) == false) {
		EWOL_ERROR ("(l "<<pNode->getPos()<<") can not load widget properties : \"" << widgetName << "\"");
	}
	return tmpWidget;
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
	std::shared_ptr<exml::Document> doc = exml::Document::create();
	if (doc->load(_fileName) == false) {
		EWOL_ERROR(" can not load file XML : " << _fileName);
		return false;
	}
	std::shared_ptr<const exml::Element> root = doc->getNamed("composer");
	if (root == nullptr) {
		// Maybe a multiple node XML for internal config:
		root = doc->toElement();
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
	std::shared_ptr<exml::Document> doc = exml::Document::create();
	if (doc->parse(_composerXmlString) == false) {
		EWOL_ERROR(" can not load file XML string...");
		return false;
	}
	std::shared_ptr<const exml::Element> root = doc->getNamed("composer");
	if (root == nullptr) {
		// Maybe a multiple node XML for internal config:
		root = doc->toElement();
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
